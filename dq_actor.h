/* dq_actor.h */

#include <stdbool.h>

#ifndef DQ_ACTOR__
#define DQ_ACTOR__

struct Tile;
struct Locator;
struct LogService;

struct Actor {
    char _name[15];
    int _type_id;
    int _merchant_id;
    int _row;
    int _col;
    int _x_pos;
    int _y_pos;
    int _facing;
    int _idle_time;
    bool _moving;
    bool _stationary;
    bool _on_link;
};

enum TypeId {
    TESTACTOR = 0,
    KING,
    PRINCESS,
    GUARD,
    BARD,
    OLDMAN,
    YOUNGMAN,
    OLDWOMAN,
    YOUNGWOMAN,
    CHILD,
    MERCHANT,
    INNKEEP,
    GREENDRAGON,
    DRAGONLORD,
    HERO = 999
};

enum Facing {
    NOMOVE = 0,
    NORTH,
    EAST,
    SOUTH,
    WEST
};

/* Actor controls */
int move_speed;

extern struct Actor* actor_new(char name_[15], int type_id_, int merchant_id_, bool stationary_, int row_, int col_, int tile_size_);
extern struct Actor* actor_load(char b[255]);

extern void actor_move(struct Actor* a_, int dir_, int dest_row_, int dest_col_);
extern void actor_move_actor(struct Actor* a_, struct Tile* c_, struct Tile* d_, int dir_);
extern void actor_update(struct Actor* a_, int tile_size_);
extern void actor_face(struct Actor* a_, int dir_);
extern void actor_set_location(struct Actor* a_, int x_, int y_);
extern void actor_set_grid(struct Actor* a_, int col_, int row_);
extern void actor_set_on_link(struct Actor* a_, bool onlink_);
extern void actor_identify(struct Actor* a_);
extern bool actor_has_arrived(struct Actor* a_, int tile_size_);
extern bool actor_will_move(struct Actor* a_);

#endif