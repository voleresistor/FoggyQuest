/*
    dq_render.c

    Rendering service
*/

#include <stdlib.h>
#include <stdio.h>

#include "dq_renderer.h"
#include "dq_locator.h"
#include "dq_log.h"
#include "dq_sdl.h"

/* Public constructor */
void new_render(void)
{
    /*
        Only initiate if not already done.
        
        This prevents overwriting an existing service.
    */
    if(system_renderer != NULL)
    {
        return;
    }

    system_renderer = malloc(sizeof(struct RenderService));
    system_renderer->draw_window = &draw_window;
    system_renderer->draw_local_area = &draw_local_area;
    system_renderer->set_texture = &set_texture;
    system_renderer->draw_menu_window = &draw_menu_window;
    system_renderer->draw_text = &draw_text;

    /* Register with locator */
    system_locator->provide_renderer(system_renderer);
}