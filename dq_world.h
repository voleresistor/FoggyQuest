#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/util.h"
#include "dq_tile.h"
#include "dq_actor.h"
#include "dq_sdl.h"

/* Map array size control */
#define MAX_ROWS 50
#define MAX_COLS 50

typedef struct {
    char _area_name[15];
    int _cols;
    int _rows;
    int _map_height;
    int _actor_count;
    Tile _map[MAX_ROWS][MAX_COLS];
    Actor _actors[25];
} AreaMap;

/*
    Map object to define map IDs for loading from
    links and map file names.
*/
typedef struct {
    int _id;
    char _file_name[15];
} MapDef;

/*
    A link structure to define where stairs or tiles
    may link to for purposes of moving between
    floors/areas,
*/
typedef struct {
    int _id;
    int _map_id;
    int _col;
    int _row;
} LinkDef;

/* 
    Chest object to define the contents and
    state of a chest.

    This one needs more thought. Should it be part of
    the save game to prevent important chests from
    respawning on reload?
*/
typedef struct {
    int _id;
    int _contents;
} ChestDef;

enum AreaMaps {
    BLANK = 0,          // Default blank link
    TESTMAP2F,               // Link to test map
    TESTMAP1F,
    TESTMAP1B,
    // Cities
    TANTEGEL1F = 100,
    TANTEGEL2F,
    TANTEGEL1B,
    BRECCONARY,
    GARINHAM,
    KOL,
    RIMULDAR,
    HAUKSNES,
    CANTLIN
};

/* Global poopoo */
// Nothing to see here

extern int world_init_game();
extern int world_main_loop(void);

extern AreaMap* world_load_area(char* area_name_);  // Load area/world map
extern AreaMap* world_load_area_old(char* file_name_);
extern AreaMap* world_new_areamap(char* area_name_);

extern void world_map_init(AreaMap* m_);
extern void world_draw_map(AreaMap* cur_);
extern void world_draw_background(int map_height_);
extern void world_actors_move(AreaMap* m_);
extern void world_actor_move(Actor* a_, AreaMap* m_, DestTile* d_);
extern void world_actors_update(AreaMap* m_);
extern void world_hero_move(Actor* h_, AreaMap* m_);
extern void world_hero_update(Actor* h_);
extern void world_draw_actors(AreaMap* m_);
extern void world_draw_actor(Actor* a_);
extern void world_tile_add_link(AreaMap* m_, char b[255]);
extern void world_tile_add_door(AreaMap* m_, char b[255]);
extern void world_tile_add_chest(AreaMap* m_, char b[255]);
extern bool world_area_transition(Actor* h_, AreaMap* m_);

extern Tile* world_tile_lookup(AreaMap* m_, int row_, int col_);
extern FILE* world_open_map_file(char file_name_[20]);

/*
    dq_world maintains helper functions to facilitate data
    transfer between modules by translating to or from structures
    that the source and dest module can understand.

    This allows dq_world to maintain all the headers for other modules
    to prevent cyclic dependencies while allowing data to be easily
    transferred back and forth.

    e.g. Get destination tile row/col from dq_tile/DestTile to update
         move to location in dq_actor/Actor

        void world_actor_move(Actor* a_, int dir_, int row_, int col_)
        Gets row_ and col_ from a DestTile object and transmits it
        into the dq_actor module to facilitate moving to a new tile.
*/