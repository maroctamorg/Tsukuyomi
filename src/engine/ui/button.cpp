#include "button.hpp"
#include "event.hpp"

Button::Button(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::shared_ptr<Layout> layout, const unsigned int id, bool active, bool select, SDL_Color color, SDL_Rect rect)
    :   UI_Element(context, rect, color), handler(handler), layout(layout), id { id }, state { active, select, false, false } {
        // std::cout << "Button state on initialization:\nactive: " << state.active << "\tselect: " << state.select << '\n';
        if(this->layout)
            this->layout->updatePosition(this->rect);
        if(this->handler)
            this->handler->registerButtonToHandler(this);
    }
Button::Button(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::shared_ptr<Layout> layout, const unsigned int id, bool active, bool select, SDL_Rect rect, SDL_Texture* a_texture)
    :   UI_Element(context, rect, a_texture), handler(handler), layout(layout), id { id }, state { active, select, false, false } {
        if(this->layout)
            this->layout->updatePosition(this->rect);
        if(this->handler)
            this->handler->registerButtonToHandler(this);
    }

int Button::getId() { return id; }
void Button::registerCallBack(std::function<void(const GraphicsContext&, const EventHandler&, Button&)> callback) {
    this->callback = callback;
}
void Button::activate()     {   state.active    =   true;   }
void Button::select()       {   state.select    =   true;   }
Button* Button::press() {
    // std::cout << "Call to button press!\n";
    if(state.select)
        state.selected = true; // handler->addButtonToSelected(this);
    state.pressed = true;
    // handler->addButtonToPressed(this);
    callback(*context, *handler, *this);
    //implement button press animation
    return this;
}
void Button::deactivate()   {   state.active  =   false;  }
void Button::dettachHandler() {
    if(this->handler) {
        this->handler = nullptr;
    }
}

bool Button::isActive()     {   return state.active;    }
bool Button::isPressed()    {   return state.pressed;   }
bool Button::isSelected()   {   return state.selected;  }

bool Button::Clicked(const SDL_Point &cursor_pos) {
    // std::cout << "Call to clicked:\nrect ->\tx: " << rect.x << "\ty: " << rect.y << "\tw: " << rect.w << "\th: " << rect.h << '\n';
    return isContained(cursor_pos, this->rect);
}

void Button::update() {
    if(state.pressed)
        this->state.pressed = false;
    if(layout)
        layout->update();
}

void Button::updatePosition (const SDL_Rect& rect) {
    this->UI_Element::updatePosition(rect);
    if(layout) {
        layout->updatePosition(rect);
        // layout->updateSize();
    }
}

void Button::updateSize () {
    if(layout)
        layout->updateSize();
}

void Button::render() {
    this->UI_Element::render();
    if(layout)
        layout->render();
}

Button::~Button() {
    if(this->handler)
        this->handler->removeButtonFromHandler(this->id);
}