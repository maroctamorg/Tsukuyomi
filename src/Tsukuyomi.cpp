#include "Tsukuyomi.hpp"

UI_Handler::UI_Handler(int W_H, int W_W, int refresh_rate)
        :   refresh_rate{refresh_rate}, context(W_H, W_W) {
        }
UI_Handler::~UI_Handler() {
    // quit();
}

void UI_Handler::setMain(std::shared_ptr<UI_Element> main_target) {
    // quit();
    this->main_target = main_target;
    // if(!done) main();
}
void UI_Handler::setOverlay(std::shared_ptr<UI_Element> overlay) {
    // quit();
    this->overlay = overlay;
    // if(!done) main();
}
std::weak_ptr<Animation_Handler> UI_Handler::getAnimationHandler() {
    return std::make_shared<Animation_Handler>(this->animation_handler);
}
void UI_Handler::registerCustomMainInputHandler(std::function<void(Event, Graphics_Context&, Animation_Handler&, std::shared_ptr<UI_Element>, std::shared_ptr<UI_Element>)> customMainInputHandler) {
    this->customMainInputHandler = customMainInputHandler;
}
void UI_Handler::registerCustomOverlayInputHandler(std::function<void(Event, Graphics_Context&, Animation_Handler&, std::shared_ptr<UI_Element>, std::shared_ptr<UI_Element>)> customOverlayInputHandler) {
    this->customOverlayInputHandler = customOverlayInputHandler;
}

bool UI_Handler::main() {
    done = false;
    // std::weak_ptr<UI_Element> main_wk(main_target);
    // std::weak_ptr<UI_Element> overlay_wk(overlay_target);
    UI_Handler* handler = this;
    std::thread render_thread = std::thread([handler]() mutable {
        if(!this->main_target.get()) return false;
        Timer rLoop;
        Event event;
        while(!done) {
            // if(auto main = main_wk.lock()) main.render();
            // if(auto overlay = overlay_wk.lock()) overlay.render();
            // if(auto main = main_wk.lock()) main.present();
            // if(auto overlay = overlay_wk.lock()) overlay.present();
            handler->animation_handler.update();
            handler->main_target->render();
            if(handler->overlay.get()) handler->overlay->render();
            SDL_RenderPresent(handler->context.renderer);
            handler->main_target->update();
            if(handler->overlay.get()) handler->overlay->update();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000*(1.0/this->refresh_rate - rLoop.elapsed()))));
        // reset timer
        rLoop.reset();
    });
    handler = nullptr;
    while(!done) {
        if(!event_handler.pollEvent() || animation_handler.pending()) continue;
        if(auto input_handler = event_handler.main_input_handler.lock()) {
            if(input_handler->polling())
                customMainInputHandler(input_handler->pollEvent(), this->context, this->animation_handler, this->main_target, this->overlay);
        }
        if(auto input_handler = event_handler.overlay_input_handler.lock()) {
            if(input_handler->polling())
                customOverlayInputHandler(input_handler->pollEvent(), this->context, this->animation_handler, this->main_target, this->overlay);
        }
    }
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