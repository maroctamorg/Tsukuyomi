#include "event.hpp"

// void Event_Handler::registerMainInputHandler(std::shared_ptr<Input_Handler> input_handler) {
//     main_input_handler = input_handler;
// }

// void Event_Handler::registerOverlayInputHandler(std::shared_ptr<Input_Handler> input_handler) {
//     overlay_input_handler = input_handler;
// }

void Event_Handler::registerSDLEventCallback(Uint32 sdl_event_type, std::function<DEFEvent()> callback) {
    sdl_event_callbacks.insert(std::pair<Uint32, std::function<DEFEvent()>>(sdl_event_type, callback));
}
void Event_Handler::registerButtonEventCallback(int button_id, std::function<DEFEvent()> callback) {
    button_event_callbacks.insert(std::pair<int, std::function<DEFEvent()>>(button_id, callback));
}

void Event_Handler::registerKeyCallback(SDL_Keycode key, std::function<DEFEvent()> callback) {
    key_callbacks.insert(std::pair<SDL_Keycode, std::function<DEFEvent()>>(key, callback));
}

void Event_Handler::deRegisterSDLEventCallback(Uint32 sdl_event_type) {
    sdl_event_callbacks.erase(sdl_event_type);
}
void Event_Handler::deRegisterButtonEventCallback(int button_id) {
    button_event_callbacks.erase(button_id);
}

void Event_Handler::deRegisterKeyCallback(SDL_Keycode key) {
    key_callbacks.erase(key);
}

DEFEvent Event_Handler::pollEvent() {
    SDL_Point cursor_pos {-100, -100};
    if(!SDL_PollEvent(&(event))) return DEFEvent({EVENT_TYPES::NO_EVENT, -1, NULL});
    for(auto it = sdl_event_callbacks.cbegin(); it != sdl_event_callbacks.cend(); it++) {
        if(event.type != it->first) continue;
        return it->second();
    }
    switch(event.type) {
        case (SDL_QUIT) : return DEFEvent({EVENT_TYPES::QUIT, -1, NULL});
        case (SDL_WINDOWEVENT): {
            if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // must take in a graphics context + must implement follow through of updateSize()
                g_context->updateWindowSize(event.window.data1, event.window.data2);
                return DEFEvent({EVENT_TYPES::RESIZE, -1, NULL});
                // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
            }
            else return DEFEvent({EVENT_TYPES::UNHANDLED_SDL_EVENT, -1, NULL});
        }
        case (SDL_MOUSEBUTTONUP) : { //separate into MOUSEBUTTONDOWN/UP/CLICK/HOLD, etc...
            // std::cout << "SDL_MOUSEBUTTONDOWN event registered.\n";
            SDL_GetMouseState(&(cursor_pos.x), &(cursor_pos.y));
            if(main_input_handler->polling()) main_input_handler->mouseClick(cursor_pos);
            if(overlay_input_handler->polling()) overlay_input_handler->mouseClick(cursor_pos);
            DEFEvent custom_event{EVENT_TYPES::CLICK, -1, NULL};
            if(main_input_handler->polling()) custom_event = main_input_handler->pollEvent();
            if( (custom_event.type == EVENT_TYPES::CLICK || custom_event.type == EVENT_TYPES::NO_EVENT) && overlay_input_handler->polling()) custom_event = overlay_input_handler->pollEvent();
            for(auto it = button_event_callbacks.cbegin(); it != button_event_callbacks.cend(); it++) {
                if(it->first == custom_event.button_id) return it->second();
            }
            return custom_event;
        }
        case SDL_KEYDOWN : {
            for(auto key_callbacks_it = key_callbacks.cbegin(); key_callbacks_it != key_callbacks.cend(); key_callbacks_it++)
                if(key_callbacks_it->first == event.key.keysym.sym) return key_callbacks_it->second();
            if(main_input_handler->polling()) main_input_handler->keyDown(event.key.keysym.sym);
            if(overlay_input_handler->polling()) overlay_input_handler->keyDown(event.key.keysym.sym);
            return DEFEvent({EVENT_TYPES::KEY_DOWN, -1, NULL});
        }
        case SDL_TEXTINPUT : {
            if(main_input_handler->polling()) main_input_handler->textInput(SDL_GetModState(), *event.text.text);
            if(overlay_input_handler->polling()) overlay_input_handler->textInput(SDL_GetModState(), *event.text.text);
            return DEFEvent({EVENT_TYPES::CHAR_INPUT, -1, NULL});
        }
        default: {
            return DEFEvent({EVENT_TYPES::UNHANDLED_SDL_EVENT, -1, NULL});
        }
    }
}

Event_Handler::Event_Handler(std::shared_ptr<Input_Handler> main, std::shared_ptr<Input_Handler> overlay, std::shared_ptr<Graphics_Context> context)
    : main_input_handler{main}, overlay_input_handler{overlay}, g_context {context} {}

Event_Handler::Event_Handler(std::shared_ptr<Graphics_Context> context)
    : g_context {context} {
        main_input_handler = std::make_shared<Input_Handler>();
        overlay_input_handler = std::make_shared<Input_Handler>();
    }