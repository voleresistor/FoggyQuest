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
void (*key_press_a)(void);
void (*key_press_b)(void);
void (*key_press_c)(void);
void (*key_press_d)(void);
void (*key_press_e)(void);
void (*key_press_f)(void);
void (*key_press_g)(void);
void (*key_press_h)(void);
void (*key_press_i)(void);
void (*key_press_j)(void);
void (*key_press_k)(void);
void (*key_press_l)(void);
void (*key_press_m)(void);
void (*key_press_n)(void);
void (*key_press_o)(void);
void (*key_press_p)(void);
void (*key_press_q)(void);
void (*key_press_r)(void);
void (*key_press_s)(void);
void (*key_press_t)(void);
void (*key_press_u)(void);
void (*key_press_v)(void);
void (*key_press_w)(void);
void (*key_press_x)(void);
void (*key_press_y)(void);
void (*key_press_z)(void);
void (*key_press_0)(void);
void (*key_press_1)(void);
void (*key_press_2)(void);
void (*key_press_3)(void);
void (*key_press_4)(void);
void (*key_press_5)(void);
void (*key_press_6)(void);
void (*key_press_7)(void);
void (*key_press_8)(void);
void (*key_press_9)(void);

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