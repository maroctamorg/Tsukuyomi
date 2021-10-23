#include "pannel.hpp"

void Pannel::render () {
    // std::cout << "############\tCall to pannel.render()!\t############\n";
    this->UI_Element::render();
    if(layout)
        layout->render();
}

void Pannel::update () {
    if(layout)
        layout->update();
}

void Pannel::updatePosition (const SDL_Rect& rect) {
    this->UI_Element::updatePosition(rect);
    if(layout) {
        layout->updatePosition(rect);
        // layout->updateSize();
    }
}

void Pannel::updateSize () {
    if(layout)
        layout->updateSize();
}