#ifndef BUTTON_H
#define BUTTON_H

#include "extern.hpp"
#include "text.cpp"

typedef struct
{
    bool active, pressed;
} state;

class Button
{
private:
    std::unique_ptr<Layout> layout { nullptr };
    state b_state { false };
    int b_id { -1 };
    bool initialized {false};

public:
    int generateBttTexture(char *imgPath, SDL_Renderer *renderer);
    void destroyTexture();

public:
    Button() = default;
    Button(char* imgPath, std::string text, std::string font, int ptsize, const SDL_Color *font_color, const SDL_Rect *target_rect, const SDL_Rect *rect, const SDL_Color *color, bool is_active, int id, SDL_Renderer *renderer);
    ~Button();

public:
    void assign(char* imgPath, std::string text, std::string font, int ptsize, const SDL_Color *font_color, const SDL_Rect *target_rect, const SDL_Rect *rect, const SDL_Color *color, bool is_active, int id, SDL_Renderer *renderer);
    void activate();
    Button* press();
    void deactivate();
    bool isActive();
    bool isPressed();
    bool Clicked(const SDL_Point &cursor_pos);
    void display(SDL_Renderer *renderer);


};

#endif
