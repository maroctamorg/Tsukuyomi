#ifndef UI_EVENT_HPP
#define UI_EVENT_HPP

#include "input_handler.hpp"

class Event_Handler {
public:
    std::weak_ptr<Input_Handler> main_input_handler;
    std::weak_ptr<Input_Handler> overlay_input_handler;
private:
    SDL_Event event;
    std::vector<std::function<void(SDL_Keycode)>> key_callbacks;

public:
    void registerMainInputHandler(std::weak_ptr<Input_Handler> input_handler);
    void registerOverlayInputHandler(std::weak_ptr<Input_Handler> input_handler);

    void registerKeyCallback(std::function<void(SDL_Keycode)> callback);
    //the enum passed to the callback is 'event.key.keysym.sym', type <SDL_Keycode>

    bool pollEvent();
};

#endif