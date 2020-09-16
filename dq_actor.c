/* dq_actor.c */

#include "dq_actor.h"

Actor* actor_new(char name_[15], int type_id_, int merchant_id_, bool stationary_, int row_, int col_, int tile_size_)
{
    Actor* n_ = malloc(sizeof(Actor));

    strcpy(n_->_name, name_);
    n_->_type_id         = type_id_;
    n_->_merchant_id     = merchant_id_;
    n_->_stationary      = stationary_;
    n_->_moving          = false;
    n_->_on_link         = false;
    n_->_row             = row_;
    n_->_col             = col_;
    n_->_x_pos           = col_ * tile_size_;
    n_->_y_pos           = row_ * tile_size_;

    return n_;
}

Actor* actor_load(char b[255])
{
    int actor_data[5];
    char a_name[15];

    sscanf(b, "%d:%d:%d:%d:%d:%s\n", &actor_data[0], &actor_data[1],
        &actor_data[2], &actor_data[3], &actor_data[4], a_name);

    Actor* a_ = malloc(sizeof(Actor));
    a_->_row             = actor_data[0];
    a_->_col             = actor_data[1];
    a_->_type_id         = actor_data[2];
    a_->_merchant_id     = actor_data[4];
    a_->_stationary      = false;
    *strcpy(a_->_name, a_name);

    if(actor_data[3] == 1)
    {
        a_->_stationary = true;
    }

    //actor_identify(a_);

    return a_;
}

void actor_identify(Actor* a_)
{
    printf("Name: %s\n", a_->_name);
    printf("Type: %d\n", a_->_type_id);

    if(a_->_type_id == HERO)
    {
        printf("XP: %d (%d)\n", a_->_xp, a_->_level);
        printf("HP: %d/%d\n", a_->_cur_hp, a_->_max_hp);
        printf("MP: %d/%d\n", a_->_cur_mp, a_->_max_mp);
        printf("Atk: %d\n", a_->_atk);
        printf("Def: %d\n", a_->_def);
        printf("Str: %d\n", a_->_str);
        printf("Agi: %d\n", a_->_agi);
    }
    else
    {
        printf("Location: [%d, %d]\n", a_->_row, a_->_col);
        printf("Stationary: %d\n", a_->_stationary);
        printf("Merchant ID: %d\n", a_->_merchant_id);
    }
    printf("\n");
}

void actor_move(Actor* a_, int dir_, int dest_row_, int dest_col_)
{
    actor_face(a_, dir_);
    
    if(a_->_moving == false)
    {
        a_->_moving = true;
        a_->_col = dest_col_;
        a_->_row = dest_row_;
    }

    if(a_->_on_link)
    {
        actor_set_on_link(a_, false);
    }
}

void actor_update(Actor* a_, int tile_size_)
{
    if(actor_has_arrived(a_, tile_size_))
    {
        a_->_moving = false;
    }

    if(a_->_moving)
    {
        switch(a_->_facing)
        {
            case 0:
            // printf("%s move [%d,%d] - [%d,%d]\n", a_->_name, a_->_y_pos, a_->_x_pos, a_->_y_pos - move_speed, a_->_x_pos);
            actor_set_location(a_, a_->_x_pos, a_->_y_pos -= move_speed);
            break;

            case 1:
            // printf("%s move [%d,%d] - [%d,%d]\n", a_->_name, a_->_y_pos, a_->_x_pos, a_->_y_pos, a_->_x_pos + move_speed);
            actor_set_location(a_, a_->_x_pos += move_speed, a_->_y_pos);
            break;

            case 2:
            // printf("%s move [%d,%d] - [%d,%d]\n", a_->_name, a_->_y_pos, a_->_x_pos, a_->_y_pos + move_speed, a_->_x_pos);
            actor_set_location(a_, a_->_x_pos, a_->_y_pos += move_speed);
            break;

            case 3:
            // printf("%s move [%d,%d] - [%d,%d]\n", a_->_name, a_->_y_pos, a_->_x_pos, a_->_y_pos, a_->_x_pos - move_speed);
            actor_set_location(a_, a_->_x_pos -= move_speed, a_->_y_pos);
            break;
        }
    }

    // Update texture or something
}

void actor_set_location(Actor* a_, int x_, int y_)
{
    a_->_x_pos = x_;
    a_->_y_pos = y_;
}

void actor_set_grid(Actor* a_, int col_, int row_)
{
    a_->_col = col_;
    a_->_row = row_;
}

void actor_face(Actor* a_, int dir_)
{
    a_->_facing = dir_;
    // Something something texture
}

bool actor_has_arrived(Actor* a_, int tile_size_)
{
    int abs_dist;
    switch(a_->_facing)
    {
        case 0:
        abs_dist = abs(a_->_row * tile_size_ - a_->_y_pos);
        if(abs_dist <= move_speed)
        {
            // printf("%s moving North arrived at [%d,%d] ", a_->_name, a_->_row, a_->_col);
            actor_set_location(a_, a_->_x_pos, a_->_row * tile_size_);
            // printf("([%d, %d])\n", a_->_y_pos, a_->_x_pos);
            return true;
        }
        break;

        case 1:
        abs_dist = abs(a_->_col * tile_size_ - a_->_x_pos);
        if(abs_dist <= move_speed)
        {
            // printf("%s moving East arrived at [%d,%d] ", a_->_name, a_->_row, a_->_col);
            actor_set_location(a_, a_->_col * tile_size_, a_->_y_pos);
            // printf("([%d, %d])\n", a_->_y_pos, a_->_x_pos);
            return true;
        }
        break;
        
        case 2:
        abs_dist = abs(a_->_row * tile_size_ - a_->_y_pos);
        if(abs_dist <= move_speed)
        {
            // printf("%s moving South arrived at [%d,%d] ", a_->_name, a_->_row, a_->_col);
            actor_set_location(a_, a_->_x_pos, a_->_row * tile_size_);
            // printf("([%d, %d])\n", a_->_y_pos, a_->_x_pos);
            return true;
        }
        break;
        
        case 3:
        abs_dist = abs(a_->_col * tile_size_ - a_->_x_pos);
        if(abs_dist <= move_speed)
        {
            // printf("%s moving West arrived at [%d,%d] ", a_->_name, a_->_row, a_->_col);
            actor_set_location(a_, a_->_col * tile_size_, a_->_y_pos);
            // printf("([%d, %d])\n", a_->_y_pos, a_->_x_pos);
            return true;
        }
        break;
    }

    return false;
}

bool actor_will_move(Actor* a_)
{
    if(!a_->_stationary && !a_->_moving && rand_i(1, 30) == 15)
    {
        return true;
    }

    return false;
}

void actor_set_on_link(Actor* a_, bool onlink_)
{
    a_->_on_link = onlink_;
}