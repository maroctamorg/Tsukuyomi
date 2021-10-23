#ifndef TEXT_UI_HPP
#define TEXT_UI_HPP

#include "extern.hpp"
#include "ui_element.hpp"

class TextBox final : public UI_Element {
private:
    std::string contents;
    std::string font;
    int ptsize;
    SDL_Color font_color {0, 0, 0, 255};
    std::vector<std::unique_ptr<Text>> lines;
    ALIGN_X align_x { ALIGN_X::CENTER };
    ALIGN_Y align_y { ALIGN_Y::CENTER };


public:
    TextBox(const std::shared_ptr<GraphicsContext> context, const std::string text, const std::string font, int ptsize, const SDL_Colour font_color, ALIGN_X alignX = ALIGN_X::CENTER, ALIGN_Y alignY = ALIGN_Y::CENTER, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}), SDL_Colour color = SDL_Colour({0, 0, 0, 0}), SDL_Colour border_color = SDL_Colour({0, 0, 0, 0}), bool hidden = false, int r = 0)
    :   UI_Element(context, rect, color, border_color, hidden, r), contents(text), font(font), ptsize(ptsize), font_color(font_color), align_x{alignX}, align_y{alignY} {
        // std::unique_ptr<Text>
        // this->lines.push_back(std::make_unique<Text>(context->renderer, text, font, ptsize, font_color));
        this->breakContentsToLines();
        // this->adaptContentsToBox();
    }

    ~TextBox() = default;

public:
    std::string getText();
    void getCharTextureSize(int* w, int* h);
    int numberOfLines();
    SDL_Point getPos(int line, int charPos);
    void updateAlignment(ALIGN_X alignX, ALIGN_Y alignY);
    void updateFontSize(int ptsize);
    int calculateCapacity();
    bool checkOverflow();
    int getContentLengthFromLines(int start_index=0, int end_index=-1);
    bool breakContentsToLines(int start_index=0, int content_pointer=0); //recursive
    // bool breakContentsToLines(); //iterative
    void adaptContentsToBox();
    void append(char a);
    bool del();
    void updateText(std::string text);
    void render() override;
    void updatePosition (const SDL_Rect& rect) override;
    void updateSize() override;
};

#endif