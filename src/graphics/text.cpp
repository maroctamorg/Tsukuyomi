#include "text.hpp"

Text::Text(SDL_Renderer* renderer, const std::string text, const std::string font, int ptsize, const SDL_Colour color)
    : text(text), font(font), ptsize(ptsize), color(color) {
    if(this->loadFont())
        this->generateTexture(renderer);
}

void Text::deepCopy(const Text& text) {
    this->destroyFont();
    this->destroyTxtTexture();

    this->text = text.text;
    this->font = text.font;
    this->ptsize = text.ptsize;
    this->color = text.color;

    this->loadFont();
    this->txt_texture = nullptr;
}

bool Text::loadFont() {
    destroyFont();

    try {
        txt_font = TTF_OpenFont(font.c_str(), ptsize);
    } catch(...) {
        std::cout << "EXCEPTION:\tError loading font... Yet to implement default...\n";
        txt_texture = nullptr;
        return false;
    }
    return true;
}

std::string Text::getString() {
    return this->text;
}

SDL_Point Text::getPos(SDL_Renderer* renderer, const SDL_Rect& target, ALIGN_X alignX, ALIGN_Y alignY) {
    if (!checkTextureOnTheFly(renderer))
        return {-100, -100};

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(txt_texture, NULL, NULL, &texW, &texH);

    SDL_Rect dstrect {0, 0, texW, texH};
    switch(alignX) {
        case (ALIGN_X::LEFT): {
            dstrect.x = target.x;
            break;
        }
        case (ALIGN_X::CENTER): {
            dstrect.x = target.x + static_cast<int>( (target.w - texW) / 2 - 0.5);
            break;
        }
        case (ALIGN_X::RIGHT): {
            dstrect.x = target.x + target.w - texW;
            break;
        }
    }
    switch(alignY) {
        case (ALIGN_Y::TOP): {
            dstrect.y = target.y;
            break;
        }
        case (ALIGN_Y::CENTER): {
            dstrect.y = target.y + static_cast<int>( (target.h - texH) / 2 - 0.5);
            break;
        }
        case (ALIGN_Y::BOTTOM): {
            dstrect.y = target.y + target.h - texH;
            break;
        }
    }
    return { dstrect.x, dstrect.y };
}

int Text::getLength() {
    return this->text.length();
}
int Text::getPtSize() {
    return this->ptsize;
}
void Text::getCharacterTextureSize(int* w, int* h) {
    int length = text.length();
    // int sum_w {0};
    // int sum_h {0};
    // int temp_w, temp_h;
    // for(int i = 0; i < length; i++) {
    //     // probably the problem is the string being passed here! Rather than doing this, I should just divide for the total string size!
    //     TTF_SizeText(txt_font, text.c_str(), &temp_w, &temp_h);
    //     sum_w += temp_w;
    //     sum_h += temp_h;
    // }
    // *w = static_cast<int>(sum_w / length);
    // *h = static_cast<int>(sum_h / length);
    int lw, lh;
    try {
        TTF_SizeText(txt_font, text.c_str(), &lw, &lh);
    } catch (...) {
        std::cout << "EXCEPTION:\tUnable to measure font size.\n";
        return;
    }
    *w = static_cast<int>(lw / length);
    *h = lh;
}

std::array<std::string,2> Text::split(int index) {
    if(index < 0 || index >= this->text.length() - 1)
        return std::array<std::string, 2> { text, "" };
    return std::array<std::string, 2> { text.substr(0, index + 1), text.substr(index + 1, text.length() - (index + 1)) };
}

void Text::update(SDL_Renderer *renderer) {
    if(update_font) loadFont();
    if(update_text)  generateTxtTexture(renderer);
}

// void Text::updateTxt(SDL_Renderer* renderer, const std::string text) {
void Text::updateTxt(const std::string text) {
    this->text = text;
    update_text = true;
    // this->generateTxtTexture(renderer);
}

// void Text::updateFontSize(SDL_Renderer *renderer, const int ptsize) {
void Text::updateFontSize(const int ptsize) {
    this->ptsize = ptsize;
    update_font = true;
    update_text = true;
    // if(this->loadFont())
    //     this->generateTxtTexture(renderer);
}

void Text::display(SDL_Renderer* renderer, const SDL_Rect& target, ALIGN_X alignX, ALIGN_Y alignY) {
    // std::cout << "Target passed to text display: {" << target.x << ", " << target.y << ", " << target.w << ", " << target.h << "}\n";
    
    if(!checkTextureOnTheFly(renderer)) return;

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(txt_texture, NULL, NULL, &texW, &texH);

    SDL_Rect dstrect {0, 0, texW, texH};
    SDL_Point pos = this->getPos(renderer, target, alignX, alignY);
    dstrect.x = pos.x;
    dstrect.y = pos.y;
    // std::cout << "Displaying to target: {" << dstrect.x << ", " << dstrect.y << ", " << dstrect.w << ", " << dstrect.h << "}\n";
    SDL_RenderCopy(renderer, txt_texture, NULL, &dstrect);
}

int Text::generateTxtTexture(SDL_Renderer *renderer)
{
    // std::cout << "Generating text texture for '" << text << "' in font <'" << font << "'>\n";
    destroyTxtTexture();
    try {
        SDL_Surface *surface { TTF_RenderText_Blended(txt_font, text.c_str(), color) };
        txt_texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FreeSurface(surface);
        surface = nullptr;
    } catch (...) {
        std::cout << "EXCEPTION:\tUnable to generate text texture on the fly.\n";
        return 0;
    }

    return 1;
}

int Text::generateTexture(SDL_Renderer *renderer)
{
    // std::cout << "Generating text texture for '" << text << "' in font <'" << font << "'>\n";
    destroyFont();

    try {
        this->loadFont();
        this->generateTxtTexture(renderer);
    } catch (...) {
        std::cout << "EXCEPTION:\tUnable to load text font on the fly.\n";
        return 0;
    }

    return 1;
}

void Text::destroyFont() {
    if(!txt_font) return;
    try {
        TTF_CloseFont(txt_font);
    } catch (...) {
        std::cout << "EXCEPTION:\tUnable to close font...\n";
    }
    txt_font = nullptr;
}

void Text::destroyTxtTexture()
{
    if(!txt_texture) return;
    try {
        SDL_DestroyTexture(txt_texture);
    } catch (...) {
        std::cout << "EXCEPTION:\tUnable to destroy text texture...\n";
    }
    txt_texture = nullptr;
}