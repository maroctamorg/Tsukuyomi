#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "extern.hpp"

enum GAME_STATE
{
    NOINPUT,
    UPDATE,
    TERMINATE,
    ANIMATION,
};

class UI_Element {
protected:
    std::shared_ptr<GraphicsContext> context { nullptr };

    bool hidden { false };
    bool shadow { true };
    int r { 0 };
    SDL_Texture* texture { nullptr };
    SDL_Rect rect {0, 0, 0, 0};
    SDL_Colour color {0, 0, 0, 0};
    SDL_Colour border_color {0, 0, 0, 0};

public:
    UI_Element(const std::shared_ptr<GraphicsContext> graphics_context, SDL_Rect rect, SDL_Texture* a_texture = nullptr, bool hidden = false, int r = 0)
        : context(graphics_context), rect(rect), texture(a_texture) {
            a_texture = nullptr;
        }
    // UI_Element(const std::shared_ptr<GraphicsContext> graphics_context, int x = 0, int y = 0, int w = 0, int h = 0, SDL_Colour color = SDL_Colour({0, 0, 0, 0}), SDL_Colour border_color = SDL_Colour({0, 0, 0, 0}), bool hidden = false, int r = 0)
    //     : context(graphics_context), rect{x, y, w, h}, color {color.r, color.g, color.b, color.a} {}
    UI_Element(const std::shared_ptr<GraphicsContext> graphics_context, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}), SDL_Colour color = SDL_Colour({0, 0, 0, 0}), SDL_Colour border_color = SDL_Colour({0, 0, 0, 0}), bool hidden = false, int r = 0)
        : context(graphics_context), rect{rect.x, rect.y, rect.w, rect.h}, color {color.r, color.g, color.b, color.a} {}
    virtual ~UI_Element()  {
        if(texture)
            SDL_DestroyTexture(texture);
        texture = nullptr;
    };

public:
    virtual void render() = 0;
    virtual void update() {};
    virtual void updateSize() = 0;
    virtual void updatePosition(const SDL_Rect& rect);

    void setHidden(bool hidden);
    void setCurveRadius(int r);
    void setSpace(int x, int y, int w, int h);
    void setSpace(const SDL_Rect& rect);
    void setColor(const SDL_Colour &color);
    void setBorderColor(const SDL_Colour &color);
    
    SDL_Rect getPosition();
};

#endif