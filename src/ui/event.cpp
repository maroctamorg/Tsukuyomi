#include "event.hpp"

// void Event_Handler::registerMainInputHandler(std::shared_ptr<Input_Handler> input_handler) {
//     main_input_handler = input_handler;
// }

// void Event_Handler::registerOverlayInputHandler(std::shared_ptr<Input_Handler> input_handler) {
//     overlay_input_handler = input_handler;
// }

void Event_Handler::registerKeyCallback(std::function<void(SDL_Keycode)> callback) {
    key_callbacks.push_back(callback);
}

Event_t Event_Handler::pollEvent() {
    SDL_Point cursor_pos {-100, -100};
    if(!SDL_PollEvent(&(event))) return Event_t({EVENT_TYPES::NO_EVENT, -1, NULL});
    switch(event.type) {
        case (SDL_QUIT) : return Event_t({EVENT_TYPES::QUIT, -1, NULL});
        case (SDL_WINDOWEVENT): {
            if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // must take in a graphics context + must implement follow through of updateSize()
                g_context->updateWindowSize(event.window.data1, event.window.data2);
                return Event_t({EVENT_TYPES::RESIZE, -1, NULL});
                // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
            }
            else return Event_t({EVENT_TYPES::UNHANDLED_SDL_EVENT, -1, NULL});
        }
        case (SDL_MOUSEBUTTONDOWN) : {
            // std::cout << "SDL_MOUSEBUTTONDOWN event registered.\n";
            SDL_GetMouseState(&(cursor_pos.x), &(cursor_pos.y));
            if(main_input_handler->polling()) main_input_handler->mouseClick(cursor_pos);
            if(overlay_input_handler->polling()) overlay_input_handler->mouseClick(cursor_pos);
            return Event_t({EVENT_TYPES::CLICK, -1, NULL});
        }
        case SDL_KEYDOWN : {
            for(auto key_callbacks_it = key_callbacks.cbegin(); key_callbacks_it != key_callbacks.cend(); key_callbacks_it++)
                (*key_callbacks_it)(event.key.keysym.sym);
            if(main_input_handler->polling()) main_input_handler->keyDown(event.key.keysym.sym);
            if(overlay_input_handler->polling()) overlay_input_handler->keyDown(event.key.keysym.sym);
            return Event_t({EVENT_TYPES::KEY_DOWN, -1, NULL});
        }
        case SDL_TEXTINPUT : {
            if(main_input_handler->polling()) main_input_handler->textInput(SDL_GetModState(), *event.text.text);
            if(overlay_input_handler->polling()) overlay_input_handler->textInput(SDL_GetModState(), *event.text.text);
            return Event_t({EVENT_TYPES::CHAR_INPUT, -1, NULL});
        }
        default: return Event_t({EVENT_TYPES::UNHANDLED_SDL_EVENT, -1, NULL});
    }
}

// std::shared_ptr<Input_Handler> Event_Handler::getMainInputHandler() {
//     return main_input_handler;
// }
// std::shared_ptr<Input_Handler> Event_Handler::getOverlayInputHandler() {
//     return overlay_input_handler;
// }

Event_Handler::Event_Handler(std::shared_ptr<Input_Handler> main, std::shared_ptr<Input_Handler> overlay, std::shared_ptr<Graphics_Context> context)
    : main_input_handler{main}, overlay_input_handler{overlay}, g_context {context} {}

Event_Handler::Event_Handler(std::shared_ptr<Graphics_Context> context)
    : g_context {context} {
        main_input_handler = std::make_shared<Input_Handler>();
        overlay_input_handler = std::make_shared<Input_Handler>();
    }