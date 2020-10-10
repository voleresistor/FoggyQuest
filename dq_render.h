/*
    dq_render.h

    A service for handling game drawing.
*/

#include <stdbool.h>
#include <SDL2/SDL.h>

#ifndef DQ_RENDER__
#define DQ_RENDER__

struct Locator;
struct LogService;

struct RenderService {
    // void (*reset_move)(void);
    void (*draw_window)(void);
    void (*draw_local_area)(int row_, int col_, int tex_);
    void (*set_texture)(int tex_);
    void (*draw_menu_window)(SDL_Rect* menu_rect);
    void (*draw_text)(char message[15], int x, int y);
};

/* Private service pointer */
static struct RenderService* system_renderer;

/* Public Constructor */
extern void new_render(void);

/* Private render functions */
static void draw_window(void);
static void draw_local_area(int row_, int col_, int tex_);
static void set_texture(int tex_);
static void draw_menu_window(SDL_Rect* menu_rect);
static void draw_text(char message[15], int x, int y);

/* Internal functions */

#endif