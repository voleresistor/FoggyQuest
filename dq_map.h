/* dq_map.h */

/* Forward declarations from external components outside of ifndef? */
struct Tile;
struct Actor;

#ifndef DQ_MAP__
#define DQ_MAP__

/* Map array size control */
#define MAX_ROWS 50
#define MAX_COLS 50
#define MOVE_UPDATES 12
#define IDLE_DELAY 90

struct AreaMap {
    char _area_name[15];
    int _cols;
    int _rows;
    int _map_height;
    int _actor_count;
    struct Tile* _map[MAX_ROWS][MAX_COLS];
    struct Actor* _actors[25];
};

/* Area render control */
int tile_size;

/* Currently loaded map */
struct AreaMap* _area_map;

/* Map loading */
extern FILE* map_open_map_file(char file_name_[20]);
extern void map_map_init(struct AreaMap* m_);
extern struct AreaMap* map_new_areamap(char area_name_[15]);
extern struct AreaMap* map_load_area(char* file_name_, int tile_size);
extern struct Tile* map_area_transition(struct Actor* h_, struct AreaMap* m_);
extern void map_tile_add_chest(struct AreaMap* m_, char b[255]);
extern void map_tile_add_door(struct AreaMap* m_, char b[255]);
extern void map_tile_add_link(struct AreaMap* m_, char b[255]);

/* Map queries */
extern struct Tile* map_get_tile(int col, int row);

#endif