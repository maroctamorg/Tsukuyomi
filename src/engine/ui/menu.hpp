#ifndef MENU_HPP
#define MENU_HPP

#include "extern.hpp"

class Layout;
class EventHandler;

class Menu final {
private:
    std::shared_ptr<GraphicsContext> context { nullptr };
    std::shared_ptr<EventHandler> handler { nullptr };
    std::shared_ptr<Layout> layout { nullptr };

    SDL_Texture* b_texture { nullptr };
    SDL_Color b_color { 0, 0, 0, 0 };

public:
    // should be done with move semantics to transfer over ownership of UI_Elements
    Menu(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::unique_ptr<Layout> layout, SDL_Texture* texture)
        :   context(context), handler(handler), layout(std::move(layout)), b_texture {texture} {}
    Menu(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::unique_ptr<Layout> layout, SDL_Color color)
        :   context(context), handler(handler), layout(std::move(layout)), b_color {color} {}

    Menu(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::shared_ptr<Layout> layout, SDL_Texture* texture)
        :   context(context), handler(handler), layout(layout), b_texture {texture} {}
    Menu(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::shared_ptr<Layout> layout, SDL_Color color)
        :   context(context), handler(handler), layout(layout), b_color {color} {}

    Menu(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, Layout* layout, SDL_Color color)
        :   context(context), handler(handler), layout{layout}, b_color {color} {}

    ~Menu() {
        if(b_texture) {
            SDL_DestroyTexture(b_texture);
            b_texture = nullptr;
        }
        // SETTING THE CONTEXT POINTER TO NULLPTR SO THAT THE OBJECT IS NOT DESTRUCTED
        // context = nullptr;
        // No longer needed since we are using shared_ptr
    }

public:
    // int update();
    void updateSize();

    // void addUI_Element(UI_Element&& element, int position, X_POS x_pos, Y_POS y_pos) {
    //     layout->add(std::move<element>, position, x_pos, y_pos);
    // }

    void render();

};

#endif