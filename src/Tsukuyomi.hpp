#ifndef UI_ENGINE_HPP
#define UI_ENGINE_HPP

#include <thread>
#include <chrono>

#include "ui/extern_ui.hpp"
#include "ui/ui_element.hpp"
#include "ui/layout.hpp"
#include "ui/text_box.hpp"
#include "ui/button.hpp"
#include "ui/input_field.hpp"
#include "ui/pannel.hpp"
#include "ui/background.hpp"
#include "ui/menu.hpp"
#include "ui/animations.hpp"
#include "ui/event.hpp"
#include "ui/input_handler.hpp"

#include "utilities/timer.hpp"


enum class UPDATE_REQ {
    RESIZE = 0,
};

template <class Main, class Overlay>
class UI_Handler {
static_assert(std::is_base_of<UI_Element, Main>::value, "Main must derive from UI_Element");
static_assert(std::is_base_of<UI_Element, Overlay>::value, "Overlay must derive from UI_Element");
protected:
    int refresh_rate;

    std::shared_ptr<Graphics_Context>   context;
    std::shared_ptr<Event_Handler>      event_handler;
    std::shared_ptr<Animation_Handler>  animation_handler;
    std::shared_ptr<Layout> layout;
    std::shared_ptr<Main> main_target;
    std::shared_ptr<Overlay> overlay;

    std::deque<UPDATE_REQ> update_requests;

    std::function<void(Event, std::shared_ptr<Graphics_Context>, std::shared_ptr<Animation_Handler>, std::shared_ptr<Main>, std::shared_ptr<Overlay>)> customMainInputHandler;
    std::function<void(Event, std::shared_ptr<Graphics_Context>, std::shared_ptr<Animation_Handler>, std::shared_ptr<Main>, std::shared_ptr<Overlay>)> customOverlayInputHandler;

public:
    bool done {true};
    UI_Handler(int W_H, int W_W, std::string Title, int refresh_rate = 120);
    ~UI_Handler();
public:
    void setMain(std::shared_ptr<Main> main_target);
    void setOverlay(std::shared_ptr<Overlay> overlay);

    // std::weak_ptr<Animation_Handler> getAnimationHandler();
    // std::weak_ptr<Event_Handler> getEventHandler();
    // std::weak_ptr<Graphics_Context> getGraphicsContext();
    
    std::shared_ptr<Layout> getLayout();
    std::shared_ptr<Animation_Handler> getAnimationHandler();
    std::shared_ptr<Event_Handler> getEventHandler();
    std::shared_ptr<Graphics_Context> getGraphicsContext();

    void registerCustomMainInputHandler(std::function<void(Event, std::shared_ptr<Graphics_Context>, std::shared_ptr<Animation_Handler>, std::shared_ptr<Main>, std::shared_ptr<Overlay>)> customMainInputHandler);
    void registerCustomOverlayInputHandler(std::function<void(Event, std::shared_ptr<Graphics_Context>, std::shared_ptr<Animation_Handler>, std::shared_ptr<Main>, std::shared_ptr<Overlay>)> customOverlayInputHandler);
public:
    virtual void update();
    virtual void updateRequest(UPDATE_REQ request);
    virtual bool main();
    // virtual bool main_async();
    // void pause();
    // virtual bool quit();
};

template <class Main, class Overlay>
UI_Handler<Main, Overlay>::UI_Handler(int W_W, int W_H, std::string Title, int refresh_rate)
        :   refresh_rate{refresh_rate} {
            context = std::make_shared<Graphics_Context>(W_W, W_H, Title);
            event_handler = std::make_shared<Event_Handler>(context);
            animation_handler = std::make_shared<Animation_Handler>();

            layout = std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0, 0, 1, 1), Container(0, 0, 1, 1)}));

            customMainInputHandler = [](Event, std::shared_ptr<Graphics_Context>, std::shared_ptr<Animation_Handler>, std::shared_ptr<Main>, std::shared_ptr<Overlay>){
                // std::cout << "No custom main input handler implemented...\n";
            };

            customOverlayInputHandler = [](Event, std::shared_ptr<Graphics_Context>, std::shared_ptr<Animation_Handler>, std::shared_ptr<Main>, std::shared_ptr<Overlay>){
                // std::cout << "No custom overlay input handler implemented...\n";
            };
        }

template <class Main, class Overlay>
UI_Handler<Main, Overlay>::~UI_Handler() {
    // quit();
}

template <class Main, class Overlay>
void UI_Handler<Main, Overlay>::setMain(std::shared_ptr<Main> main_target) {
    // quit();
    this->main_target = main_target;
    this->layout->placeUI_Element(this->main_target, 0);
    // if(!done) main();
}

template <class Main, class Overlay>
void UI_Handler<Main, Overlay>::setOverlay(std::shared_ptr<Overlay> overlay) {
    // quit();
    this->overlay = overlay;
    this->layout->placeUI_Element(this->overlay, 1);
    // if(!done) main();
}

// template <class Main, class Overlay>
// std::weak_ptr<Animation_Handler> UI_Handler<Main, Overlay>::getAnimationHandler() {
//     return this->animation_handler;
// }

// template <class Main, class Overlay>
// std::weak_ptr<Event_Handler> UI_Handler<Main, Overlay>::getEventHandler(){
//     return this->event_handler;
// }

