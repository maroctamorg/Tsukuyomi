#ifndef UI_EVENT_HPP
#define UI_EVENT_HPP

#include "input_handler.hpp"

class Event_Handler {
public:
    std::shared_ptr<Input_Handler> main_input_handler;
    std::shared_ptr<Input_Handler> overlay_input_handler;
    SDL_Event event;

private:
    std::shared_ptr<Graphics_Context> g_context;
    std::vector<std::function<void(SDL_Keycode)>> key_callbacks;

public:
    // friend int SDL_PollEvent(SDL_Event *event);

    // void registerMainInputHandler(std::shared_ptr<Input_Handler> input_handler);
    // void registerOverlayInputHandler(std::shared_ptr<Input_Handler> input_handler);

    void registerKeyCallback(std::function<void(SDL_Keycode)> callback);
    //the enum passed to the callback is 'event.key.keysym.sym', type <SDL_Keycode>

    Event pollEvent();

    // std::weak_ptr<Input_Handler> getMainInputHandler();
    // std::weak_ptr<Input_Handler> getOverlayInputHandler();

public:
    Event_Handler(std::shared_ptr<Input_Handler> main, std::shared_ptr<Input_Handler> overlay, std::shared_ptr<Graphics_Context> context);
    Event_Handler(std::shared_ptr<Graphics_Context> context);
    ~Event_Handler() = default;
};

#endif