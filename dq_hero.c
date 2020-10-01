/* dq_hero.c */

#include <stdlib.h>
#include <stdio.h>

#include "dq_hero.h"
#include "dq_actor.h"
#include "dq_locator.h"
#include "dq_log.h"
#include "dq_map.h"
#include "dq_tile.h"

struct Hero* hero_new_hero(char name[15], int row, int col, int tile_size)
{
    /* Create base actor */
    struct Actor* new_a = actor_new(name, HERO, 0, false, row, col, tile_size);

    /* Create inventory */
    struct HeroInv* new_i = hero_new_inventory();

    /* Create hero */
    struct Hero* new_h = malloc(sizeof(struct Hero));
    new_h->_level      = 1;
    new_h->_xp         = 0;
    new_h->_gold       = 0;
    new_h->_max_hp     = 15;
    new_h->_max_mp     = 0;
    new_h->_cur_hp     = 15;
    new_h->_cur_mp     = 0;
    new_h->_str        = 4;
    new_h->_agi        = 6;
    new_h->_res        = 0;
    new_h->_wis        = 0;
    new_h->_lck        = 0;
    new_h->_atk        = new_h->_str;
    new_h->_def        = new_h->_agi / 2;

    /* Add base and inventory to hero */
    new_h->_actor = new_a;
    new_h->_inv = new_i;

    char log_msg_[255];
    system_locator->get_log()->write_log("Hero successfully loaded.\0", this_func, DQINFO);
    sprintf(log_msg_, "Hero name: %s\0", new_h->_actor->_name);
    system_locator->get_log()->write_log(log_msg_, this_func, DQINFO);
    sprintf(log_msg_, "Hero location: px[%d,%d] gr[%d,%d]\0", new_h->_actor->_x_pos, new_h->_actor->_y_pos, new_h->_actor->_col, new_h->_actor->_row);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero level: %d\0", new_h->_level);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero xp: %d\0", new_h->_xp);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero gold: %d\0", new_h->_gold);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero HP: %d/%d\0", new_h->_cur_hp, new_h->_max_hp);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero MP: %d/%d\0", new_h->_cur_mp, new_h->_max_mp);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero strength: %d\0", new_h->_str);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero agility: %d\0", new_h->_agi);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero resilience: %d\0", new_h->_res);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero wisdom: %d\0", new_h->_wis);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero luck: %d\0", new_h->_lck);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero attack: %d\0", new_h->_atk);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    sprintf(log_msg_, "Hero defense: %d\0", new_h->_def);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);

    return new_h;
}

struct HeroInv* hero_new_inventory(void)
{
    struct HeroInv* new_ = malloc(sizeof(struct HeroInv));
    new_->_inv_count = 0;
    new_->_next = NULL;

    system_locator->get_log()->write_log("New empty inventory initialized.\0", this_func, DQDEBUG);
    return new_;
}

struct InvItem* hero_new_inv_item(int item_id_)
{
    struct InvItem* new_    = malloc(sizeof(struct InvItem));
    new_->_item_id          = item_id_;
    new_->_item_count       = 1;
    new_->_max_items        = 1;
    new_->_next             = NULL;
    new_->_prev             = NULL;

    char log_msg_[255];
    sprintf(log_msg_, "Created %d item of type %d\0", new_->_item_count, new_->_item_id);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);

    return new_;
}

bool hero_inventory_add(struct Hero* hero_, int item_id_)
{
    char log_msg_[255];

    if(hero_->_inv->_inv_count < MAX_INV)
    {
        struct InvItem* new_ = hero_new_inv_item(item_id_);

        if(hero_->_inv->_next != NULL)
        {
            struct InvItem* last_ = hero_inventory_last(hero_->_inv->_next);
            sprintf(log_msg_, "last_->_item_id: %d\0", last_->_item_id);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            sprintf(log_msg_, "new_->_item_id: %d\0", new_->_item_id);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            last_->_next = new_;
            new_->_prev = last_;
        }
        else
        {
            hero_->_inv->_next = new_;
        }

        hero_->_inv->_inv_count++;
        sprintf(log_msg_, "New inventory count: %d\0", hero_->_inv->_inv_count);
        system_locator->get_log()->write_log(log_msg_, this_func, DQINFO);
        return true;
    }

    system_locator->get_log()->write_log("Hero inventory is fill.\0", this_func, DQINFO);
    return false;
}

