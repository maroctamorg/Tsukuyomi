#include "text.hpp"

Text::Text(SDL_Renderer* renderer, const std::string text, const std::string font, int ptsize, const SDL_Colour color)
    : text(text), font(font), ptsize(ptsize), color(color) {
    if(this->loadFont())
        this->generateTxtTexture(renderer);

}

bool Text::isLoaded() {
    if(txt_texture)
        return true;
    return false;
}

bool Text::loadFont() {
    if(txt_font) {
        TTF_CloseFont(txt_font);
        txt_font = nullptr;
    }
    txt_font = TTF_OpenFont(font.c_str(), ptsize);
    if(!txt_font) {
        std::cout << "Invalid font being loaded! Yet to implement default...\n";
        txt_texture = nullptr;
        return false;
    }
    return true;
}

std::string Text::getString() {
    return this->text;
}

SDL_Point Text::getPos(SDL_Renderer* renderer, const SDL_Rect& target, ALIGN_X alignX, ALIGN_Y alignY) {
    if (!txt_texture)
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
    TTF_SizeText(txt_font, text.c_str(), &lw, &lh);
    *w = static_cast<int>(lw / length);
    *h = lh;
}

std::array<std::string,2> Text::split(int index) {
    if(index < 0 || index >= this->text.length() - 1)
        return std::array<std::string, 2> { text, "" };
    return std::array<std::string, 2> { text.substr(0, index + 1), text.substr(index + 1, text.length() - (index + 1)) };
}

void Text::updateTxt(SDL_Renderer* renderer, const std::string text) {
    this->text = text;
    if(this->loadFont())
        this->generateTxtTexture(renderer);
}

void Text::updateFontSize(SDL_Renderer *renderer, const int ptsize) {
    this->ptsize = ptsize;
    if(this->loadFont())
        this->generateTxtTexture(renderer);
}

void Text::display(SDL_Renderer* renderer, const SDL_Rect& target, ALIGN_X alignX, ALIGN_Y alignY) {
    // std::cout << "Target passed to text display: {" << target.x << ", " << target.y << ", " << target.w << ", " << target.h << "}\n";
    if (!txt_texture)
        return;
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
    if(txt_texture)
    {
        SDL_DestroyTexture(txt_texture);
        txt_texture = nullptr;
    }

    // std::cout << "Call to generateTxtTexture.\n";
    if (!txt_font) {
        std::cout << "Unable to load font.\n";
        txt_texture = nullptr;
    } else {
        SDL_Surface *surface { TTF_RenderText_Blended(txt_font, text.c_str(), color) };
        txt_texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FreeSurface(surface);
    }

    if(txt_texture) {
        // std::cout << "############\tTexture loaded successfully!\t############\n";
        return 1;
    }
    else { return 0; }
}

void Text::destroyTxtTexture()
{
    if(txt_texture != NULL && txt_texture != nullptr)
    {
        SDL_DestroyTexture(txt_texture);
        txt_texture = NULL;
    }
}

Text::~Text() {
    if(txt_font)
        TTF_CloseFont(txt_font);
    destroyTxtTexture();
    txt_font = nullptr;
    txt_texture = nullptr;
}