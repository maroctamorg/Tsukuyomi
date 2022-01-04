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

class UI_Handler {
protected:
    bool done {true};
    int refresh_rate;

    Graphics_Context            context;
    Event_Handler               event_handler;
    Animation_Handler           animation_handler;
    std::shared_ptr<UI_Element> main_target;
    std::shared_ptr<UI_Element> overlay;

    std::function<void(Event, Graphics_Context&, Animation_Handler&, std::shared_ptr<UI_Element>, std::shared_ptr<UI_Element>)> customMainInputHandler;
    std::function<void(Event, Graphics_Context&, Animation_Handler&, std::shared_ptr<UI_Element>, std::shared_ptr<UI_Element>)> customOverlayInputHandler;
public:
    UI_Handler(int W_H, int W_W, int refresh_rate = 120);
    ~UI_Handler();
public:
    void setMain(std::shared_ptr<UI_Element> main_target);
    void setOverlay(std::shared_ptr<UI_Element> overlay);
    std::weak_ptr<Animation_Handler> getAnimationHandler();

    void registerCustomMainInputHandler(std::function<void(Event, Graphics_Context&, Animation_Handler&, std::shared_ptr<UI_Element>, std::shared_ptr<UI_Element>)> customMainInputHandler);
    void registerCustomOverlayInputHandler(std::function<void(Event, Graphics_Context&, Animation_Handler&, std::shared_ptr<UI_Element>, std::shared_ptr<UI_Element>)> customOverlayInputHandler);
public:
    virtual bool main();
    // virtual bool main_async();
    // void pause();
    // virtual bool quit();
};

#endif