struct InvItem* hero_inventory_get_item(struct InvItem* next_, int item_id_)
{
    char log_msg_[255];

    while(next_->_item_id != item_id_ && next_->_next != NULL)
    {
        next_ = hero_inventory_next(next_);
    }

    if(next_->_item_id == item_id_)
    {
        sprintf(log_msg_, "Found item type: %d\0", next_->_item_id);
        system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
        return next_;
    }

    sprintf(log_msg_, "Item type %d not found in inventory.\0", item_id_);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    return NULL;
}

bool hero_inventory_remove(struct Hero* hero_, int item_id_)
{
    char log_msg_[255];

    if(hero_->_inv->_next != NULL)
    {
        struct InvItem* remove_ = hero_inventory_get_item(hero_->_inv->_next, item_id_);

        /* if we don't find the item */
        if(remove_ == NULL)
        {
            system_locator->get_log()->write_log("Item not found.\0", this_func, DQINFO);
            return false;
        }

        struct InvItem* prev_ = hero_inventory_prev(remove_);
        struct InvItem* next_ = hero_inventory_next(remove_);
        if(prev_ == NULL && next_ == NULL)
        {
            hero_->_inv->_next = NULL;
            sprintf(log_msg_, "Last item removed from %s inventory.\0", hero_->_actor->_name);
            system_locator->get_log()->write_log(log_msg_, this_func, DQINFO);
        }
        else if(prev_ == NULL && next_ != NULL)
        {
            hero_->_inv->_next = next_;
            next_->_prev = NULL;
        }
        else if(next_->_next == NULL && prev_ != NULL)
        {
            prev_->_next = NULL;
        }
        else
        {
            struct InvItem* n_next_ = hero_inventory_next(next_);
            prev_->_next = next_->_next;
            n_next_->_prev = next_->_prev;
        }

        hero_->_inv->_inv_count--;
        sprintf(log_msg_, "Item id %d successfully removed from %s inventory.\0", item_id_, hero_->_actor->_name);
        return true;
    }

    return false;
}

struct InvItem* hero_inventory_next(struct InvItem* i_)
{
    return i_->_next;
}

struct InvItem* hero_inventory_prev(struct InvItem* i_)
{
    return i_->_prev;
}

struct InvItem* hero_inventory_last(struct InvItem* i_)
{
    if(i_->_next != NULL)
    {
        return hero_inventory_last(i_->_next);
    }

    return i_;
}

void hero_inventory_list(struct Hero* hero_)
{
    printf("Items in %s's bag (%d/%d):\n", hero_->_actor->_name, hero_->_inv->_inv_count, MAX_INV);
    if(hero_->_inv->_next == NULL)
    {
        printf("Bag is empty.\n");
    }
    else
    {
        struct InvItem* i_ = hero_->_inv->_next;
        while(i_ != NULL)
        {
            printf("%d(%d)\n", i_->_item_id, i_->_item_count);
            i_ = hero_inventory_next(i_);
        }
    }

    printf("\n");
}

void hero_move_hero(struct Actor* h_, struct AreaMap* m_)
{
    // Don't allow actor movement in menus
    // if(in_menu)
    // {
    //     hero_move = -1;
    //     h_->_idle_time = 0;
    //     return;
    // }

    if(hero_move > 0 && !h_->_moving)
    {
        struct DestTile* d_ = tile_get_dest(h_->_col, h_->_row, hero_move);
        struct Tile* dest = map_get_tile(d_->_row, d_->_col);
        struct Tile* cur = map_get_tile(h_->_row, h_->_col);
        actor_move_actor(h_, cur, dest, d_->_dir);
        free(d_);
        h_->_idle_time = 0;

        hero_move = 0;
    }

    hero_update(h_);
}

void hero_update(struct Actor* h_)
{
    if(h_->_idle_time < 180)
    {
        h_->_idle_time++;
    }

    if(h_->_moving)
    {
        actor_update(h_, tile_size);
        return;
    }
}