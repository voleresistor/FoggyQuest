/* dq_sdl.c */

#include "dq_sdl.h"

int init_sdl()
{
    // Set vars to something
    gRenderer   = NULL;
    gWindow     = NULL;
    gFont       = NULL;
    gCant       = NULL;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL! Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create window
    gWindow = SDL_CreateWindow("SDL Dragon Quest", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL)
    {
        printf("Error creating SDL window! Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL)
    {
        printf("Error creating SDL renderer! Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    else
    {
        // Set background
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0x0);
    }

    /* Initialize PNG loading */
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("Unable to init SDL_Image. Error: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }

    /* Initialize TTF fonts */
    if(TTF_Init() == -1)
    {
        printf("Unable to init SDL_ttf. Error: %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    /* Menu and dialog font
        SourceCodePro-Bold.otf
        Cantarell-Bold.otf
    */
    gCant = TTF_OpenFont("font/SourceCodePro-Bold.otf", 32);

    /* SDL blend settings */
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    // Initialize SDL mixer
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0 )
    {
        return false;
    }

    // Set keyboard repeat

    return EXIT_SUCCESS;
}

void event_sdl(SDL_Event* e)
{
    // Clicking x
    if(e->type == SDL_QUIT)
    {
        quit = true;
    }

    /*
        TODO: eventually have this just return what buttons
        were pushed rather than directly changing game states.
    */
    if(e->type == SDL_KEYDOWN)
    {
        switch(e->key.keysym.sym)
        {
            case SDLK_q:
            quit = true;
            break;

            case SDLK_e:
            action_menu = true;
            break;

            case SDLK_x:
            action_menu = false;
            break;

            case SDLK_w:
            hero_move = 0;
            break;

            case SDLK_a:
            hero_move = 3;
            break;

            case SDLK_s:
            hero_move = 2;
            break;

            case SDLK_d:
            hero_move = 1;
            break;
        }
    }
}

void close_sdl()
{
    /* Free gloabl font */
    TTF_CloseFont(gFont);
    gFont = NULL;

    /* Free up window and renderer */
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    /* Free up music file */
    //Mix_FreeMusic(music);
    //music = NULL;

    /* Close out SDL subsystems */
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void event_handler()
{
    /*
        Handle events

        Even though I'm using KeyBoardState, this is still
        necessary to poll the mouse and keyboard state for
        SDL_GetKeyBoardState and SDL_GetMouseState.
    */
    while(SDL_PollEvent(&event) != 0)
    {
        event_sdl(&event);
    }

    /* Handle mouse and keyboard input */
    SDL_GetMouseState(&m_x, &m_y);
    //printf("MouseX: %d\tMouseY: %d\n\n", m_x, m_y);

    /* TODO: Handle this in a function */
    keystates = SDL_GetKeyboardState(NULL);
}

void draw_window(void)
{
    /*
        Reset renderer for this new frame
        
        Currently setting background color based on current map
        _map_height field where > 0 is sky blue, 0 is green grass
        and < 0 is black.
    */
    SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xFF);
    // if(_cur->_map_height > 0)
    //     SDL_SetRenderDrawColor(gRenderer, 0xDD, 0xFC, 0xFF, 0xFF);

    // if(_cur->_map_height == 0)
    //     SDL_SetRenderDrawColor(gRenderer, 0x0, 0xFF, 0x0, 0xFF);

    SDL_RenderClear(gRenderer);

    /* Build area grid */
    // draw_local_area(cur_);

    /* Present new frame */
    SDL_RenderPresent(gRenderer);
}

void draw_local_area(int row_, int col_, int tex_)
{
    for(int r = 0; r < grid_rows; r++)
    {
        for(int c = 0; c < grid_cols; c++)
        {

        }
    }
}

void video_set_tile_texture(int tex_)
{
    switch(tex_)
    {
        case 10:
        SDL_SetRenderDrawColor(gRenderer, 0xA8, 0xA8, 0xA8, 0xFF);
        break;

        case 13:
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0xFF, 0x0, 0xFF);
        break;

        case 24:
        SDL_SetRenderDrawColor(gRenderer, 0xAC, 0x35, 0x35, 0xFF);
        break;

        case 31:
        SDL_SetRenderDrawColor(gRenderer, 0x36, 0x36, 0x36, 0xFF);
        break;
    }
}

void video_set_actor_texture(int tex_)
{
    if(tex_ == 999)
    {
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0xFF, 0xFF);
    }
    else
    {
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0xFF, 0x32);
    }
}
