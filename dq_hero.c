/* dq_hero.c */

#include "dq_hero.h"
#include "dq_actor.h"

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

    return new_h;
}

struct HeroInv* hero_new_inventory(void)
{
    struct HeroInv* new_ = malloc(sizeof(struct HeroInv));
    new_->_inv_count = 0;
    new_->_next = NULL;

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

    return new_;
}

bool hero_inventory_add(struct Hero* hero_, int item_id_)
{
    if(hero_->_inv->_inv_count < MAX_INV)
    {
        struct InvItem* new_ = hero_new_inv_item(item_id_);

        if(hero_->_inv->_next != NULL)
        {
            // printf("_inv->_next != NULL\n");
            struct InvItem* last_ = hero_inventory_last(hero_->_inv->_next);
            // printf("last_->_item_id: %d\n", last_->_item_id);
            // printf("new_->_item_id: %d\n", new_->_item_id);
            last_->_next = new_;
            new_->_prev = last_;
        }
        else
        {
            hero_->_inv->_next = new_;
        }

        hero_->_inv->_inv_count++;
        return true;
    }

    return false;
}

struct InvItem* hero_inventory_get_item(struct InvItem* next_, int item_id_)
{
    while(next_->_item_id != item_id_ && next_->_next != NULL)
    {
        next_ = hero_inventory_next(next_);
    }

    if(next_->_item_id == item_id_)
    {
        return next_;
    }

    return NULL;
}

bool hero_inventory_remove(struct Hero* hero_, int item_id_)
{
    if(hero_->_inv->_next != NULL)
    {
        struct InvItem* remove_ = hero_inventory_get_item(hero_->_inv->_next, item_id_);

        /* if we don't find the item */
        if(remove_ == NULL)
        {
            return false;
        }

        struct InvItem* prev_ = hero_inventory_prev(remove_);
        struct InvItem* next_ = hero_inventory_next(remove_);
        if(prev_ == NULL && next_ == NULL)
        {
            hero_->_inv->_next = NULL;
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

        // free(next_);
        hero_->_inv->_inv_count--;
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