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
    bool update_text {false};
    bool update_font {false};
    std::string text;
    std::string font;
    int ptsize { 20 };
    TTF_Font* txt_font { nullptr };
    SDL_Texture* txt_texture { nullptr };
    SDL_Colour color { 0, 0, 0, 250 };

private:
    bool inline checkTextureOnTheFly(SDL_Renderer* renderer) {
        if (!txt_texture) {
            if(!txt_font) {
                try {
                    this->loadFont();
                } catch (...) {
                    std::cout << "EXCEPTION:\tUnable to load text font on the fly.\n";
                    return false;
                }
            }
            try {
                this->generateTxtTexture(renderer);
            } catch (...) {
                std::cout << "EXCEPTION:\tUnable to generate text texture on the fly.\n";
                return false;
            }
            return true;
        }
        return true;
    }

    void deepCopy(const Text& text);
    
    bool loadFont();
    int generateTexture(SDL_Renderer *renderer);
    int generateTxtTexture(SDL_Renderer *renderer);

    void destroyFont();
    void destroyTxtTexture();
    
public:
    Text(SDL_Renderer* renderer, const std::string text, const std::string font, int ptsize, const SDL_Colour color);
    // implement move/copy constructor/assignment
    // Copy constructor
    Text(const Text& text) {
        deepCopy(text);
    }

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
    Text& operator=(const Text& text) {
        if(&text != this) deepCopy(text);
        return *this;
    }

    // Move assignment
    Text& operator=(Text&& text) noexcept
    {
        if(&text == this) return *this;

        this->text = text.text;
        this->font = text.font;
        this->ptsize = text.ptsize;
        this->color = text.color;

        this->destroyFont();
        this->destroyTxtTexture();

        this->txt_texture = text.txt_texture; 
        this->txt_font = text.txt_font; 

        text.txt_texture = nullptr;
        text.txt_font = nullptr;
        
        return *this;
    }

    ~Text() {
        destroyFont();
        destroyTxtTexture();
    }

public:
    bool inline isFontLoaded() {
        return (txt_font ? true : false);
    }

    bool inline isTextureLoaded() {
        return (txt_texture ? true : false);
    }

    std::string getString();
    SDL_Point getPos(SDL_Renderer* renderer, const SDL_Rect& target, ALIGN_X alignX, ALIGN_Y alignY);
    int getLength();
    int getPtSize();
    void getCharacterTextureSize(int* w, int* h);
    
    std::array<std::string,2> split(int index);
    void update(SDL_Renderer *renderer);
    // void updateTxt(SDL_Renderer *renderer, const std::string text);
    // void updateFontSize(SDL_Renderer *renderer, const int ptsize);
    void updateTxt(const std::string text);
    void updateFontSize(const int ptsize);
    void display(SDL_Renderer* renderer, const SDL_Rect& target, ALIGN_X alignX, ALIGN_Y alignY);
};

#endif