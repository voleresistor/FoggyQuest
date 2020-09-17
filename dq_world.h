#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/util.h"
#include "dq_sdl.h"
#include "dq_tile.h"
#include "dq_menu.h"
//#include "dq_controls.h"

/* Forward declarations from external components outside of ifndef? */
struct Hero;
struct Actor;
struct AreaMap;
struct Tile;
struct DestTile;

#ifndef DQ_WORLD__
#define DQ_WORLD__

/* Game modes */
enum GameMode {
    /* Menus */
    MAIN_MENU = 0x10,
    ACTION_MENU,
    COMBAT_MENU,
    DIALOG_MENU,
    
    /* Map Navigation */
    WORLD_MAP = 0x33,
    AREA_MAP,

    /* Game state control */
    NEW_GAME = 0x69,
    QUIT_GAME
};

/* Main game controls */
extern int world_init_game();
extern int world_main_loop(void);

/* Change */
extern void world_actors_move(struct AreaMap* m_);
extern void world_actor_move(struct Actor* a_, struct Tile* c_, struct Tile* d_, int dir_);
extern void world_actors_update(struct AreaMap* m_);
extern void world_tile_add_link(struct AreaMap* m_, char b[255]);
extern void world_tile_add_door(struct AreaMap* m_, char b[255]);
extern void world_tile_add_chest(struct AreaMap* m_, char b[255]);
extern void world_actor_transition(struct Actor* a_, struct Tile* t_);

/* Draw */
extern void world_draw_map(struct AreaMap* cur_);
extern void world_draw_background(int map_height_);
extern void world_draw_actors(struct AreaMap* m_);
extern void world_draw_actor(struct Actor* a_);

/* Query */

extern struct Tile* world_tile_lookup(struct AreaMap* m_, int row_, int col_);

/* Menus */
extern void world_menu_draw_idle(struct Hero* hero_);
extern void world_menu_draw_action(void);

/* Control functions */
extern void world_action_up(void);
extern void world_action_down(void);
extern void world_action_left(void);
extern void world_action_right(void);
extern void world_action_interact(void);
extern void world_action_cancel(void);
extern void world_action_select(void);
extern void world_action_unbound(void);
extern void world_load_controls(char ctrl_conf[15]);

/* These are likely to die or change */
extern void get_int_string(int i, char* data, int buf);
extern void world_set_hero_move(int d);

#endif