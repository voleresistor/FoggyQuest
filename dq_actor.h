/* dq_actor.h */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lib/util.h"

typedef struct {
    char _name[15];
    int _type_id;
    int _merchant_id;
    int _level;
    int _xp;
    int _gold;
    int _max_hp;
    int _cur_hp;
    int _max_mp;
    int _cur_mp;
    int _str;
    int _agi;
    int _res;
    int _wis;
    int _lck;
    int _atk;
    int _def;
    int _row;
    int _col;
    int _x_pos;
    int _y_pos;
    int _facing;
    bool _moving;
    bool _stationary;
    bool _on_link;
} Actor;

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
    DRAGONKING,
    HERO = 999
};

enum Facing {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

/* Actor controls */
int move_speed;

extern Actor* actor_new(char name_[15], int type_id_, int merchant_id_, bool stationary_, int row_, int col_, int tile_size_);
extern Actor* actor_load(char b[255]);

extern void actor_move(Actor* a_, int dir_, int dest_row_, int dest_col_);
extern void actor_update(Actor* a_, int tile_size_);
extern void actor_face(Actor* a_, int dir_);
extern void actor_set_location(Actor* a_, int x_, int y_);
extern void actor_set_grid(Actor* a_, int col_, int row_);
extern void actor_set_on_link(Actor* a_, bool onlink_);
extern void actor_identify(Actor* a_);
extern bool actor_has_arrived(Actor* a_, int tile_size_);
extern bool actor_will_move(Actor* a_);