#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "extern_ui.hpp"

enum GAME_STATE
{
    NOINPUT,
    UPDATE,
    TERMINATE,
    ANIMATION,
};

class UI_Element {
protected:
    std::shared_ptr<Graphics_Context> context { nullptr };

    bool hidden { false };
    bool shadow { true };
    int r { 0 };
    std::string texture_path;
    SDL_Texture* texture {nullptr};
    SDL_Rect rect {0, 0, 0, 0};
    SDL_Colour color {0, 0, 0, 0};
    SDL_Colour border_color {0, 0, 0, 0};

public:
    UI_Element(const std::shared_ptr<Graphics_Context> graphics_context, SDL_Rect rect, std::string a_texture, bool hidden = false, int r = 0)
        : context(graphics_context), rect(rect), texture_path(a_texture), hidden(hidden), r(r) {
            texture = loadTexture(context->renderer, texture_path);
            if(!texture) std::cout << "Texture could not be loaded...\n";
        }
    // UI_Element(const std::shared_ptr<Graphics_Context> graphics_context, int x = 0, int y = 0, int w = 0, int h = 0, SDL_Colour color = SDL_Colour({0, 0, 0, 0}), SDL_Colour border_color = SDL_Colour({0, 0, 0, 0}), bool hidden = false, int r = 0)
    //     : context(graphics_context), rect{x, y, w, h}, color {color.r, color.g, color.b, color.a} {}
    UI_Element(const std::shared_ptr<Graphics_Context> graphics_context, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}), SDL_Colour color = SDL_Colour({0, 0, 0, 0}), SDL_Colour border_color = SDL_Colour({0, 0, 0, 0}), bool hidden = false, int r = 0)
        : context(graphics_context), rect(rect), color(color), border_color(border_color), hidden(hidden), r(r) {}
    virtual ~UI_Element()  {
        if(texture)
            SDL_DestroyTexture(texture);
        texture = nullptr;
    };

public:
    virtual void render();          // why was this initially purely virtual?
    virtual void update() {};
    virtual void updateSize();
    virtual void updatePosition(const SDL_Rect& rect);

    void setHidden(bool hidden);
    void setCurveRadius(int r);
    void setSpace(int x, int y, int w, int h);
    void setSpace(const SDL_Rect& rect);
    void setColor(const SDL_Colour &color);
    void setBorderColor(const SDL_Colour &color);
    
    bool getHidden();
    int getCurveRadius();
    SDL_Rect getSpace();
    SDL_Color getColor();
    SDL_Color getBorderColor();
    
    SDL_Point getPosition();
    void setPosition(int x, int y);
};

#endif