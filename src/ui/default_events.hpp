#ifndef DEFAULT_EVENTS_HPP
#define DEFAULT_EVENTS_HPP

enum class EVENT_TYPES {
    UNHANDLED_SDL_EVENT = -2,
    QUIT = -1,
    NO_EVENT = 0,
    CLICK = 1,
    BUTTON_PRESS = 2,
    KEY_DOWN = 3,
    CHAR_INPUT = 4,
    RESIZE = 5,
    NEXT = 6,
    CUSTOM_EVENT = 7,
};

struct DEFEvent{
    EVENT_TYPES type { EVENT_TYPES::NO_EVENT };
    int button_id { -1 };
    char char_input;
};

#endif