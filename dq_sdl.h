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
int             hero_move;

/* Keyboard alpha-numeric function pointers */
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

/* Keyboard punctuation/operator pointers */
void (*key_press_apostrophe)(void);
void (*key_press_backslash)(void);
void (*key_press_comma)(void);
void (*key_press_equals)(void);
void (*key_press_grave)(void);
void (*key_press_period)(void);
void (*key_press_minus)(void);
void (*key_press_lbracket)(void);
void (*key_press_rightbracket)(void);
void (*key_press_semicolon)(void);
void (*key_press_slash)(void);
void (*key_press_space)(void);

/* Keyboard keypad function pointers */
void (*key_press_kp_0)(void);
void (*key_press_kp_1)(void);
void (*key_press_kp_2)(void);
void (*key_press_kp_3)(void);
void (*key_press_kp_4)(void);
void (*key_press_kp_5)(void);
void (*key_press_kp_6)(void);
void (*key_press_kp_7)(void);
void (*key_press_kp_8)(void);
void (*key_press_kp_9)(void);
void (*key_press_kp_divide)(void);
void (*key_press_kp_minus)(void);
void (*key_press_kp_multiply)(void);
void (*key_press_kp_plus)(void);
void (*key_press_kp_enter)(void);
void (*key_press_kp_decimal)(void);

/* Keyboard function keys */
void (*key_press_f1)(void);
void (*key_press_f2)(void);
void (*key_press_f3)(void);
void (*key_press_f4)(void);
void (*key_press_f5)(void);
void (*key_press_f6)(void);
void (*key_press_f7)(void);
void (*key_press_f8)(void);
void (*key_press_f9)(void);
void (*key_press_f10)(void);
void (*key_press_f11)(void);
void (*key_press_f12)(void);

/* Keyboard other keys */
void (*key_press_down)(void);
void (*key_press_up)(void);
void (*key_press_left)(void);
void (*key_press_right)(void);
void (*key_press_application)(void);
void (*key_press_backspace)(void);
void (*key_press_capslock)(void);
void (*key_press_end)(void);
void (*key_press_escape)(void);
void (*key_press_home)(void);
void (*key_press_insert)(void);
void (*key_press_lalt)(void);
void (*key_press_lctrl)(void);
void (*key_press_lshift)(void);
void (*key_press_numlock)(void);
void (*key_press_pgdown)(void);
void (*key_press_pgup)(void);
void (*key_press_pause)(void);
void (*key_press_prtscrn)(void);
void (*key_press_ralt)(void);
void (*key_press_rctrl)(void);
void (*key_press_return)(void);
void (*key_press_return2)(void);
void (*key_press_rshift)(void);
void (*key_press_scrllck)(void);
void (*key_press_tab)(void);

/* Gamepad function pointers */
void (*gamepad_up)(void);
void (*gamepad_down)(void);
void (*gamepad_left)(void);
void (*gamepad_right)(void);
void (*gamepad_b0)(void);
void (*gamepad_b1)(void);
void (*gamepad_b2)(void);
void (*gamepad_b3)(void);
void (*gamepad_b4)(void);
void (*gamepad_b5)(void);
void (*gamepad_b6)(void);
void (*gamepad_b7)(void);

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