// template <class Main, class Overlay>
// std::weak_ptr<Graphics_Context> UI_Handler<Main, Overlay>::getGraphicsContext(){
//     return this->context;
// }

template <class Main, class Overlay>
std::shared_ptr<Layout> UI_Handler<Main, Overlay>::getLayout() {
    return this->layout;
}

template <class Main, class Overlay>
std::shared_ptr<Animation_Handler> UI_Handler<Main, Overlay>::getAnimationHandler() {
    return this->animation_handler;
}

template <class Main, class Overlay>
std::shared_ptr<Event_Handler> UI_Handler<Main, Overlay>::getEventHandler(){
    return this->event_handler;
}

template <class Main, class Overlay>
std::shared_ptr<Graphics_Context> UI_Handler<Main, Overlay>::getGraphicsContext(){
    return this->context;
}

template <class Main, class Overlay>
void UI_Handler<Main, Overlay>::registerCustomMainInputHandler(std::function<void(Event, std::shared_ptr<Graphics_Context>, std::shared_ptr<Animation_Handler>, std::shared_ptr<Main>, std::shared_ptr<Overlay>)> customMainInputHandler) {
    this->customMainInputHandler = customMainInputHandler;
}

template <class Main, class Overlay>
void UI_Handler<Main, Overlay>::registerCustomOverlayInputHandler(std::function<void(Event, std::shared_ptr<Graphics_Context>, std::shared_ptr<Animation_Handler>, std::shared_ptr<Main>, std::shared_ptr<Overlay>)> customOverlayInputHandler) {
    this->customOverlayInputHandler = customOverlayInputHandler;
}

template <class Main, class Overlay>
void UI_Handler<Main, Overlay>::update() {
    while(!update_requests.empty()) {
        if(update_requests.front() == UPDATE_REQ::RESIZE) {
            SDL_Rect rect {0, 0, context->getWidth(), context->getHeight()};
            layout->updatePosition(rect);
        }
        update_requests.pop_front();
    }

    layout->update();
    animation_handler->update();
}
template <class Main, class Overlay>
void UI_Handler<Main, Overlay>::updateRequest(UPDATE_REQ request) {
    update_requests.push_back(request);
}

template <class Main, class Overlay>
bool UI_Handler<Main, Overlay>::main() {
    std::cout << "Starting main loop...\n";
    done = false;

    // implement layout update queue: events in the event_handler register an update request
    // (e.g. a resize update request) which gets fulfilled when layout->update() is called before normal execution
    //  moreover, have update return a boolean flagging whether anything has been updated and thus a re-render is necessary
    // if nothing has been updated, no need to re-render
    // std::thread render_thread = std::thread([handler{this}]() {
    //     // double run = 0;
    //     double temp = 0;
    //     while(!handler->done) {
    //         temp = 0;
    //         handler->update();

    //         temp = rLoop.elapsed();
    //         handler->context->clear();
    //         handler->layout->render();
    //         temp = rLoop.elapsed() - temp;
    //         std::cout << "RENDER: " << temp << "\n";

    //         handler->context->display();
    //         temp = rLoop.elapsed() - temp;
    //         std::cout << "DISPLAY: " << temp << "\n";
            
    //         // std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000*(1.0/handler->refresh_rate - run))));
    //         std::cout << " - ######################################################################### - \n\n";
    //         rLoop.reset();
    //     }
    // });
    double temp = 0;
    Event event;
    Timer rLoop;
    while(!done) {
        update();

        temp = 0;
        rLoop.reset();
        context->clear();
        layout->render();
        temp = rLoop.elapsed();
        // std::cout << "RENDER: " << temp << "\n";

        context->display();
        temp = rLoop.elapsed() - temp;
        // std::cout << "DISPLAY: " << temp << "\n";
        
        // // std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000*(1.0/handler->refresh_rate - run))));
        // std::cout << " - ######################################################################### - \n\n";

        event = event_handler->pollEvent();
        switch(event.type) {
            case(EVENT_TYPES::NO_EVENT) : continue;
            case (EVENT_TYPES::QUIT) :
                done = true;
                break; // breaking from the switch statement
            case (EVENT_TYPES::RESIZE) :
                updateRequest(UPDATE_REQ::RESIZE); // thread safe
                continue;
        }

        if(animation_handler->pending()) continue;
        
        if(event_handler->main_input_handler->polling()) {
            event = event_handler->main_input_handler->pollEvent();
            customMainInputHandler(event, this->context, this->animation_handler, this->main_target, this->overlay);
        }

        if(event_handler->overlay_input_handler->polling()) {
            event = event_handler->overlay_input_handler->pollEvent();
            customOverlayInputHandler(event, this->context, this->animation_handler, this->main_target, this->overlay);
        }
    }
    // if(render_thread.joinable()) render_thread.join();
    return true;
}
// bool UI_Handler::main_async() {
//     render_thread = std::thread([&done, &overlay_done]() mutable {

//     });
// }
// void UI_Handler::pause() {
//     pause = true;
// }

// bool UI_Handler::quit() {
//     done = true;
//     if(render_thread.joinable()) render_thread.join();
//     if(event_thread.joinable()) event_thread.join();
// }

#endif