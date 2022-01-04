#include "ui_element.hpp"

void UI_Element::setHidden(bool hidden) { this->hidden = hidden; }
void UI_Element::setCurveRadius(int r) { this->r = r; }
void UI_Element::setSpace(int x, int y, int w, int h) { this->rect = SDL_Rect({x, y, w, h}); }
void UI_Element::setSpace(const SDL_Rect& rect) { this->rect = rect; }
void UI_Element::setColor(const SDL_Colour &color) { this->color = color; }
void UI_Element::setBorderColor(const SDL_Colour &color) { this->border_color = color; }

bool UI_Element::getHidden()        { return this->hidden;          }
int UI_Element::getCurveRadius()   { return this->r;               }
SDL_Rect UI_Element::getSpace()         { return this->rect;            }
SDL_Color UI_Element::getColor()         { return this->color;           }
SDL_Color UI_Element::getBorderColor()   { return this->border_color;    }

void UI_Element::updatePosition(const SDL_Rect& rect) {
    this->rect.x = rect.x;
    this->rect.y = rect.y;
    this->rect.w = rect.w;
    this->rect.h = rect.h;
}

SDL_Rect UI_Element::getPosition() { return rect; }

void UI_Element::render() {
    if (hidden) return;
    if(texture) {
        // std::cout << "Call to yet to be implemented render texture in UI_Element render!\n";
        // TO BE IMPLEMENTED!
        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect src_rect   {0, 0, texW, texH};

        SDL_RenderCopy(context->renderer, texture, &src_rect, &rect);
    } else if(color.a != 0) {
        // if(r < 1) {
        //     SDL_SetRenderDrawColor(context->renderer, color.r, color.g, color.b, color.a);
        //     SDL_RenderFillRect(context->renderer, &(this->rect));

        //     if(border_color.a != 0) {
        //         SDL_SetRenderDrawColor(context->renderer, border_color.r, border_color.g, border_color.b, border_color.a);
        //         SDL_RenderDrawRect(context->renderer, &(this->rect));
        //     }

        //     return;
        // }

        drawRoundedRect(context->renderer, this->rect, this->color, this->border_color, 5, true);

    }
}

void UI_Element::updateSize() {
    rect.w = this->context->getWidth();
    rect.h = this->context->getHeight();
}