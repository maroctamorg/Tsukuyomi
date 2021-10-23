#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "extern.hpp"
#include "ui_element.hpp"

enum class X_POS {
    TOP = 0,
    CENTER = 1,
    BOTTOM = 2,
};

enum class Y_POS {
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2,
};

class Container final {
private:
    float r_x { 0 };
    float r_y { 0 };
    float r_w { 0 };
    float r_h { 0 };
    // X_POS x_pos { X_POS::CENTER };
    // Y_POS y_pos { Y_POS::CENTER };
    // UI_Element* ui_element { nullptr };

public:
    Container() = default;
    Container(float rx, float ry, float rw, float rh)
        : r_x{rx}, r_y{ry}, r_w{rw}, r_h{rh} {}
    // Container(int rx, int ry, int rw, int rh, X_POS xpos, Y_POS ypos, UI_Element* element)
    //     : r_x{rx}, r_y{ry}, r_w{rw}, r_h{rh}, x_pos{xpos}, y_pos{ypos}, ui_element{element} {
    //         ui_element.updateSize();
    //     }
    ~Container() {
        // setting ui_element to nullptr so it doesn't destroy the resource
        // since the implementation is only internal and no external use of this class is intended
        // there is no need for smart pointers (destruction is handled by the layout class)
        // ui_element = nullptr;
    }

public:
    // rect is the context rectangle (if the layout is at the window level, it is the window rectangle;
    // if it is at the pannel level, then the pannel rectangle)
    void place(UI_Element* element, const SDL_Rect& rect);
};

class Layout final : public UI_Element {
private:
    std::vector<Container> containers;
    std::vector<std::shared_ptr<UI_Element>> ui_elements;
    // std::vector<std::weak_ptr<UI_Element>> ui_elements;

public:
    Layout(std::shared_ptr<GraphicsContext> context, std::initializer_list<Container> list)
        : UI_Element(context), containers(list) {
            if(this->rect.w == 0 || this->rect.h == 0)
                this->rect = SDL_Rect({0, 0, this->context->getWidth(), this->context->getHeight()});
        };

    // Move constructor
	Layout(Layout&& layout) noexcept
		: UI_Element(layout.context, layout.rect, layout.texture)
	{
        if(this->rect.w == 0 || this->rect.h == 0)
            this->rect = SDL_Rect({0, 0, this->context->getWidth(), this->context->getHeight()});

        for(int i = 0; i < layout.ui_elements.size(); i++)
            this->ui_elements.push_back(layout.ui_elements.at(i));
        for(int i = 0; i < layout.containers.size(); i++)
            this->containers.push_back(layout.containers.at(i));
        layout.texture = nullptr;
	}
 
	// Copy assignment
	// Layout& operator=(const Layout& layout) = delete;
 
	// Move assignment
	Layout& operator=(Layout&& layout) noexcept
	{
		if (&layout == this)
			return *this;

        if(texture)
            SDL_DestroyTexture(texture);
        texture = nullptr;
        
        // MUST IMPLEMENT CONTEXT VARIABLE MOVE AS WELL!

        if(this->rect.w == 0 || this->rect.h == 0)
            this->rect = SDL_Rect({0, 0, this->context->getWidth(), this->context->getHeight()});

        this->ui_elements = std::vector<std::shared_ptr<UI_Element>>(layout.ui_elements.size());
        this->containers = std::vector<Container>(0);

        for(int i = 0; i < layout.ui_elements.size(); i++)
            this->ui_elements.push_back(layout.ui_elements.at(i));
        for(int i = 0; i < layout.containers.size(); i++)
            this->containers.push_back(layout.containers.at(i));

        context = layout.context;
        texture = layout.texture;

		return *this;
	}

    ~Layout() = default;

public:
    // void placeUI_Element(std::unique_ptr<UI_Element> element, int index);
    void placeUI_Element(std::shared_ptr<UI_Element> element, int index);
    void placeUI_Element(UI_Element* a_element, int index);

    void render() override;
    void update() override;
    void updateSize() override;
    void updatePosition(const SDL_Rect& rect) override;
};

#endif