/* dq_controls.c */

#include <stdlib.h>
#include <stdio.h>
// #include <SDL2/SDL.h>

#include "dq_controls.h"
#include "dq_locator.h"
#include "dq_log.h"
#include "dq_sdl.h"

/* Public constructor */
void new_controls(void)
{
    /*
        Only initiate if not already done.
        
        This prevents overwriting an existing service.
    */
    if(system_controls != NULL)
    {
        return;
    }

    system_controls = malloc(sizeof(struct ControlService));
    system_controls->reset_move = &reset_move;
    system_controls->get_move = &get_move;
    system_controls->handle_input = &handle_input;
    system_controls->load_controls = &load_controls;

    /* Register with locator */
    system_locator->provide_control(system_controls);
}

/*
    Service provider functions
*/
static void reset_move(void)
{
    hero_move = 0;
}

static int get_move(void)
{
    return hero_move;
}

static void handle_input(void)
{
    char log_msg[255];

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

    /* Get mouse and keyboard state */
    SDL_GetMouseState(&m_x, &m_y);
    keystates = SDL_GetKeyboardState(NULL);
    //printf("MouseX: %d\tMouseY: %d\n\n", m_x, m_y);

    /* Joystick/Gamepad x/y axes */
    if(event.type == SDL_JOYAXISMOTION)
    {
        /* Motion on controller 0 */
        if(event.jaxis.which == 0)
        {
            /* x axis */
            if(event.jaxis.axis == 0)
            {
                sprintf(log_msg, "Gamepad/Joystick [%d] axis: (%d)%d\0", event.jaxis.which, event.jaxis.axis, event.jaxis.value);
                system_locator->get_log()->write_log(log_msg, this_func, DQDEBUG);
                if(event.jaxis.value < 0) gamepad_left();
                if(event.jaxis.value > 0) gamepad_right();
            }

            /* y axis */
            if(event.jaxis.axis == 1)
            {
                sprintf(log_msg, "Gamepad/Joystick [%d] axis: (%d)%d\0", event.jaxis.which, event.jaxis.axis, event.jaxis.value);
                system_locator->get_log()->write_log(log_msg, this_func, DQDEBUG);
                if(event.jaxis.value < 0) gamepad_up();
                if(event.jaxis.value > 0) gamepad_down();
            }
        }
    }

    /* Joystick/Gamepad buttons */
    if(event.type == SDL_JOYBUTTONDOWN)
    {
        /* Buttons on controller 0 */
        if(event.jbutton.which == 0)
        {
            // printf("Gamepad Button: %d\n", event.jbutton.button);
            sprintf(log_msg, "Gamepad/Joystick [%d] button: %d\0", event.jbutton.which, event.jbutton.button);
            system_locator->get_log()->write_log(log_msg, this_func, DQDEBUG);
            gp_bt[event.jbutton.button]();
        }
    }

    /*
        Even though we'd prefer gamepad as a primary, we still
        have these keyboard keys to fall back on. These might
        also be useful for grabbing characters straight from
        the keyboard for things like naming.
    */
    if(event.type == SDL_TEXTINPUT)
    {
        // printf("Keysym: %d\n", event.key.keysym.scancode);
        sprintf(log_msg, "Keyboard key: %d\0", event.key.keysym.scancode);
        system_locator->get_log()->write_log(log_msg, this_func, DQDEBUG);
        kb_fp[event.key.keysym.scancode]();
    }
}

static void load_controls(char ctrl_conf[15])
{
    char log_msg[255];
    sprintf(log_msg, "Begin load controls.");
    system_locator->get_log()->write_log(log_msg, this_func, DQINFO);

    if(ctrl_conf == NULL)
    {
        /* Define game control function pointers */
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
        sprintf(log_msg, "No control file provided. Using defaults.");
        system_locator->get_log()->write_log(log_msg, this_func, DQWARNING);

        /* Set all gamepad buttons to unbound */
        for(int i = 0; i < 32; i++)
        {
            gp_bt[i] = &action_unbound;
        }

        sprintf(log_msg, "Gamepad buttons defaulted.");
        system_locator->get_log()->write_log(log_msg, this_func, DQDEBUG);

        /* Bind gamepad/joystick buttons */
        gamepad_down    = &action_down;
        gamepad_up      = &action_up;
        gamepad_left    = &action_left;
        gamepad_right   = &action_right;
        gp_bt[0]        = &action_select;
        gp_bt[1]        = &action_cancel;
        gp_bt[2]        = &action_interact;
        gp_bt[3]        = &action_interact;

        sprintf(log_msg, "Gamepad defaults loaded.");
        system_locator->get_log()->write_log(log_msg, this_func, DQINFO);

        /* Set all kb keys to unbound */
        for(int i = 0; i < 512; i++)
        {
            kb_fp[i] = &action_unbound;
        }

        sprintf(log_msg, "Keyboard controls defaulted.");
        system_locator->get_log()->write_log(log_msg, this_func, DQDEBUG);

        /* Bind default keys */
        kb_fp[SDLK_a] = &action_left;
        kb_fp[SDLK_s] = &action_down;
        kb_fp[SDLK_d] = &action_right;
        kb_fp[SDLK_w] = &action_up;
        kb_fp[SDLK_c] = &action_cancel;
        kb_fp[SDLK_q] = &action_interact;
        kb_fp[SDLK_e] = &action_select;

        sprintf(log_msg, "Keyboard defaults loaded.");
        system_locator->get_log()->write_log(log_msg, this_func, DQINFO);
    }
    else
    {
        // Read in control mapping from file
        sprintf(log_msg, "Reading in control mapping from %s.", ctrl_conf);
        system_locator->get_log()->write_log(log_msg, this_func, DQINFO);
    }

    sprintf(log_msg, "Done loading controls.");
    system_locator->get_log()->write_log(log_msg, this_func, DQINFO);
}

/*
    Private functions
*/
static void action_unbound(void)
{
    return;
}

static void action_select(void)
{
    if(!in_menu)
    {
        in_menu = true;
    }
}

static void action_cancel(void)
{
    if(in_menu)
    {
        in_menu = false;
    }
}

static void action_interact(void)
{
    printf("action_interact()\n");
}

static void action_left(void)
{
    if(in_menu)
    {

    }
    else
    {
        hero_move = 4;
    }
}

static void action_right(void)
{
    if(in_menu)
    {

    }
    else
    {
        hero_move = 2;
    }
}

static void action_down(void)
{
    if(in_menu)
    {

    }
    else
    {
        hero_move = 3;
    }
}

static void action_up(void)
{
    if(in_menu)
    {

    }
    else
    {
        hero_move = 1;
    }
}











