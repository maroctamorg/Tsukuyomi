#include "menu.hpp"
#include "event.hpp"

// int Menu::update() {
//     if(layout)
//         layout->update();
//     return handler->PollEvent();
// };

void Menu::updateSize() {
    SDL_Rect w_rect{0, 0, this->context->getWidth(), this->context->getHeight()};
    layout->updatePosition(w_rect);
    // layout->updateSize();
}

// void addUI_Element(UI_Element&& element, int position, X_POS x_pos, Y_POS y_pos) {
//     layout->add(std::move<element>, position, x_pos, y_pos);
// }

void Menu::render() {
    if(b_texture)
        //YET TO BE IMPLEMENTED RENDER_TEXTURE
        std::cout << "!!!!!!!!!!!!!!!!\tCall to yet to be implemented render background texture in render Menu\t!!!!!!!!!!!!!!!!\n";
        // SDL_RenderTexture(context->renderer, b_texture);
    else if(b_color.a != 0) {
        SDL_SetRenderDrawColor(context->renderer, b_color.r, b_color.g, b_color.b, b_color.a);
        SDL_RenderClear(context->renderer);
    }
    //initialiser list order determines relative foreground of elements
    if(layout) {
        layout->render();
    }
};