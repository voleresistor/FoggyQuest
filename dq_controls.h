/*
    dq_controls.h

    A service for handling player input.
*/

#include <stdbool.h>

#ifndef DQ_CONTROLS__
#define DQ_CONTROLS__

struct Locator;
struct LogService;

struct ControlService {
    void (*reset_move)(void);
    int (*get_move)(void);
    void (*handle_input)(void);
    void (*load_controls)(char ctrl_conf[15]);
};

/* Hero movement */
static int hero_move;
bool in_menu;
static int key_wait_buffer;

/* Private service pointer */
static struct ControlService* system_controls;

/* Keyboard alpha-numeric function pointers */
static void (*kb_fp[512])(void);
static void (*gp_bt[32])(void);
static void (*gamepad_up)(void);
static void (*gamepad_down)(void);
static void (*gamepad_left)(void);
static void (*gamepad_right)(void);

/* Public constructor */
extern void new_controls(void);

/* Private control functions */
static void handle_input(void);
static void load_controls(char ctrl_conf[15]);
static void reset_move(void);
static int get_move(void);

/* Internal functions */
static void action_unbound(void);
static void action_select(void);
static void action_cancel(void);
static void action_interact(void);
static void action_right(void);
static void action_left(void);
static void action_down(void);
static void action_up(void);

#endif