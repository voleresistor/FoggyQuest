/* dq_sdl.c */

#include "dq_sdl.h"
#include "dq_locator.h"
#include "dq_log.h"
#include "dq_hero.h"

int init_sdl()
{
    // Set vars to something
    gRenderer           = NULL;
    gWindow             = NULL;
    gFont               = NULL;
    gTextFont           = NULL;
    gGameController     = NULL;

    char log_msg[255];

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        sprintf(log_msg, "Couldn't initialize SDL! Error: %s\0", SDL_GetError());
        system_locator->get_log()->write_log(log_msg, this_func, DQERROR);
        return EXIT_FAILURE;
    }

    // Create window
    gWindow = SDL_CreateWindow("SDL Dragon Quest", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL)
    {
        sprintf(log_msg, "Error creating SDL window! Error: %s\0", SDL_GetError());
        system_locator->get_log()->write_log(log_msg, this_func, DQERROR);
        return EXIT_FAILURE;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL)
    {
        sprintf(log_msg, "Error creating SDL renderer! Error: %s\0", SDL_GetError());
        system_locator->get_log()->write_log(log_msg, this_func, DQERROR);
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
        sprintf(log_msg, "Unable to init SDL_Image. Error: %s\0", IMG_GetError());
        system_locator->get_log()->write_log(log_msg, this_func, DQERROR);
        return EXIT_FAILURE;
    }

    /* Initialize TTF fonts */
    if(TTF_Init() == -1)
    {
        sprintf(log_msg, "Unable to init SDL_ttf. Error: %s\0", TTF_GetError());
        system_locator->get_log()->write_log(log_msg, this_func, DQERROR);
        return EXIT_FAILURE;
    }

    /* Menu and dialog font
        SourceCodePro-Bold.otf
        Cantarell-Bold.otf
    */
    gTextFont = TTF_OpenFont("font/SourceCodePro-Bold.otf", 32);

    /* SDL blend settings */
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    // Initialize SDL mixer
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0 )
    {
        return false;
    }

    /* init joystick */
    if(SDL_NumJoysticks() < 1)
    {
        sprintf(log_msg, "No joysticks detected.\0");
        system_locator->get_log()->write_log(log_msg, this_func, DQWARNING);
    }
    else
    {
        /* Load joystick */
        gGameController = SDL_JoystickOpen(0);
        if(gGameController == NULL)
        {
            sprintf(log_msg, "Unable to open game controller! SDL Error: %S\0", SDL_GetError());
            system_locator->get_log()->write_log(log_msg, this_func, DQERROR);
        }
    }

    return EXIT_SUCCESS;
}

void event_sdl(SDL_Event* e)
{
    

    /*
        TODO: eventually have this just return what buttons
        were pushed rather than directly changing game states.
    */
    // if(e->type == SDL_KEYDOWN)
    // {
    //     switch(e->key.keysym.sym)
    //     {
    //         case SDLK_q:
    //         quit = true;
    //         break;

    //         case SDLK_e:
    //         action_menu = true;
    //         break;

    //         case SDLK_x:
    //         action_menu = false;
    //         break;

    //         case SDLK_w:
    //         hero_move = 0;
    //         break;

    //         case SDLK_a:
    //         hero_move = 3;
    //         break;

    //         case SDLK_s:
    //         hero_move = 2;
    //         break;

    //         case SDLK_d:
    //         hero_move = 1;
    //         break;
    //     }
    // }
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

    /* Free the joystick */
    SDL_JoystickClose(gGameController);
    gGameController = NULL;

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

void video_draw_menu_window(SDL_Rect* menu_rect)
{
    /* Render background with transparency because why not */
    SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xCC);
    SDL_RenderFillRect(gRenderer, menu_rect);
    SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xAA);
    SDL_RenderFillRect(gRenderer, menu_rect);
}

void video_draw_text(char message[15], int x, int y)
{
    SDL_Color textCol = {255, 255, 255};
    SDL_Surface* sfc_idle = TTF_RenderText_Solid(gTextFont, message, textCol);
    SDL_Texture* tex_idle = SDL_CreateTextureFromSurface(gRenderer, sfc_idle);

    /* Query that texture */
    int texW;
    int texH;
    SDL_QueryTexture(tex_idle, NULL, NULL, &texW, &texH);
    // printf("%d\n", texW);

    SDL_Rect rect_idle = {x, y, texW, texH};
    SDL_RenderCopy(gRenderer, tex_idle, NULL, &rect_idle);
    SDL_FreeSurface(sfc_idle);
    SDL_DestroyTexture(tex_idle);
}

