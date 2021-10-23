#include "event.hpp"

void EventHandler::registerButtonToHandler(Button* button) {
    if(button)
        buttons.push_back(button);
    // std::cout << "Registered button to Handler! Id: " << this->buttons.back()->getId() << "\n";
}
void EventHandler::registerInputFieldToHandler(InputField* input_field) {
    if(input_field)
        input_fields.push_back(input_field);
    // std::cout << "Registered button to Handler! Id: " << this->buttons.back()->getId() << "\n";
}

Event EventHandler::PollEvent() {
    // remove previous button press
    b_pressed = nullptr;

    SDL_Point cursor_pos {-100, -100};
    if(!SDL_PollEvent(&(event.sdl_event))) {
        event.type = EVENT_TYPES::NO_EVENT;
        event.button_id = -1;
        event.char_input = NULL;
        return this->event;
    };
    InputField* selected_field = this->getSelectedInputField();
    switch(event.sdl_event.type) {
        case (SDL_MOUSEBUTTONDOWN) :
            // std::cout << "SDL_MOUSEBUTTONDOWN event registered.\n";
            SDL_GetMouseState(&(cursor_pos.x), &(cursor_pos.y));
            // std::cout << "Mouse State obtained:\t" << cursor_pos.x << ",\t" << cursor_pos.y << "\n";
            if(selected_field)  selected_field->deSelect();
            for (int i{0}; i < buttons.size(); i++) {
                if (buttons.at(i)->isActive() && buttons.at(i)->Clicked(cursor_pos)) {
                    // std::cout << "Pressing button!\n";
                    b_pressed = buttons.at(i)->press();
                }
            }
            for (int i{0}; i < input_fields.size(); i++) {
                if (input_fields.at(i)->isActive() && input_fields.at(i)->Clicked(cursor_pos)) {
                    this->input_fields.at(i)->select();
                    break;
                }
            }
            break;
        case SDL_KEYDOWN:
            if(!selected_field)
                break;
            switch(event.sdl_event.key.keysym.sym) {
                case SDLK_SPACE:
                    std::cout << "Detected key press:\t'space'\n";
                    
                    break;
                case SDLK_RETURN:
                    std::cout << "Detected key press:\t'enter'\n";
                    
                    break;
                //Handle backspace
                case SDLK_BACKSPACE:
                    selected_field->del();
                    break;
                //Handle copy
                case SDLK_c:
                    if(SDL_GetModState() & KMOD_CTRL ){
                        SDL_SetClipboardText( selected_field->getText().c_str() );
                    }
                    break;
                //Handle paste
                case SDLK_v :
                    if(SDL_GetModState() & KMOD_CTRL ) {
                        selected_field->updateText( std::string(SDL_GetClipboardText()));
                    }
                break;
            }
            break;
        case SDL_TEXTINPUT :
            if(!selected_field)
                break;
            //Not copy or pasting
            if( !( SDL_GetModState() & KMOD_CTRL && ( event.sdl_event.text.text[ 0 ] == 'c' || event.sdl_event.text.text[ 0 ] == 'C' || event.sdl_event.text.text[ 0 ] == 'v' || event.sdl_event.text.text[ 0 ] == 'V' ) ) ){
                selected_field->charIn(event.sdl_event.text.text);
            }
            break;
    }
    selected_field = nullptr;

    if(!b_pressed) {
        event.button_id = -1;
        event.type = EVENT_TYPES::UNHANDLED_SDL_EVENT;
    }
    else {
        event.button_id = b_pressed->getId();
        event.type = EVENT_TYPES::BUTTON_PRESS;
    }
    // int index = getCallbackIndexById(id);
    // if(index > 0)
    //     callbacks.at(i).execute();
    return this->event;

}

// std::vector<Button*> EventHandler::getSelectedButtons() {
//     return b_pressed;
// }
std::vector<Button*> EventHandler::getSelectedButtons() {
    std::vector<Button*> selected;
    for (const auto button : buttons)
        if(button->isSelected()) selected.push_back(button);
    return selected;
}
InputField* EventHandler::getSelectedInputField() {
    for (const auto input_field : input_fields)
        if(input_field->isSelected()) return input_field;
    return nullptr;
}

// std::vector<int> EventHandler::getSelectedButtonIds() {
//     std::vector<int> ids;
//     for (const auto& button : b_pressed)
//         ids.push_back(button->id);
//     return ids;
// }

std::vector<int> EventHandler::getSelectedButtonIds() {
    std::vector<int> selected;
    for (const auto button : buttons)
        if(button->isSelected()) selected.push_back(button->getId());
    return selected;
}

// void EventHandler::addButtonToSelected(const Button* button) {
//     b_pressed.push_back(button);
// }

// void EventHandler::removeButtonFromSelected(const Button* button) {
//     int index {-1};
//     for (int i{0}; i < b_pressed.size(); i++) {
//         if(b_pressed.at(i) == button)
//             index = i;
//     }
//     if(index > 0)
//         b_pressed.erase(index);
// }

void EventHandler::removeButtonFromHandler(int id) {
    std::cout << "Call to removeButtonFromHandler!\n";
    int i = this->getButtonIndexById(id);
    if(i < 0) return;
    buttons.at(i) = nullptr;
    this->dropButton(i);
    std::cout << "Returned from call to removeButtonFromHandler!\n";
}
void EventHandler::removeInputFieldFromHandler(int id) {
    // std::cout << "Call to removeInputFieldFromHandler!\n";
    int i = this->getInputFieldIndexById(id);
    if(i < 0) return;
    input_fields.at(i) = nullptr;
    this->dropInputField(i);
}