#include "text.hpp"

Text::Text(const std::string text, const std::string font, int ptsize, const SDL_Colour *color, const SDL_Rect *target_rect, const SDL_Colour *rect_colour, bool display_rect, SDL_Renderer *renderer)
{
    this->text = text;
    this->font = font;
    this->ptsize = ptsize;
    this->color = *color;
    this->target_rect = *target_rect;
    this->rect_colour = *rect_colour;
    this->display_rect = display_rect;

    initialized = true;

    int control = this->generateTxtTexture(renderer);
    //std::cout << "Returned from generateTxtTexture with flag " << control << '\n';
}

void Text::assign(const std::string text, const std::string font, int ptsize, const SDL_Colour *color, const SDL_Rect *target_rect, const SDL_Colour *rect_colour, bool display_rect, SDL_Renderer *renderer)
{
    //std::cout << "Call to text.assign().\n";
    this->text = text;
    this->font = font;
    this->ptsize = ptsize;
    this->color = *color;
    this->target_rect = *target_rect;
    this->rect_colour = *rect_colour;
    this->display_rect = display_rect;

    initialized = true;

    int control = this->generateTxtTexture(renderer);
    //std::cout << "Returned from generateTxtTexture with flag " << control << '\n';

}

void Text::assignTxt(const std::string text, SDL_Renderer *renderer){
    this->text = text;
    int control = this->generateTxtTexture(renderer);
    //std::cout << "Returned from generateTxtTexture with flag " << control << '\n';
}

bool Text::isInitialized() {
    return initialized;
}

int Text::generateTxtTexture(SDL_Renderer *renderer)
{
    if(txtTexture)
    {
        SDL_DestroyTexture(txtTexture);
        txtTexture = nullptr;
    }

    //std::cout << "Call to generateTxtTexture.\n";
    TTF_Font *txt_font { TTF_OpenFont(font.c_str(), ptsize) };
    if (!txt_font) {
        //std::cout << "Unable to load font.\n";
        txtTexture = nullptr;
    } else {
        SDL_Surface *surface { TTF_RenderText_Blended(txt_font, text.c_str(), color) };
        txtTexture = SDL_CreateTextureFromSurface(renderer, surface);

        TTF_CloseFont(txt_font);
        SDL_FreeSurface(surface);
    }

    if(txtTexture) { return 1; }
    else { return 0; }
}

void Text::displayText(SDL_Renderer *renderer)
{
    if (txtTexture != NULL && txtTexture != nullptr)
        {
            int texW = 0;
            int texH = 0;
            SDL_QueryTexture(txtTexture, NULL, NULL, &texW, &texH);
            SDL_Rect dstrect = { target_rect.x + static_cast<int>(target_rect.w/10), target_rect.y + static_cast<int>(target_rect.h/8), texW, texH };

            if(display_rect) {
                SDL_SetRenderDrawColor(renderer, rect_colour.r, rect_colour.g, rect_colour.b, rect_colour.a);
                SDL_RenderFillRect(renderer, &target_rect);
            }

            SDL_RenderCopy(renderer, txtTexture, NULL, &dstrect);
        } else { //std::cout << "Text texture noninitialised: " << text << '\n';
        }
}

void Text::destroyTxtTexture()
{
    if(txtTexture != NULL && txtTexture != nullptr)
    {
        SDL_DestroyTexture(txtTexture);
        txtTexture = NULL;
    }
}

Text::~Text() {
    destroyTxtTexture();
}