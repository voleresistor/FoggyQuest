/* dq_sdl.c */

#include "dq_sdl.h"

int init_sdl()
{
    // Set vars to something
    gRenderer   = NULL;
    gWindow     = NULL;
    gFont       = NULL;
    gTextFont   = NULL;

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
    gTextFont = TTF_OpenFont("font/SourceCodePro-Bold.otf", 32);

    /* SDL blend settings */
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    // Initialize SDL mixer
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0 )
    {
        return false;
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

void controls_handle_input(void)
{
    /* Buffer to slow down keypresses? */
    if(key_wait_buffer < 6)
    {
        key_wait_buffer++;
        return;
    }

    /*
        Poll events into the appropriate buffers
    */
    while(SDL_PollEvent(&event) != 0)
    {
        /* Click X on the window */
        if(event.type == SDL_QUIT)
        {
            quit = true;
        }
    }

    if(event.key.type == SDL_TEXTINPUT)
    {
        // printf("SDL_TEXTINPUT\n");
        key_wait_buffer = 0;
    }

    /* Handle mouse and keyboard input */
    SDL_GetMouseState(&m_x, &m_y);
    keystates = SDL_GetKeyboardState(NULL);
    //printf("MouseX: %d\tMouseY: %d\n\n", m_x, m_y);

    if(keystates[SDL_SCANCODE_A] == 1) key_a();
    if(keystates[SDL_SCANCODE_B] == 1) key_b();
    if(keystates[SDL_SCANCODE_C] == 1) key_c();
    if(keystates[SDL_SCANCODE_D] == 1) key_d();
    if(keystates[SDL_SCANCODE_E] == 1) key_e();
    if(keystates[SDL_SCANCODE_F] == 1) key_f();
    if(keystates[SDL_SCANCODE_G] == 1) key_g();
    if(keystates[SDL_SCANCODE_H] == 1) key_h();
    if(keystates[SDL_SCANCODE_I] == 1) key_i();
    if(keystates[SDL_SCANCODE_Q] == 1) key_q();
    if(keystates[SDL_SCANCODE_S] == 1) key_s();
    if(keystates[SDL_SCANCODE_W] == 1) key_w();
    if(keystates[SDL_SCANCODE_X] == 1) key_x();
}