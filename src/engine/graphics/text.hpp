#ifndef TEXT_GRAPHICS_H
#define TEXT_GRAPHICS_H

#include "extern.hpp"

enum class ALIGN_Y {
    TOP = 0,
    CENTER = 1,
    BOTTOM = 2

};

enum class ALIGN_X {
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2
};

class Text {
private:
    std::string text;
    std::string font;
    int ptsize { 20 };
    TTF_Font* txt_font { nullptr };
    SDL_Texture* txt_texture { nullptr };
    SDL_Colour color { 0, 0, 0, 250 };

public:
    int generateTxtTexture(SDL_Renderer *renderer);
    void destroyTxtTexture();


    Text(SDL_Renderer* renderer, const std::string text, const std::string font, int ptsize, const SDL_Colour color);
    // implement move/copy constructor/assignment
    // Copy constructor
    Text(const Text& text) = delete;

    // Move constructor
	Text(Text&& text) noexcept
		:   text(text.text), font(text.font), ptsize(text.ptsize), color(text.color)
	{
        this->txt_font = text.txt_font;
        this->txt_texture = text.txt_texture;

        text.txt_font = nullptr;
        text.txt_texture = nullptr;
	}
 
	// Copy assignment
	Text& operator=(const Text& text) = delete;
 
	// Move assignment
	Text& operator=(Text&& text) noexcept
	{
		if (&text == this)
			return *this;

        this->text = text.text;
        this->font = text.font;
        this->ptsize = text.ptsize;
        this->color = text.color;

        if(this->txt_texture)
            SDL_DestroyTexture(this->txt_texture);
        this->txt_texture = nullptr;
        if(this->txt_font)
            TTF_CloseFont(this->txt_font);
        this->txt_font = nullptr;
 
		this->txt_texture = text.txt_texture; 
		this->txt_font = text.txt_font; 

        text.txt_texture = nullptr;
        text.txt_font = nullptr;
        
		return *this;
	}

    ~Text();

public:
    bool isLoaded();
    bool loadFont();

    std::string getString();
    SDL_Point getPos(SDL_Renderer* renderer, const SDL_Rect& target, ALIGN_X alignX, ALIGN_Y alignY);
    int getLength();
    int getPtSize();
    void getCharacterTextureSize(int* w, int* h);
    
    std::array<std::string,2> split(int index);
    void updateTxt(SDL_Renderer *renderer, const std::string text);
    void updateFontSize(SDL_Renderer *renderer, const int ptsize);
    void display(SDL_Renderer* renderer, const SDL_Rect& target, ALIGN_X alignX, ALIGN_Y alignY);
};


#endif