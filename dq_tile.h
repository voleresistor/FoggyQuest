#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum Tiles {
    // Impassable tiles
    WALL = 10,
    TABLE,
    DOOR,
    TREE,
    MOUNTAIN,
    OCEAN,
    SKY,
    // Passable tiles
    GRASS = 20,
    FOREST,
    SAND,
    HILL,
    TILE_FLOOR,
    CAVE_FLOOR,
    ROAD,
    BRIDGE,
    // Interactive tiles
    CHEST = 30,
    STAIRS,
    CAVE,
    TOWN,
    CASTLE,
    // Damaging tiles
    SWAMP = 40,
    TRAP
};

typedef struct {
    int _row;
    int _col;
    int _tile_texture;
    int _chest_id;
    int _door_id;
    int _link_row;
    int _link_col;
    int _mob_level;
    float _encounter_rate;
    bool _is_passable;
    bool _is_active;
    char _link_map[15];
} Tile;

typedef struct {
    int _row;
    int _col;
    int _dir;
} DestTile;

extern Tile* tile_new(int texture_, int chest_id_, int door_id_, int link_row_, int link_col_, int mob_level_, float enc_rate_, bool is_active_, char link_map_[15]);
extern Tile* tile_load(char b[255]);

extern void tile_open_door(Tile* t);
extern void tile_open_chest(Tile* t);
extern DestTile* tile_get_dest(int col_, int row_, int dir_);
extern bool tile_is_passable(Tile* t_);
extern void tile_set_passable(Tile* t_, bool passable);
extern void tile_identify(Tile* t_);
extern void tile_add_door(Tile* t_, int type_);
extern void tile_add_chest(Tile* t_, int contents_);
extern void tile_add_link(Tile* t_, int row_, int col_, char link_map_[15]);