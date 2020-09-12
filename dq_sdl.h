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
TTF_Font*       gFont;
SDL_Event       event;
TTF_Font*       gTextFont;
const Uint8*    keystates;
int             m_x;
int             m_y;
int             hero_move;

/* Function pointers */
void (*key_a)(void);
void (*key_b)(void);
void (*key_c)(void);
void (*key_d)(void);
void (*key_e)(void);
void (*key_f)(void);
void (*key_g)(void);
void (*key_h)(void);
void (*key_i)(void);
void (*key_j)(void);
void (*key_q)(void);
void (*key_s)(void);
void (*key_w)(void);
void (*key_x)(void);

/* Area render control */
int tile_size;
int tile_cols;
int grid_rows;
int grid_cols;

/* Game control */
bool quit;
bool in_menu;
int key_wait_buffer;

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