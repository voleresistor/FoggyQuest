/* dq_hero.h */

#include <stdbool.h>

/* Forward declarations from external components outside of ifndef? */
struct Actor;
struct InvItem;
struct HeroInv;
struct Hero;
struct Tile;
struct AreaMap;
struct Locator;
struct LogService;
struct ControlService;

#ifndef DQ_HERO__
#define DQ_HERO__

struct InvItem {
    struct InvItem* _prev;
    struct InvItem* _next;
    int _item_id;
    int _item_count;
    int _max_items;
};

struct HeroInv {
    struct InvItem* _next;
    int _inv_count;
};

struct Hero {
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
    struct Actor* _actor;
    struct HeroInv* _inv;
};

#define MAX_INV 8

// int hero_move;
int tile_size;

extern struct Hero* hero_new_hero(char name[15], int row, int col, int tile_size);

/* Movement */
extern void hero_move_hero(struct Actor* h_, struct AreaMap* m_);
extern void hero_update(struct Actor* h_);

/* Inventory functions */
extern struct HeroInv* hero_new_inventory(void);
extern struct InvItem* hero_new_inv_item(int item_id_);
extern void hero_inventory_list(struct Hero* a_);
extern bool hero_inventory_remove(struct Hero* hero_, int item_id_);
extern bool hero_inventory_add(struct Hero* a_, int item_id_);
// extern int* hero_inventory_new(void);
extern struct InvItem* hero_inventory_next(struct InvItem* i_);
extern struct InvItem* hero_inventory_prev(struct InvItem* i_);
extern struct InvItem* hero_inventory_last(struct InvItem* i_);

#endif