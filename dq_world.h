#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/util.h"
#include "dq_sdl.h"
#include "dq_tile.h"
#include "dq_actor.h"
#include "dq_menu.h"
#include "dq_controls.h"

/* Map array size control */
#define MAX_ROWS 50
#define MAX_COLS 50
#define MOVE_UPDATES 12
#define IDLE_DELAY 90

typedef struct {
    char _area_name[15];
    int _cols;
    int _rows;
    int _map_height;
    int _actor_count;
    Tile _map[MAX_ROWS][MAX_COLS];
    Actor _actors[25];
} AreaMap;

/* Global poopoo */
// Nothing to see here


/* Main game controls */
extern int world_init_game();
extern int world_main_loop(void);

/* AreaMaps */
extern AreaMap* world_load_area(char* area_name_);  // Load area/world map
extern AreaMap* world_new_areamap(char* area_name_);

/* Create */
extern void world_map_init(AreaMap* m_);

/* Change */
extern void world_actors_move(AreaMap* m_);
extern void world_actor_move(Actor* a_, AreaMap* m_, DestTile* d_);
extern void world_actors_update(AreaMap* m_);
extern void world_hero_move(Actor* h_, AreaMap* m_);
extern void world_hero_update(Actor* h_);
extern void world_tile_add_link(AreaMap* m_, char b[255]);
extern void world_tile_add_door(AreaMap* m_, char b[255]);
extern void world_tile_add_chest(AreaMap* m_, char b[255]);
extern Tile* world_area_transition(Actor* h_, AreaMap* m_);
extern void world_actor_transition(Actor* a_, Tile* t_);

/* Draw */
extern void world_draw_map(AreaMap* cur_);
extern void world_draw_background(int map_height_);
extern void world_draw_actors(AreaMap* m_);
extern void world_draw_actor(Actor* a_);

/* Query */

extern Tile* world_tile_lookup(AreaMap* m_, int row_, int col_);
extern FILE* world_open_map_file(char file_name_[20]);

/* Menus */
extern void world_menu_draw_idle(Actor* a_);
extern void world_menu_draw_action(void);

/* These are likely to die or change */
extern void get_int_string(int i, char* data, int buf);