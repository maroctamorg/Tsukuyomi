#ifndef TEXT_GRAPHICS_H
#define TEXT_GRAPHICS_H

#include "graphics_engine.hpp"

class Text
{
private:

    std::string text;
    std::string font;
    int ptsize { 20 };
    SDL_Colour color { 0, 0, 0, 250 };
    SDL_Rect target_rect { 500, 500, 800, 500 };
    SDL_Colour rect_colour {250, 250, 250, 250};
    bool display_rect {false};
    bool initialized {false};

    SDL_Texture *txtTexture {nullptr};

public:
    Text() = default;
    Text(const std::string text, const std::string font, int ptsize, const SDL_Colour *color, const SDL_Rect *target_rect, const SDL_Colour *rect_colour, bool display_rect, SDL_Renderer *renderer);
    void assign(const std::string text, const std::string font, int ptsize, const SDL_Colour *color, const SDL_Rect *target_rect, const SDL_Colour *rect_colour, bool display_rect, SDL_Renderer *renderer);
    void assignTxt(const std::string text, SDL_Renderer *renderer);
    bool isInitialized();
    int generateTxtTexture(SDL_Renderer *renderer);
    void displayText(SDL_Renderer *renderer);
    void destroyTxtTexture();
    ~Text();

};

#endif