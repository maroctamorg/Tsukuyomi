#include "button.hpp"

//create overloaded version of button constructor and assignor that does not take in a text
Button::Button(char* imgPath, std::string text, std::string font, int ptsize, const SDL_Color *font_color, const SDL_Rect *target_rect, const SDL_Rect *rect, const SDL_Color *color, bool is_active, int id, SDL_Renderer *renderer)
    :  b_txt { text, font, ptsize, font_color, target_rect, color, false, renderer }, b_rect { rect->x, rect->y, rect->w, rect->h }, b_color { color->r, color->g, color->b, color->a }, b_state { is_active }, b_id { id }, initialized {true}
{
    if (!imgPath)
    {
        //std::cout << "Call to generateBttTexture with argument: " << imgPath << '\n'; 
        int control = generateBttTexture(imgPath, renderer);
        //std::cout << "Returned from generateBttTexture with flag " << control << '\n';
    }
}


void Button::assign(char* imgPath, std::string text, std::string font, int ptsize, const SDL_Color *font_color, const SDL_Rect *target_rect, const SDL_Rect *rect, const SDL_Color *color, bool is_active, int id, SDL_Renderer *renderer)
{
    //std::cout << "Call to button.assign().\n";

    b_txt.assign(text, font, ptsize, font_color, target_rect, color, false, renderer);
    b_rect = *rect;
    b_color = *color;
    b_state = { is_active, false };
    b_id = id;
    initialized = true;

    if (imgPath)
    {
        //std::cout << "Call to generateBttTexture with argument: " << imgPath << '\n'; 
        int control = generateBttTexture(imgPath, renderer);
        //std::cout << "Returned from generateBttTexture with flag " << control << '\n';
    } else {
        //std::cout << "No imgPath provided for button Texture, call to generateBttTexture skipped.\n";
    }
}


void Button::activate()     {   b_state.active  =   true;   }
Button* Button::press()        {   b_state.pressed =   true; return this;  }
void Button::deactivate()   {   b_state.active  =   false;  }


bool Button::isActive()     {   return b_state.active;  }
bool Button::isPressed()    {   return b_state.pressed; }

bool Button::Clicked(const SDL_Point &cursor_pos)
{
    //std::cout << "Call to Button.Clicked.\n";
    bool control { false };

    //YET TO BE IMPLEMENTED - ??? isn't it already implemented ?!?
    if (isContained(cursor_pos, b_rect))
    {
        control = true;
    }

    return control;
}

int Button::generateBttTexture(char *imgPath, SDL_Renderer *renderer)
{
    if(b_texture)
    {
        SDL_DestroyTexture(b_texture);
        b_texture = NULL;
    }

    SDL_Surface *surface { SDL_LoadBMP(imgPath) };
    b_texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if(b_texture) { return 1; }
    else { return 0; }
}

void Button::display(SDL_Renderer *renderer)
{   
    SDL_SetRenderDrawColor(renderer, b_color.r, b_color.g, b_color.b, b_color.a);
    SDL_RenderFillRect(renderer, &b_rect);

    b_txt.displayText(renderer);
}

void Button::destroyTexture() {
    if(b_texture != NULL && b_texture != nullptr)
    {
        SDL_DestroyTexture(b_texture);
        b_texture = NULL;
    }
    b_txt.destroyTxtTexture();
};

Button::~Button() {
    destroyTexture();
}
