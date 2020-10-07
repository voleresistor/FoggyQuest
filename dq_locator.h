/*
    dq_locator.h

    A service locator for the DQ remake in C.
*/

#ifndef DQ_LOCATOR__
#define DQ_LOCATOR__

#define this_func __func__

struct LogService;
struct ControlService;
// struct RenderService;
// struct AudioService;

/*
    The Locator struct carries only function pointers
    to static functions. The data resides within the
    module, accessible only from one of these function
    pointers.
*/
struct Locator {
    /* Providers */
    void (*provide_log)(struct LogService* log);
    void (*provide_control)(struct ControlService* control);
    // void (*provide_render)(struct RenderService* render);
    // void (*provide_audio)(struct AudioService* audio);

    /* Access */
    struct LogService* (*get_log)(void);
    struct ControlService* (*get_control)(void);
    // struct RenderService* (*get_render)(void);
    // struct AudioService* (*get_service)(void);
};

struct Locator* system_locator;

/* Services */
static struct LogService* log_service;
static struct ControlService* control_service;
// static struct RenderService* control_service;
// static struct AudioService* audio_service;

/*
    Creates the locator service one time during initialization
*/
extern void new_locator(void);

/* Log */
static struct LogService* get_log(void);
static void provide_log(struct LogService* log);

/* Control */
static struct ControlService* get_control(void);
static void provide_control(struct ControlService* control);

// /* Render */
// static struct RenderService* get_render(void);
// static void provide_render(struct RenderService* render);

// /* Audio */
// static struct AudioService* get_audio(void);
// static  void provide_audio(struct AudioService* audio);

#endif