#ifndef UI_INPUT_HANDLER_HPP
#define UI_INPUT_HANDLER_HPP

#include "extern_ui.hpp"
#include "input_field.hpp"
#include "button.hpp"

enum class EVENT_TYPES {
    NO_EVENT = -1,
    BUTTON_PRESS = 1,
    CHAR_INPUT = 2,
};

typedef struct {
    EVENT_TYPES type { EVENT_TYPES::NO_EVENT };
    int button_id { -1 };
    char char_input;
} Event;

class Input_Handler {
private:
    bool listening {true};
    // Menu* menu { nullptr };
    std::vector<Input_Field*> input_fields;
    std::vector<Button*> buttons;
    Button* b_pressed { nullptr };

    Event event;

public:
    Input_Handler() = default;
    // provide move assignment and copy?

    ~Input_Handler() {
        for (auto& button : buttons) {
            // button->dettachHandler();
            button = nullptr;
        }
        for (auto& input_field : input_fields) {
            // input_field->dettachHandler();
            input_field = nullptr;
        }
        b_pressed = nullptr;
    }

public:
    inline Button* getButtonById(int id) {
        for(const auto& button : buttons)
            if(button->getId() == id) return button;
        return nullptr;
    }
    inline int getButtonIndexById(int id) {
        for(int i{0}; i < buttons.size(); i++)
            if(buttons.at(i)->getId() == id) return i;
        return -1;
    }

    inline void dropButton(int index) {
        std::vector<Button*> buffer;
        for (int i = buttons.size() - 1; i > -1; i--) {
            if(i != index) buffer.push_back(buttons.at(i));
            buttons.at(i) = nullptr;
            buttons.pop_back();
        }
    }
    inline Input_Field* getInputFieldById(int id) {
        for(const auto& input_field : input_fields)
            if(input_field->getId() == id) return input_field;
        return nullptr;
    }
    inline int getInputFieldIndexById(int id) {
        for(int i{0}; i < input_fields.size(); i++)
            if(input_fields.at(i)->getId() == id) return i;
        return -1;
    }

    inline void dropInputField(int index) {
        std::vector<Input_Field*> buffer;
        for (int i = input_fields.size() - 1; i > -1; i--) {
            if(i != index) buffer.push_back(input_fields.at(i));
            input_fields.at(i) = nullptr;
            input_fields.pop_back();
        }
    }

    // inline unsigned int getCallbackIndexById(int id) {
    //     for(int i{0}; i < callbacks.size(); i++)
    //         if(callbacks.at(i).id == id) return i;
    //     return -1;
    // }

    // void registerCallBack(unsigned int id, std::function<void(Graphics_Context*, Event_Handler*, Button*)> callback) {
    //     if(!getButtonById(id)) return;
    //     if(const index = getCallbackIndexById(); index > 0)
    //         callbacks.at(i) = { id, callback }; return;
    //     callbacks.push_back({ id, callback });
    // }

public:
    void resume();
    void pause();
    bool polling();

    void registerButtonToHandler(Button* button);
    void registerInputFieldToHandler(Input_Field* input_field);

    void mouseClick(const SDL_Point cursor_pos);
    void keyDown(const SDL_Keycode key);
    void textInput(const SDL_Keymod modifier, const char input);

    Event pollEvent();

    std::vector<Button*> getSelectedButtons();
    Input_Field* getSelectedInputField();
    std::vector<int> getSelectedButtonIds();
    // void addButtonToSelected(const Button* button);
    // void removeButtonFromSelected(const Button* button);
    void removeButtonFromHandler(int id);
    void removeInputFieldFromHandler(int id);
};

#endif