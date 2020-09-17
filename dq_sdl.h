/* dq_sdl.h */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

unsigned int SCREEN_WIDTH;
unsigned int SCREEN_HEIGHT;

/* Global SDL assets */
SDL_Window*     gWindow;
SDL_Renderer*   gRenderer;

/* Specific SDL assest */
SDL_Surface*    intro_image;
SDL_Texture*    intro_texture;
SDL_Joystick*   gGameController;
TTF_Font*       gFont;
SDL_Event       event;
TTF_Font*       gTextFont;
const Uint8*    keystates;
int             m_x;
int             m_y;

/* Keyboard alpha-numeric function pointers */
void (*kb_fp[512])(void);
void (*gp_bt[32])(void);
void (*gamepad_up)(void);
void (*gamepad_down)(void);
void (*gamepad_left)(void);
void (*gamepad_right)(void);

/* Game control */
bool quit;
bool in_menu;
int key_wait_buffer;

int tile_cols;
int grid_rows;
int grid_cols;

extern int init_sdl(void);

extern void event_sdl(SDL_Event* e);
extern void close_sdl(void);
extern void event_handler(void);
extern void draw_window(void);
extern void draw_local_area(int row_, int col_, int tex_);
extern void video_set_tile_texture(int tex_);
extern void video_set_actor_texture(int tex_);
extern void video_draw_menu_window(SDL_Rect* menu_rect);
extern void video_draw_text(char message[15], int x, int y);

/* Controls */
extern void controls_handle_input(void);
extern void controls_load_controls(char ctrl_conf[15]);
extern void controls_action_unbound(void);
extern void controls_action_select(void);
extern void controls_action_cancel(void);
extern void controls_action_interact(void);
extern void controls_action_right(void);
extern void controls_action_left(void);
extern void controls_action_down(void);
extern void controls_action_up(void);