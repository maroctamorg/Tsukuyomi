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
        case (SDL_QUIT) :
            if(auto input_handler = main_input_handler.lock())  input_handler->quit();
            if(auto input_handler = overlay_input_handler.lock())  input_handler->quit();
            break;
        case (SDL_WINDOWEVENT): {
            if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // must take in a graphics context + must implement follow through of updateSize()
                g_context->updateWindowSize(event.window.data1, event.window.data2);
                if(auto input_handler = main_input_handler.lock())   input_handler->resize();
                if(auto input_handler = overlay_input_handler.lock())   input_handler->resize();
                // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
            }
            break;
        }
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

Event_Handler::Event_Handler(std::weak_ptr<Input_Handler> main, std::weak_ptr<Input_Handler> overlay, std::shared_ptr<Graphics_Context> context)
    : main_input_handler{main}, overlay_input_handler{overlay}, g_context {context} {}