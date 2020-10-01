/* dq_actor.c */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dq_actor.h"
#include "lib/util.h"
#include "dq_locator.h"
#include "dq_log.h"
#include "dq_tile.h"

struct Actor* actor_new(char name_[15], int type_id_, int merchant_id_, bool stationary_, int row_, int col_, int tile_size_)
{
    struct Actor* n_ = malloc(sizeof(struct Actor));

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

struct Actor* actor_load(char b[255])
{
    int actor_data[5];
    char a_name[15];

    sscanf(b, "%d:%d:%d:%d:%d:%s\n", &actor_data[0], &actor_data[1],
        &actor_data[2], &actor_data[3], &actor_data[4], a_name);

    struct Actor* a_ = malloc(sizeof(struct Actor));
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

void actor_identify(struct Actor* a_)
{
    printf("Name: %s\n", a_->_name);
    printf("Type: %d\n", a_->_type_id);
    printf("Location: [%d, %d]\n", a_->_row, a_->_col);
    printf("Stationary: %d\n", a_->_stationary);
    printf("Merchant ID: %d\n", a_->_merchant_id);
    printf("\n");
}

void actor_move(struct Actor* a_, int dir_, int dest_col_, int dest_row_)
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

void actor_update(struct Actor* a_, int tile_size_)
{
    char log_msg_[255];

    if(actor_has_arrived(a_, tile_size_))
    {
        a_->_moving = false;
    }

    if(a_->_moving)
    {
        switch(a_->_facing)
        {
            case 1:
            sprintf(log_msg_, "%s move [%d,%d] - [%d,%d]\0", a_->_name, a_->_x_pos, a_->_y_pos, a_->_x_pos, a_->_y_pos - move_speed);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            actor_set_location(a_, a_->_x_pos, a_->_y_pos -= move_speed);
            break;

            case 2:
            sprintf(log_msg_, "%s move [%d,%d] - [%d,%d]\0", a_->_name, a_->_x_pos, a_->_y_pos, a_->_x_pos + move_speed, a_->_y_pos);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            actor_set_location(a_, a_->_x_pos += move_speed, a_->_y_pos);
            break;

            case 3:
            sprintf(log_msg_, "%s move [%d,%d] - [%d,%d]\0", a_->_name, a_->_x_pos, a_->_y_pos, a_->_x_pos, a_->_y_pos + move_speed);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            actor_set_location(a_, a_->_x_pos, a_->_y_pos += move_speed);
            break;

            case 4:
            sprintf(log_msg_, "%s move [%d,%d] - [%d,%d]\0", a_->_name, a_->_x_pos, a_->_y_pos, a_->_x_pos - move_speed, a_->_y_pos);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            actor_set_location(a_, a_->_x_pos -= move_speed, a_->_y_pos);
            break;
        }
    }

    // Update texture or something
}

void actor_set_location(struct Actor* a_, int x_, int y_)
{
    a_->_x_pos = x_;
    a_->_y_pos = y_;
}

void actor_set_grid(struct Actor* a_, int col_, int row_)
{
    a_->_col = col_;
    a_->_row = row_;
}

void actor_face(struct Actor* a_, int dir_)
{
    a_->_facing = dir_;
    // Something something texture
}

bool actor_has_arrived(struct Actor* a_, int tile_size_)
{
    char log_msg_[255];
    int abs_dist;
    switch(a_->_facing)
    {
        case 1:
        abs_dist = abs(a_->_row * tile_size_ - a_->_y_pos);
        sprintf(log_msg_, "%s - Absolute distance from destination: %d (%d - %d)\0", a_->_name, abs_dist, a_->_row * tile_size_, a_->_y_pos);
        system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
        if(abs_dist <= move_speed)
        {
            sprintf(log_msg_, "%s moving North arrived at [%d,%d] ", a_->_name, a_->_col, a_->_row);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            actor_set_location(a_, a_->_x_pos, a_->_row * tile_size_);
            // printf("([%d, %d])\n", a_->_y_pos, a_->_x_pos);
            return true;
        }
        break;

        case 2:
        abs_dist = abs(a_->_col * tile_size_ - a_->_x_pos);
        sprintf(log_msg_, "%s - Absolute distance from destination: %d\0", a_->_name, abs_dist);
        system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
        if(abs_dist <= move_speed)
        {
            sprintf(log_msg_, "%s moving East arrived at [%d,%d]\0", a_->_name, a_->_col, a_->_row);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            actor_set_location(a_, a_->_col * tile_size_, a_->_y_pos);
            // printf("([%d, %d])\n", a_->_y_pos, a_->_x_pos);
            return true;
        }
        break;
        
        case 3:
        abs_dist = abs(a_->_row * tile_size_ - a_->_y_pos);
        sprintf(log_msg_, "%s - Absolute distance from destination: %d\0", a_->_name, abs_dist);
        system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
        if(abs_dist <= move_speed)
        {
            sprintf(log_msg_, "%s moving South arrived at [%d,%d] ", a_->_name, a_->_col, a_->_row);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            actor_set_location(a_, a_->_x_pos, a_->_row * tile_size_);
            // printf("([%d, %d])\n", a_->_y_pos, a_->_x_pos);
            return true;
        }
        break;
        
        case 4:
        abs_dist = abs(a_->_col * tile_size_ - a_->_x_pos);
        sprintf(log_msg_, "%s - Absolute distance from destination: %d\0", a_->_name, abs_dist);
        system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
        if(abs_dist <= move_speed)
        {
            sprintf(log_msg_, "%s moving West arrived at [%d,%d] ", a_->_name, a_->_col, a_->_row);
            system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
            actor_set_location(a_, a_->_col * tile_size_, a_->_y_pos);
            // printf("([%d, %d])\n", a_->_y_pos, a_->_x_pos);
            return true;
        }
        break;
    }

    return false;
}

bool actor_will_move(struct Actor* a_)
{
    if(!a_->_stationary && !a_->_moving && rand_i(1, 30) == 15)
    {
        return true;
    }

    return false;
}

void actor_set_on_link(struct Actor* a_, bool onlink_)
{
    a_->_on_link = onlink_;
}

void actor_move_actor(struct Actor* a_, struct Tile* c_, struct Tile* d_, int dir_)
{
    if(tile_is_passable(d_))
    {
        // printf("[%d, %d] is passable\n", row_, col_);
        if(a_->_row != d_->_row || a_->_col != d_->_col)
        {
            /* Get the values before they're updated */
            int col_ = a_->_col;
            int row_ = a_->_row;

            /* Update the actor */
            // printf("[%d, %d] is different\n", row_, col_);
            actor_move(a_, dir_, d_->_row, d_->_col);

            /* Make destination tile impassable */
            tile_set_passable(d_, false);
            // printf("Tile [%d, %d] now impassable\n", d_->_row, d_->_col);

            /* Make current tile passable again */
            tile_set_passable(c_, true);
            // printf("Tile [%d, %d] now passable\n", row_, col_);
        }
        else
        {
            actor_face(a_, dir_);
        }
    }
}