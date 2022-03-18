#include "input_handler.hpp"
#include "button.hpp"

//not needed since Input_Handler does not own the buttons and input_fields
// void Input_Handler::deepCopy(const Input_Handler& handler) {
//     this->listening = handler.listening;
//     this->releaseButtons();
//     this->releaseFields();
//     this->b_pressed = nullptr;

//     this->input_fields = handler.input_fields;
//     this->buttons = handler.buttons;
// }

void Input_Handler::resume()    {   this->listening = true;     }
void Input_Handler::pause()     {   this->listening = false;    }
bool Input_Handler::polling()   {   return this->listening;     };

void Input_Handler::registerButtonToHandler(Button* button) {
    if(button)
        buttons.push_back(button);
    // std::cout << "Registered button to Handler! Id: " << this->buttons.back()->getId() << "\n";
}
void Input_Handler::registerInputFieldToHandler(Input_Field* input_field) {
    if(input_field)
        input_fields.push_back(input_field);
    // std::cout << "Registered button to Handler! Id: " << this->buttons.back()->getId() << "\n";
}

void Input_Handler::mouseClick(const SDL_Point cursor_pos) {
    Input_Field* selected_field = this->getSelectedInputField();
    if(selected_field) {
        selected_field = selected_field->deSelect();
        selected_field = nullptr;
    }

    for (int i{0}; i < buttons.size(); i++) {
        if (buttons.at(i)->isActive() && buttons.at(i)->Clicked(cursor_pos)) {
            // std::cout << "Pressing button!\n";
            b_pressed = buttons.at(i)->press();
            event.type = EVENT_TYPES::BUTTON_PRESS;
            event.button_id = b_pressed->getId();
        }
    }
    for (int i{0}; i < input_fields.size(); i++) {
        if (input_fields.at(i)->isActive() && input_fields.at(i)->Clicked(cursor_pos)) {
            this->input_fields.at(i)->select();
            break;
        }
    }
}

void Input_Handler::keyDown(const SDL_Keycode key) {
    Input_Field* selected_field = this->getSelectedInputField();
    switch(key) {
        case SDLK_SPACE:
            // std::cout << "Detected key press:\t'space'\n";

            break;
        case SDLK_RETURN:
            // std::cout << "Detected key press:\t'enter'\n";
            
            break;
        //Handle backspace
        case SDLK_BACKSPACE:
            if(selected_field) selected_field->del();
            break;
        //Handle copy
        case SDLK_c:
            if(selected_field && (SDL_GetModState() & KMOD_CTRL))
                SDL_SetClipboardText( selected_field->getText().c_str() );
            break;
        //Handle paste
        case SDLK_v:
            if(selected_field && (SDL_GetModState() & KMOD_CTRL))
                selected_field->updateText(std::string(SDL_GetClipboardText()));
            break;
    }
    selected_field = nullptr;
}

void Input_Handler::textInput(const SDL_Keymod modifier, const char input) {
    Input_Field* selected_field = this->getSelectedInputField();
    if(!selected_field) return;
    //Not copy or pasting
    if( !( modifier & KMOD_CTRL && ( input == 'c' || input == 'C' || input == 'v' || input == 'V' ) ) ){
        // std::cout << "CharIn: " << input << '\n';
        selected_field->charIn(input);
        event.type = EVENT_TYPES::CHAR_INPUT;
        event.char_input = input;
    }
    selected_field = nullptr;
}

void Input_Handler::resize() {
    event.type = EVENT_TYPES::RESIZE;
    event.button_id = -1;
    event.char_input = NULL;
}

void Input_Handler::quit() {
    event.type = EVENT_TYPES::QUIT;
    event.button_id = -1;
    event.char_input = NULL;
}

Event_t Input_Handler::pollEvent() {
    Event_t buffer = this->event;
    
    // clear event
    b_pressed = nullptr;
    event.type = EVENT_TYPES::NO_EVENT;
    event.button_id = -1;
    event.char_input = NULL;

    return buffer;
}

// std::vector<Button*> Input_Handler::getSelectedButtons() {
//     return b_pressed;
// }
std::vector<Button*> Input_Handler::getSelectedButtons() {
    std::vector<Button*> selected;
    for (const auto button : buttons)
        if(button->isSelected()) selected.push_back(button);
    return selected;
}
Input_Field* Input_Handler::getSelectedInputField() {
    for (const auto input_field : input_fields)
        if(input_field->isSelected()) return input_field;
    return nullptr;
}

// std::vector<int> Input_Handler::getSelectedButtonIds() {
//     std::vector<int> ids;
//     for (const auto& button : b_pressed)
//         ids.push_back(button->id);
//     return ids;
// }

std::vector<int> Input_Handler::getSelectedButtonIds() {
    std::vector<int> selected;
    for (const auto button : buttons)
        if(button->isSelected()) selected.push_back(button->getId());
    return selected;
}

// void Input_Handler::addButtonToSelected(const Button* button) {
//     b_pressed.push_back(button);
// }

// void Input_Handler::removeButtonFromSelected(const Button* button) {
//     int index {-1};
//     for (int i{0}; i < b_pressed.size(); i++) {
//         if(b_pressed.at(i) == button)
//             index = i;
//     }
//     if(index > 0)
//         b_pressed.erase(index);
// }

void Input_Handler::removeButtonFromHandler(int id) {
    std::cout << "Call to removeButtonFromHandler!\n";
    int i = this->getButtonIndexById(id);
    if(i < 0) return;
    buttons.at(i) = nullptr;
    this->dropButton(i);
    std::cout << "Returned from call to removeButtonFromHandler!\n";
}
void Input_Handler::removeInputFieldFromHandler(int id) {
    // std::cout << "Call to removeInputFieldFromHandler!\n";
    int i = this->getInputFieldIndexById(id);
    if(i < 0) return;
    input_fields.at(i) = nullptr;
    this->dropInputField(i);
}