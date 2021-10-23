#ifndef INPUT_HPP
#define INPUT_HPP

#include "extern.hpp"
#include "text.hpp"

class EventHandler;

typedef struct {
    bool active, selected;
} field_state;

class InputField : public UI_Element {
private:
    const unsigned int id;
    int cursorPos { 0 };
    TextBox textbox;

    // std::function<void(GraphicsContext*, EventHandler*, Button*)> callback;
    std::shared_ptr<EventHandler> handler;
    field_state state { true, false };

public:
    InputField(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, const unsigned int id, const std::string font, int ptsize, const SDL_Colour font_color, SDL_Color color = SDL_Color({255, 255, 255, 255}), ALIGN_X alignX = ALIGN_X::LEFT, ALIGN_Y alignY = ALIGN_Y::CENTER, bool active = true, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}));
    InputField(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, const unsigned int id, const std::string font, int ptsize, const SDL_Colour font_color, SDL_Texture* a_texture, ALIGN_X alignX = ALIGN_X::LEFT, ALIGN_Y alignY = ALIGN_Y::CENTER, bool active = true, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}));
    ~InputField();

public:
    int getId();
    std::string getText();
    // void registerCallBack(std::function<void(GraphicsContext*, EventHandler*, Button*)> callback);

    void activate();
    void deactivate();
    InputField* select();
    InputField* deSelect();
    void charIn(char* a);
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
//     std::vector<std::shared_ptr<InputField>> input_fields;
//     std::vector<std::shared_ptr<Button>> buttons;

// public:
//     InputForm(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, Layout* layout, Button* button, TextBox* text, SDL_Color color = SDL_Color({0, 0, 0, 0}), SDL_Rect rect = SDL_Rect({0, 0, 0, 0}));
//     InputForm(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, Layout* layout, Button* button, TextBox* text, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}), SDL_Texture* a_texture = nullptr);
//     ~InputForm();

// public:
//     std::string getInput();

// };

#endif