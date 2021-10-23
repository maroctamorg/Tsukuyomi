#ifndef BUTTON_H
#define BUTTON_H

#include "extern.hpp"
#include "layout.hpp"

class EventHandler;

typedef struct
{
    bool active, select, pressed, selected;
} State;

class Button final : public UI_Element {
private:
    const unsigned int id;
    std::shared_ptr<Layout> layout { nullptr };

    // must implement animations for button presses to feel real
    std::function<void(const GraphicsContext& context, const EventHandler& handler, Button& button)> callback;
    std::shared_ptr<EventHandler> handler;
    State state { false };

public:
    Button(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::shared_ptr<Layout> layout, const unsigned int id, bool active = true, bool select = false, SDL_Color color = SDL_Color({0, 0, 0, 0}), SDL_Rect rect = SDL_Rect({0, 0, 0, 0}));
    Button(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::shared_ptr<Layout> layout, const unsigned int id, bool active = true, bool select = false, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}), SDL_Texture* a_texture = nullptr);
    ~Button();

public:
    int getId();
    void registerCallBack(std::function<void(const GraphicsContext& context, const EventHandler& handler, Button& button)> callback);

    void activate();
    void select();
    void deactivate();
    Button* press();
    void dettachHandler();

    bool isSelected();
    bool isActive();
    bool isPressed();
    bool Clicked(const SDL_Point &cursor_pos);

    void update() override;
    void updatePosition (const SDL_Rect& rect) override;
    void updateSize() override;
    void render() override;
};

#endif
