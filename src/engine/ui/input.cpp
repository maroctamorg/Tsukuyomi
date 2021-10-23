#include "input.hpp"
#include "event.hpp"

InputField::InputField(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, const unsigned int id, const std::string font, int ptsize, const SDL_Colour font_color, SDL_Color color, ALIGN_X alignX, ALIGN_Y alignY, bool active, SDL_Rect rect)
    :   UI_Element(context, rect, color), handler(handler), id { id }, textbox { context, "", font, ptsize, font_color, alignX, alignY }, state { active, false } {
        if(this->handler)
            this->handler->registerInputFieldToHandler(this);
    }
InputField::InputField(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, const unsigned int id, const std::string font, int ptsize, const SDL_Colour font_color, SDL_Texture* a_texture, ALIGN_X alignX, ALIGN_Y alignY, bool active, SDL_Rect rect)
    :   UI_Element(context, rect, a_texture), handler(handler), id { id }, textbox { context, "", font, ptsize, font_color, alignX, alignY }, state { active, false } {
        if(this->handler)
            this->handler->registerInputFieldToHandler(this);
    }

int InputField::getId() { return id; }
std::string InputField::getText() { return this->textbox.getText(); }
// void InputField::registerCallBack(std::function<void(GraphicsContext*, EventHandler*, std::shared_ptr<InputField>)> callback) {
//     this->callback = callback;
// }
void InputField::activate()     {   state.active    =   true;   }
InputField* InputField::select() {
    // std::cout << "Call to input select!\n";
    if(!state.active)
        return this;
    state.selected = true;
    // callback(context.get(), handler.get(), this);
    return this;
}
InputField* InputField::deSelect() {
    // std::cout << "Call to input select!\n";
    if(!state.active)
        return this;
    state.selected = false;
    // callback(context.get(), handler.get(), this);
    return this;
}
void InputField::charIn(char* a) {
    int length = this->textbox.getText().length();
    int capacity = this->textbox.calculateCapacity();
    if(length >= capacity) {
        // std::cout << "Returning early from InputField::charIn, not enough space!\n\tlength: " << length << "\tcapacity: " << capacity << '\n';
        return;
    }
    this->textbox.append(*a);
    if(*a != '\n') {
        this->cursorPos++;
        // std::cout << "Moving up cursor!\n";
    }
}
void InputField::del() {
    if(this->textbox.del()) this->cursorPos--;
}
void InputField::deactivate()   {   state.active  =   false;  }
void InputField::dettachHandler() {
    if(this->handler) {
        this->handler = nullptr;
    }
}

bool InputField::isActive()     {   return state.active;    }
bool InputField::isSelected()   {   return state.selected;  }

bool InputField::Clicked(const SDL_Point &cursor_pos) {
    // std::cout << "Call to clicked:\nrect ->\tx: " << rect.x << "\ty: " << rect.y << "\tw: " << rect.w << "\th: " << rect.h << '\n';
    return isContained(cursor_pos, this->rect);
}

void InputField::updateText(std::string text) {
    this->textbox.updateText(text);
}

void InputField::update() {
    this->textbox.update();
}

void InputField::updatePosition (const SDL_Rect& rect) {
    this->UI_Element::updatePosition(rect);
    int w,h;
    this->textbox.getCharTextureSize(&w, &h);
    SDL_Rect text_rect { rect.x + w, rect.y + static_cast<int>(h/4), rect.w - 2*w, rect.h - static_cast<int>(h/2) };
    this->textbox.updatePosition(text_rect);
}

void InputField::updateSize () {
    this->textbox.updateSize();
}

void InputField::renderCursor() {
    int w, h;
    this->textbox.getCharTextureSize(&w, &h);
    int line {0}, counter{0}, index {0};
    if (this->textbox.numberOfLines() != 0) {
        while (counter <= cursorPos && index < this->textbox.numberOfLines()) {
            counter += this->textbox.getContentLengthFromLines(index);
            line++;
            index++;
        }
        line -= 1;
        counter -= this->textbox.getContentLengthFromLines(index - 1);
    }
    SDL_Point pos = this->textbox.getPos(line, this->cursorPos - counter);
    SDL_Rect cursor { pos.x, pos.y, static_cast<int>(w/2), h };
    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(context->renderer, &cursor);
}

void InputField::render() {
    this->UI_Element::render();
    this->textbox.render();
    if(this->isSelected()) this->renderCursor();
}

InputField::~InputField() {
    if(this->handler)
        this->handler->removeInputFieldFromHandler(this->id);
}