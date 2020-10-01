/*
    dq_world.h

    Main workhorse of the program. If anything stands to become
    monolithic in this codebase it's this component.
*/

#ifndef DQ_WORLD__
#define DQ_WORLD__

struct Locator;
struct LogService;
struct Hero;
struct Actor;
struct AreaMap;
struct Tile;
struct DestTile;

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