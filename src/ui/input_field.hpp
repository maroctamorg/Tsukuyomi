#ifndef UI_INPUT_FIELD_HPP
#define UI_INPUT_FIELD_HPP

#include "extern_ui.hpp"
#include "ui_element.hpp"
#include "text_box.hpp"

class Input_Handler;

typedef struct {
    bool active, selected;
} field_state;

class Input_Field : public UI_Element {
private:
    const unsigned int id;
    int cursorPos { 0 };
    Text_Box text_box;

    // std::function<void(Graphics_Context*, Input_Handler*, Button*)> callback;
    std::shared_ptr<Input_Handler> handler;
    field_state state { true, false };

public:
    Input_Field(std::shared_ptr<Graphics_Context> context, std::shared_ptr<Input_Handler> handler, const unsigned int id, const std::string font, int ptsize, const SDL_Colour font_color, SDL_Color color = SDL_Color({255, 255, 255, 255}), ALIGN_X alignX = ALIGN_X::LEFT, ALIGN_Y alignY = ALIGN_Y::CENTER, bool active = true, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}));
    Input_Field(std::shared_ptr<Graphics_Context> context, std::shared_ptr<Input_Handler> handler, const unsigned int id, const std::string font, int ptsize, const SDL_Colour font_color, std::string texture_path, ALIGN_X alignX = ALIGN_X::LEFT, ALIGN_Y alignY = ALIGN_Y::CENTER, bool active = true, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}));
    ~Input_Field();

public:
    int getId();
    std::string getText();
    // void registerCallBack(std::function<void(Graphics_Context*, Input_Handler*, Button*)> callback);

    void activate();
    void deactivate();
    Input_Field* select();
    Input_Field* deSelect();
    void charIn(char a);
    void del();
    void dettachHandler();

    bool isSelected();
    bool isActive();
    bool Clicked(const SDL_Point &cursor_pos);

    void updateText(std::string text);
    void update() override;
    void updatePosition (const SDL_Rect& rect) override;
    void updateSize() override;
    void renderCursor();
    void render() override;
};

// class InputForm : public UI_Element {
// private:
//     std::unique_ptr<Layout> layout;
//     std::vector<std::shared_ptr<Input_Field>> input_fields;
//     std::vector<std::shared_ptr<Button>> buttons;

// public:
//     InputForm(std::shared_ptr<Graphics_Context> context, std::shared_ptr<Input_Handler> handler, Layout* layout, Button* button, Text_Box* text, SDL_Color color = SDL_Color({0, 0, 0, 0}), SDL_Rect rect = SDL_Rect({0, 0, 0, 0}));
//     InputForm(std::shared_ptr<Graphics_Context> context, std::shared_ptr<Input_Handler> handler, Layout* layout, Button* button, Text_Box* text, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}), SDL_Texture* a_texture = nullptr);
//     ~InputForm();

// public:
//     std::string getInput();

// };

#endif