#include "graphics_context.hpp"

void GraphicsContext::Init_SDL()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        throw(SDL_GetError());
    }
    else
    {
        // printf("Succesfully initialised SDL!\n");
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        this->window = SDL_CreateWindow("PONG++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->W_W, this->W_H, 0 | SDL_WINDOW_RESIZABLE);
        if( window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            throw(SDL_GetError());
        }
        else
        {
            //Create renderer for window
            this->renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if( renderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                throw(SDL_GetError());
            }
            else
            {
                // printf("Succesfully initialised window and renderer!\n");
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                //Initialize renderer color
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    throw(IMG_GetError());
                }
            }
        }

        SDL_RenderClear(this->renderer);
        SDL_RenderPresent(this->renderer);
    }

    //Enable text input
    SDL_StartTextInput();

    //Initialize TTF and MIXER
    TTF_Init();
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

    // Initializing AUDIO subsystem
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    Mix_AllocateChannels(1);
    Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

    // return (window && renderer);
}

void GraphicsContext::Quit_SDL()
{
    //Destroy renderer and window
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    window = nullptr;
    renderer = nullptr;

    //Disable text input
    SDL_StopTextInput();

    //Quit SDL subsystems
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int GraphicsContext::getWidth() { return W_W; }
int GraphicsContext::getHeight() { return W_H; }

void GraphicsContext::display() {
    SDL_RenderPresent(this->renderer);
}

void GraphicsContext::updateWindowSize(int w, int h) {
    W_W = w;
    W_H = h;

    // SDL_SetWindowSize(window, W_W, W_H);
}