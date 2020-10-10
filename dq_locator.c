/*
    dq_locator.c

    A service locator for the DQ remake in C.
*/

#include <stdlib.h>

#include "dq_locator.h"
// #include "dq_log.h"
// #include "dq_control.h"
// #include "dq_render.h"
// #include "dq_audio.h"

extern void new_locator(void)
{
    /*
        Only initiate if not already done.
        
        This prevents overwriting an existing locator.
    */
    if(system_locator != NULL)
    {
        return;
    }

    system_locator = malloc(sizeof(struct Locator));
    /* Logging */
    system_locator->provide_log = &provide_log;
    system_locator->get_log = &get_log;
    log_service = NULL;

    /* Controls */
    system_locator->provide_control = &provide_control;
    system_locator->get_control = &get_control;
    control_service = NULL;

    /* Rendering */
    system_locator->provide_render = &provide_render;
    system_locator->get_render = &get_render;
    render_service = NULL;

    // /* Audio */
    // system_locator->provide_audio = &provide_audio;
    // system_locator->get_audio = &get_audio;
    // audio_service = NULL;
}

/* Logging */
static struct LogService* get_log(void)
{
    return log_service;
}

static void provide_log(struct LogService* log)
{
    log_service = log;
}

/* Control */
static struct ControlService* get_control(void)
{
    return control_service;
}

static void provide_control(struct ControlService* control)
{
    control_service = control;
}

// /* Render */
// static struct RenderService* get_render(void)
// {
//     return render_service;
// }

// static void provide_render(struct RenderService* render)
// {
//     render_service = render;
// }

// /* Audio */
// static struct AudioService* get_audio(void)
// {
//     return audio_service;
// }

// static  void provide_audio(struct AudioService* audio)
// {
//     audio_service = audio;
// }