#include "input_field.hpp"
#include "input_handler.hpp"

Input_Field::Input_Field(std::shared_ptr<Graphics_Context> context, std::shared_ptr<Input_Handler> handler, const unsigned int id, const std::string font, int ptsize, const SDL_Colour font_color, SDL_Color color, ALIGN_X alignX, ALIGN_Y alignY, bool active, SDL_Rect rect)
    :   UI_Element(context, rect, color), handler(handler), id { id }, text_box { context, "", font, ptsize, font_color, alignX, alignY }, state { active, false } {
        if(this->handler)
            this->handler->registerInputFieldToHandler(this);
    }
Input_Field::Input_Field(std::shared_ptr<Graphics_Context> context, std::shared_ptr<Input_Handler> handler, const unsigned int id, const std::string font, int ptsize, const SDL_Colour font_color, std::string texture_path, ALIGN_X alignX, ALIGN_Y alignY, bool active, SDL_Rect rect)
    :   UI_Element(context, rect, texture_path), handler(handler), id { id }, text_box { context, "", font, ptsize, font_color, alignX, alignY }, state { active, false } {
        if(this->handler)
            this->handler->registerInputFieldToHandler(this);
    }

int Input_Field::getId() { return id; }
std::string Input_Field::getText() { return this->text_box.getText(); }
// void Input_Field::registerCallBack(std::function<void(Graphics_Context*, Input_Handler*, std::shared_ptr<Input_Field>)> callback) {
//     this->callback = callback;
// }
void Input_Field::activate()     {   state.active    =   true;   }
Input_Field* Input_Field::select() {
    // std::cout << "Call to input select!\n";
    if(!state.active)
        return this;
    state.selected = true;
    // callback(context.get(), handler.get(), this);
    return this;
}
Input_Field* Input_Field::deSelect() {
    // std::cout << "Call to input select!\n";
    if(!state.active)
        return this;
    state.selected = false;
    // callback(context.get(), handler.get(), this);
    return this;
}
void Input_Field::charIn(char a) {
    int length = this->text_box.getText().length();
    int capacity = this->text_box.calculateCapacity();
    if(length >= capacity) {
        std::cout << "Returning early from Input_Field::charIn, not enough space!\n\tlength: " << length << "\tcapacity: " << capacity << '\n';
        return;
    }
    this->text_box.append(a);
    if(a != '\n') {
        this->cursorPos++;
        // std::cout << "Moving up cursor!\n";
    }
}
void Input_Field::del() {
    if(this->text_box.del()) this->cursorPos--;
}
void Input_Field::deactivate()   {   state.active  =   false;  }
void Input_Field::dettachHandler() {
    if(this->handler) {
        this->handler = nullptr;
    }
}

bool Input_Field::isActive()     {   return state.active;    }
bool Input_Field::isSelected()   {   return state.selected;  }

bool Input_Field::Clicked(const SDL_Point &cursor_pos) {
    // std::cout << "Call to clicked:\nrect ->\tx: " << rect.x << "\ty: " << rect.y << "\tw: " << rect.w << "\th: " << rect.h << '\n';
    return isContained(cursor_pos, this->rect);
}

void Input_Field::updateText(std::string text) {
    this->text_box.updateText(text);
}

void Input_Field::update() {
    this->text_box.update();
}

void Input_Field::updatePosition (const SDL_Rect& rect) {
    this->UI_Element::updatePosition(rect);
    int w,h;
    this->text_box.getCharTextureSize(&w, &h);
    SDL_Rect text_rect { rect.x + w, rect.y + static_cast<int>(h/4), rect.w - 2*w, rect.h - static_cast<int>(h/2) };
    this->text_box.updatePosition(text_rect);
}

void Input_Field::updateSize () {
    this->text_box.updateSize();
}

void Input_Field::renderCursor() {
    int w, h;
    this->text_box.getCharTextureSize(&w, &h);
    int line {0}, counter{0}, index {0};
    if (this->text_box.numberOfLines() != 0) {
        while (counter <= cursorPos && index < this->text_box.numberOfLines()) {
            counter += this->text_box.getContentLengthFromLines(index);
            line++;
            index++;
        }
        line -= 1;
        counter -= this->text_box.getContentLengthFromLines(index - 1);
    }
    SDL_Point pos = this->text_box.getPos(line, this->cursorPos - counter);
    SDL_Rect cursor { pos.x, pos.y, static_cast<int>(w/2), h };
    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(context->renderer, &cursor);
}

void Input_Field::render() {
    this->UI_Element::render();
    this->text_box.render();
    if(this->isSelected()) this->renderCursor();
}

Input_Field::~Input_Field() {
    if(this->handler)
        this->handler->removeInputFieldFromHandler(this->id);
}