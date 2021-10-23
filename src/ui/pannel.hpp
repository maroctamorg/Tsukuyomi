#ifndef PANNEL_HPP
#define PANNEL_HPP

#include "extern.hpp"
#include "ui_element.hpp"
#include "layout.hpp"

class Pannel final : public UI_Element {
private:
    std::shared_ptr<Layout> layout { nullptr };

public:
    // Pannel() = default;
    Pannel(std::shared_ptr<GraphicsContext> context, std::shared_ptr<Layout> layout, SDL_Rect rect, SDL_Texture* a_texture = nullptr)
        :   UI_Element(context, rect, a_texture), layout(layout) {
            if(layout)
                this->layout->updatePosition(this->rect);
        }
    Pannel(std::shared_ptr<GraphicsContext> context, std::shared_ptr<Layout> layout, SDL_Color color = SDL_Color({0, 0, 0, 0}), SDL_Rect rect = SDL_Rect({0, 0, 0, 0}))
        : UI_Element(context, rect, color), layout(layout) {
            if(layout)
                this->layout->updatePosition(this->rect);
        }

    // Implement constructors using l/r-value references and std::unique_ptr as well for user personal preference

    // Copy constructor
    Pannel(const Pannel& pannel) = delete;

    // Move constructor
	Pannel(Pannel&& pannel) noexcept
		: UI_Element(pannel.context, pannel.rect, pannel.texture)
	{
        layout = pannel.layout;
        texture = pannel.texture;
        pannel.texture = nullptr;
        if(this->layout)
            this->layout->updatePosition(this->rect);
	}
 
	// Copy assignment
	Pannel& operator=(const Pannel& pannel) = delete;
 
	// Move assignment
	Pannel& operator=(Pannel&& pannel) noexcept
	{
		if (&pannel == this)
			return *this;

        if(texture)
            SDL_DestroyTexture(texture);
        texture = nullptr;
 
		layout = pannel.layout; 
        context = pannel.context;
        texture = pannel.texture;

        pannel.texture = nullptr;
        
        if(layout)
            this->layout->updatePosition(this->rect);

		return *this;
	}

    ~Pannel() = default;

public:
    void render() override;
    void update() override;
    void updatePosition(const SDL_Rect& rect) override;
    void updateSize() override;
};

#endif