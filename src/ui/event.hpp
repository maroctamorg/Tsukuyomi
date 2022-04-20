#ifndef UI_EVENT_HPP
#define UI_EVENT_HPP

#include "default_events.hpp"
#include "input_handler.hpp"

class Event_Handler {
public:
    // Event custom_event;
    std::shared_ptr<Input_Handler> main_input_handler;
    std::shared_ptr<Input_Handler> overlay_input_handler;
    SDL_Event event;

protected:
    std::shared_ptr<Graphics_Context> g_context;
    std::map<Uint32, std::function<DEFEvent()>> sdl_event_callbacks;
    std::map<int, std::function<DEFEvent()>> button_event_callbacks;
    std::map<SDL_Keycode, std::function<DEFEvent()>> key_callbacks;

public:
    // friend int SDL_PollEvent(SDL_Event *event);

    // void registerMainInputHandler(std::shared_ptr<Input_Handler> input_handler);
    // void registerOverlayInputHandler(std::shared_ptr<Input_Handler> input_handler);

    virtual void registerSDLEventCallback(Uint32 sdl_event_type, std::function<DEFEvent()> callback);
    virtual void registerButtonEventCallback(int button_id, std::function<DEFEvent()> callback);

    //the enum argument is 'event.key.keysym.sym', type <SDL_Keycode>
    virtual void registerKeyCallback(SDL_Keycode keycode, std::function<DEFEvent()> callback);

    virtual void deRegisterSDLEventCallback(Uint32 sdl_event_type);
    virtual void deRegisterButtonEventCallback(int button_id);

    //the enum argument is 'event.key.keysym.sym', type <SDL_Keycode>
    void deRegisterKeyCallback(SDL_Keycode keycode);

    // Event pollCustomEvent();
    virtual DEFEvent pollEvent();

    // std::weak_ptr<Input_Handler> getMainInputHandler();
    // std::weak_ptr<Input_Handler> getOverlayInputHandler();

public:
    Event_Handler(std::shared_ptr<Input_Handler> main, std::shared_ptr<Input_Handler> overlay, std::shared_ptr<Graphics_Context> context);
    Event_Handler(std::shared_ptr<Graphics_Context> context);
    virtual ~Event_Handler() = default;
};

template <typename Event>
class Custom_Event_Handler final : public Event_Handler {
static_assert(std::is_base_of<DEFEvent, Event>::value, "Custom event type must derive from DEFEvent");
public:
    std::shared_ptr<Custom_Input_Handler<Event>> main_input_handler;
    std::shared_ptr<Custom_Input_Handler<Event>> overlay_input_handler;

    std::map<SDL_Keycode, std::function<Event()>> key_callbacks;
    std::map<Uint32, std::function<Event()>> sdl_event_callbacks;
    std::map<int, std::function<Event()>> button_event_callbacks;

public:
    void registerSDLEventCallback(Uint32 sdl_event_type, std::function<Event()> callback) override;
    void registerButtonEventCallback(int button_id, std::function<Event()> callback) override;
    void registerKeyCallback(SDL_Keycode keycode, std::function<Event()> callback) override;

    void deRegisterSDLEventCallback(Uint32 sdl_event_type) override;
    void deRegisterButtonEventCallback(int button_id) override;

    Event pollEvent() override;

public:
    // Custom_Event_Handler(std::shared_ptr<Custom_Input_Handler<Event>> main, std::shared_ptr<Custom_Input_Handler<Event>> overlay, std::shared_ptr<Graphics_Context> context)
    //     : Event_Handler(std::shared_ptr<Input_Handler> main, std::shared_ptr<Input_Handler> overlay, std::shared_ptr<Graphics_Context> context) {}
    Custom_Event_Handler(std::shared_ptr<Graphics_Context> context)
        : Event_Handler(context) {}
        
    ~Custom_Event_Handler() = default;
};

template <typename Event>
void Custom_Event_Handler<Event>::registerSDLEventCallback(Uint32 sdl_event_type, std::function<Event()> callback) {
    sdl_event_callbacks.insert(std::pair<Uint32, std::function<Event()>>(sdl_event_type, callback));
}
template <typename Event>
void Custom_Event_Handler<Event>::registerButtonEventCallback(int button_id, std::function<Event()> callback) {
    button_event_callbacks.insert(std::pair<int, std::function<Event()>>(button_id, callback));
}
template <typename Event>
void Custom_Event_Handler<Event>::deRegisterSDLEventCallback(Uint32 sdl_event_type) {
    sdl_event_callbacks.erase(sdl_event_type);
}
template <typename Event>
void Custom_Event_Handler<Event>::deRegisterButtonEventCallback(int button_id) {
    button_event_callbacks.erase(button_id);
}

template <typename Event>
Event Custom_Event_Handler<Event>::pollEvent() {
    SDL_Point cursor_pos {-100, -100};
    if(!SDL_PollEvent(&(event))) return Event({EVENT_TYPES::NO_EVENT, -1, NULL});
    for(auto it = sdl_event_callbacks.cbegin(); it != sdl_event_callbacks.cend(); it++) {
        if(event.type != it->first) continue;
        return it->second();
    }
    switch(event.type) {
        case (SDL_QUIT) : return Event({EVENT_TYPES::QUIT, -1, NULL});
        case (SDL_WINDOWEVENT): {
            if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // must take in a graphics context + must implement follow through of updateSize()
                g_context->updateWindowSize(event.window.data1, event.window.data2);
                return Event({EVENT_TYPES::RESIZE, -1, NULL});
                // std::cout << "Resized window:\t" << event.window.data1 << ", " << event.window.data2 << "\t" << g_context.getWidth() << ", " << g_context.getHeight() << "\n";
            }
            else return Event({EVENT_TYPES::UNHANDLED_SDL_EVENT, -1, NULL});
        }
        case (SDL_MOUSEBUTTONUP) : { //separate into MOUSEBUTTONDOWN/UP/CLICK/HOLD, etc...
            // std::cout << "SDL_MOUSEBUTTONDOWN event registered.\n";
            SDL_GetMouseState(&(cursor_pos.x), &(cursor_pos.y));
            if(main_input_handler->polling()) main_input_handler->mouseClick(cursor_pos);
            if(overlay_input_handler->polling()) overlay_input_handler->mouseClick(cursor_pos);
            DEFEvent custom_event{EVENT_TYPES::CLICK, -1, NULL};
            if(main_input_handler->polling()) custom_event = main_input_handler->pollEvent();
            if(custom_event.type == EVENT_TYPES::NO_EVENT && overlay_input_handler->polling()) custom_event = overlay_input_handler->pollEvent();
            for(auto it = button_event_callbacks.cbegin(); it != button_event_callbacks.cend(); it++) {
                if(it->first == custom_event.button_id) return it->second();
            }
            return custom_event;
        }
        case SDL_KEYDOWN : {
            for(auto key_callbacks_it = key_callbacks.cbegin(); key_callbacks_it != key_callbacks.cend(); key_callbacks_it++)
                if (key_callbacks_it->first == event.key.keysym.sym) return key_callbacks_it->second();
            if(main_input_handler->polling()) main_input_handler->keyDown(event.key.keysym.sym);
            if(overlay_input_handler->polling()) overlay_input_handler->keyDown(event.key.keysym.sym);
            return Event({EVENT_TYPES::KEY_DOWN, -1, NULL});
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

#endif