/* dq_sdl.c */

#include "dq_sdl.h"

int init_sdl()
{
    // Set vars to something
    gRenderer           = NULL;
    gWindow             = NULL;
    gFont               = NULL;
    gTextFont           = NULL;
    gGameController     = NULL;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
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

    /* init joystick */
    if(SDL_NumJoysticks() < 1)
    {
        printf("Warning: no joysticks detected.\n");
    }
    else
    {
        /* Load joystick */
        gGameController = SDL_JoystickOpen(0);
        if(gGameController == NULL)
        {
            printf("Warning: Unable to open game controller! SDL Error: %S\n", SDL_GetError());
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

void controls_handle_input(void)
{
    /* Buffer to slow down keypresses? */
    if(key_wait_buffer < 5)
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
            // This should probably confirm that the user wants to exit
            quit = true;
        }
    }

    /* Reset the key wait buffer if we got valid input */
    if(event.type == SDL_TEXTINPUT ||
       event.type == SDL_JOYAXISMOTION ||
       event.type == SDL_JOYBUTTONDOWN)
    {
        key_wait_buffer = 0;
    }

    /* Joystick/Gamepad buttons */
    if(event.type == SDL_JOYBUTTONDOWN)
    {
        /* Buttons on controller 0 */
        if(event.jbutton.which == 0)
        {
            /*
                Buffalo SNES pad mapping:
                    A           = 0
                    B           = 1
                    X           = 2
                    Y           = 3
                    L Shoulder  = 4
                    R Shoulder  = 5
                    Select      = 6
                    Start       = 7
            */
            // printf("Gamepad Button: %d\n", event.jbutton.button);
            if(event.jbutton.button == 0) gamepad_b0();
            if(event.jbutton.button == 1) gamepad_b1();
            if(event.jbutton.button == 2) gamepad_b2();
            if(event.jbutton.button == 3) gamepad_b3();
            if(event.jbutton.button == 4) gamepad_b4();
            if(event.jbutton.button == 5) gamepad_b5();
            if(event.jbutton.button == 6) gamepad_b6();
            if(event.jbutton.button == 7) gamepad_b7();
        }
    }

    /* Joystick/Gamepad x/y axes */
    if(event.type == SDL_JOYAXISMOTION)
    {
        /* Motion on controller 0 */
        if(event.jaxis.which == 0)
        {
            /* x axis */
            if(event.jaxis.axis == 0)
            {
                if(event.jaxis.value < 0) gamepad_left();
                if(event.jaxis.value > 0) gamepad_right();
            }

            /* y axis */
            if(event.jaxis.axis == 1)
            {
                if(event.jaxis.value < 0) gamepad_up();
                if(event.jaxis.value > 0) gamepad_down();
            }
        }
    }

    /* Get mouse and keyboard state */
    SDL_GetMouseState(&m_x, &m_y);
    keystates = SDL_GetKeyboardState(NULL);
    //printf("MouseX: %d\tMouseY: %d\n\n", m_x, m_y);

    /*
        Even though we'd prefer gamepad as a primary, we still
        have these keyboard keys to fall back on. These might
        also be useful for grabbing characters straight from
        the keyboard for things like naming.

        TODO: Add non-alphas and function keys?
    */

    /* Alpha-numerics */
    if(keystates[SDL_SCANCODE_A] == 1) key_press_a();
    if(keystates[SDL_SCANCODE_B] == 1) key_press_b();
    if(keystates[SDL_SCANCODE_C] == 1) key_press_c();
    if(keystates[SDL_SCANCODE_D] == 1) key_press_d();
    if(keystates[SDL_SCANCODE_E] == 1) key_press_e();
    if(keystates[SDL_SCANCODE_F] == 1) key_press_f();
    if(keystates[SDL_SCANCODE_G] == 1) key_press_g();
    if(keystates[SDL_SCANCODE_H] == 1) key_press_h();
    if(keystates[SDL_SCANCODE_I] == 1) key_press_i();
    if(keystates[SDL_SCANCODE_J] == 1) key_press_j();
    if(keystates[SDL_SCANCODE_K] == 1) key_press_k();
    if(keystates[SDL_SCANCODE_L] == 1) key_press_l();
    if(keystates[SDL_SCANCODE_M] == 1) key_press_m();
    if(keystates[SDL_SCANCODE_N] == 1) key_press_n();
    if(keystates[SDL_SCANCODE_O] == 1) key_press_o();
    if(keystates[SDL_SCANCODE_P] == 1) key_press_p();
    if(keystates[SDL_SCANCODE_Q] == 1) key_press_q();
    if(keystates[SDL_SCANCODE_R] == 1) key_press_r();
    if(keystates[SDL_SCANCODE_S] == 1) key_press_s();
    if(keystates[SDL_SCANCODE_T] == 1) key_press_t();
    if(keystates[SDL_SCANCODE_U] == 1) key_press_u();
    if(keystates[SDL_SCANCODE_V] == 1) key_press_v();
    if(keystates[SDL_SCANCODE_W] == 1) key_press_w();
    if(keystates[SDL_SCANCODE_X] == 1) key_press_x();
    if(keystates[SDL_SCANCODE_Y] == 1) key_press_y();
    if(keystates[SDL_SCANCODE_Z] == 1) key_press_z();
    if(keystates[SDL_SCANCODE_0] == 1) key_press_0();
    if(keystates[SDL_SCANCODE_1] == 1) key_press_1();
    if(keystates[SDL_SCANCODE_2] == 1) key_press_2();
    if(keystates[SDL_SCANCODE_3] == 1) key_press_3();
    if(keystates[SDL_SCANCODE_4] == 1) key_press_4();
    if(keystates[SDL_SCANCODE_5] == 1) key_press_5();
    if(keystates[SDL_SCANCODE_6] == 1) key_press_6();
    if(keystates[SDL_SCANCODE_7] == 1) key_press_7();
    if(keystates[SDL_SCANCODE_8] == 1) key_press_8();
    if(keystates[SDL_SCANCODE_9] == 1) key_press_9();

    /* Punctuation/Operation */
    if(keystates[SDL_SCANCODE_APOSTROPHE] == 1)     key_press_apostrophe();
    if(keystates[SDL_SCANCODE_BACKSLASH] == 1)      key_press_backslash();
    if(keystates[SDL_SCANCODE_COMMA] == 1)          key_press_comma();
    if(keystates[SDL_SCANCODE_EQUALS] == 1)         key_press_equals();
    if(keystates[SDL_SCANCODE_GRAVE] == 1)          key_press_grave();
    if(keystates[SDL_SCANCODE_PERIOD] == 1)         key_press_period();
    if(keystates[SDL_SCANCODE_MINUS] == 1)          key_press_minus();
    if(keystates[SDL_SCANCODE_LEFTBRACKET] == 1)    key_press_lbracket();
    if(keystates[SDL_SCANCODE_RIGHTBRACKET] == 1)   key_press_rightbracket();
    if(keystates[SDL_SCANCODE_SEMICOLON] == 1)      key_press_semicolon();
    if(keystates[SDL_SCANCODE_SLASH] == 1)          key_press_slash();
    if(keystates[SDL_SCANCODE_SPACE] == 1)          key_press_space();

    /* Keypad */
    if(keystates[SDL_SCANCODE_KP_0] == 1)           key_press_kp_0();
    if(keystates[SDL_SCANCODE_KP_1] == 1)           key_press_kp_1();
    if(keystates[SDL_SCANCODE_KP_2] == 1)           key_press_kp_2();
    if(keystates[SDL_SCANCODE_KP_3] == 1)           key_press_kp_3();
    if(keystates[SDL_SCANCODE_KP_4] == 1)           key_press_kp_4();
    if(keystates[SDL_SCANCODE_KP_5] == 1)           key_press_kp_5();
    if(keystates[SDL_SCANCODE_KP_6] == 1)           key_press_kp_6();
    if(keystates[SDL_SCANCODE_KP_7] == 1)           key_press_kp_7();
    if(keystates[SDL_SCANCODE_KP_8] == 1)           key_press_kp_8();
    if(keystates[SDL_SCANCODE_KP_9] == 1)           key_press_kp_9();
    if(keystates[SDL_SCANCODE_KP_DIVIDE] == 1)      key_press_kp_divide();
    if(keystates[SDL_SCANCODE_KP_MINUS] == 1)       key_press_kp_minus();
    if(keystates[SDL_SCANCODE_KP_MULTIPLY] == 1)    key_press_kp_multiply();
    if(keystates[SDL_SCANCODE_KP_PLUS] == 1)        key_press_kp_plus();
    if(keystates[SDL_SCANCODE_KP_ENTER] == 1)       key_press_kp_enter();
    if(keystates[SDL_SCANCODE_KP_DECIMAL] == 1)     key_press_kp_decimal();

    /* Function keys */
    if(keystates[SDL_SCANCODE_F1] == 1)     key_press_f1();
    if(keystates[SDL_SCANCODE_F2] == 1)     key_press_f2();
    if(keystates[SDL_SCANCODE_F3] == 1)     key_press_f3();
    if(keystates[SDL_SCANCODE_F4] == 1)     key_press_f4();
    if(keystates[SDL_SCANCODE_F5] == 1)     key_press_f5();
    if(keystates[SDL_SCANCODE_F6] == 1)     key_press_f6();
    if(keystates[SDL_SCANCODE_F7] == 1)     key_press_f7();
    if(keystates[SDL_SCANCODE_F8] == 1)     key_press_f8();
    if(keystates[SDL_SCANCODE_F9] == 1)     key_press_f9();
    if(keystates[SDL_SCANCODE_F10] == 1)    key_press_f10();
    if(keystates[SDL_SCANCODE_F11] == 1)    key_press_f11();
    if(keystates[SDL_SCANCODE_F12] == 1)    key_press_f12();

    /* Other */
    if(keystates[SDL_SCANCODE_DOWN] == 1)           key_press_down();
    if(keystates[SDL_SCANCODE_UP] == 1)             key_press_up();
    if(keystates[SDL_SCANCODE_LEFT] == 1)           key_press_left();
    if(keystates[SDL_SCANCODE_RIGHT] == 1)          key_press_right();
    if(keystates[SDL_SCANCODE_APPLICATION] == 1)    key_press_application();
    if(keystates[SDL_SCANCODE_BACKSPACE] == 1)      key_press_backspace();
    if(keystates[SDL_SCANCODE_CAPSLOCK] == 1)       key_press_capslock();
    if(keystates[SDL_SCANCODE_END] == 1)            key_press_end();
    if(keystates[SDL_SCANCODE_ESCAPE] == 1)         key_press_escape();
    if(keystates[SDL_SCANCODE_HOME] == 1)           key_press_home();
    if(keystates[SDL_SCANCODE_INSERT] == 1)         key_press_insert();
    if(keystates[SDL_SCANCODE_LALT] == 1)           key_press_lalt();
    if(keystates[SDL_SCANCODE_LCTRL] == 1)          key_press_lctrl();
    if(keystates[SDL_SCANCODE_LSHIFT] == 1)         key_press_lshift();
    if(keystates[SDL_SCANCODE_NUMLOCKCLEAR] == 1)   key_press_numlock();
    if(keystates[SDL_SCANCODE_PAGEDOWN] == 1)       key_press_pgdown();
    if(keystates[SDL_SCANCODE_PAGEUP] == 1)         key_press_pgup();
    if(keystates[SDL_SCANCODE_PAUSE] == 1)          key_press_pause();
    if(keystates[SDL_SCANCODE_PRINTSCREEN] == 1)    key_press_prtscrn();
    if(keystates[SDL_SCANCODE_RALT] == 1)           key_press_ralt();
    if(keystates[SDL_SCANCODE_RCTRL] == 1)          key_press_rctrl();
    if(keystates[SDL_SCANCODE_RETURN] == 1)         key_press_return();
    if(keystates[SDL_SCANCODE_RETURN2] == 1)        key_press_return2();
    if(keystates[SDL_SCANCODE_RSHIFT] == 1)         key_press_rshift();
    if(keystates[SDL_SCANCODE_SCROLLLOCK] == 1)     key_press_scrllck();    
    if(keystates[SDL_SCANCODE_TAB] == 1)            key_press_tab();
}