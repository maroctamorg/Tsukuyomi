#include "event.hpp"

void Event_Handler::registerMainInputHandler(std::weak_ptr<Input_Handler> input_handler) {
    main_input_handler = input_handler;
}

void Event_Handler::registerOverlayInputHandler(std::weak_ptr<Input_Handler> input_handler) {
    overlay_input_handler = input_handler;
}

void Event_Handler::registerKeyCallback(std::function<void(SDL_Keycode)> callback) {
    key_callbacks.push_back(callback);
}

bool Event_Handler::pollEvent() {
    SDL_Point cursor_pos {-100, -100};
    if(!SDL_PollEvent(&(event))) return false;
    switch(event.type) {
        case (SDL_MOUSEBUTTONDOWN) :
            // std::cout << "SDL_MOUSEBUTTONDOWN event registered.\n";
            SDL_GetMouseState(&(cursor_pos.x), &(cursor_pos.y));
            if(auto input_handler = main_input_handler.lock())
                if(input_handler->polling()) input_handler->mouseClick(cursor_pos);
            if(auto input_handler = overlay_input_handler.lock())
                if(input_handler->polling()) input_handler->mouseClick(cursor_pos);
            break;
        case SDL_KEYDOWN:
            for(auto key_callbacks_it = key_callbacks.cbegin(); key_callbacks_it != key_callbacks.cend(); key_callbacks_it++)
                (*key_callbacks_it)(event.key.keysym.sym);
            if(auto input_handler = main_input_handler.lock())
                if(input_handler->polling()) input_handler->keyDown(event.key.keysym.sym);
            if(auto input_handler = overlay_input_handler.lock())
                if(input_handler->polling()) input_handler->keyDown(event.key.keysym.sym);
            break;
        case SDL_TEXTINPUT :
            if(auto input_handler = main_input_handler.lock())
                if(input_handler->polling()) input_handler->textInput(SDL_GetModState(), *event.text.text);
            if(auto input_handler = overlay_input_handler.lock())
                if(input_handler->polling()) input_handler->textInput(SDL_GetModState(), *event.text.text);
            break;
    }
    return true;
}