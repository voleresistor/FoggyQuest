/* dq_map.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dq_map.h"
#include "dq_locator.h"
#include "dq_log.h"
#include "dq_tile.h"
#include "dq_actor.h"

FILE* map_open_map_file(char file_name_[20])
{
    FILE* ptr;
    char map_path_[20];
    char log_msg_[255];

    strcpy(map_path_, "map/");
    strcat(map_path_, file_name_);
    ptr = fopen(map_path_, "r");
    if(ptr == NULL)
    {
        // printf("Couldn't load map: %s\n", map_path_);
        sprintf(log_msg_, "Couldn't load map: %s", map_path_);
        system_locator->get_log()->write_log(log_msg_, this_func, DQERROR);
        return NULL;
    }

    sprintf(log_msg_, "Successfully loaded map: %s", map_path_);
    system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    return ptr;
}

/*
    map_map_init()

    Init an AreaMap _map array with inactive zero Tile
    objects.
*/
void map_map_init(struct AreaMap* m_)
{
    struct Tile* t_;

    for(int r = 0; r < MAX_ROWS; r++)
    {
        for(int c = 0; c < MAX_COLS; c++)
        {
            // printf("Setting inactive tile on [%d][%d]...\t", r, c);
            t_ = tile_new(r, c, -1, -1, -1, 0, 0, false, "NULL");
            m_->_map[r][c] = t_;
            // printf("DONE\n");
        }
    }

    // printf("Completed initiating new map.\n");
}

struct AreaMap* map_new_areamap(char area_name_[15])
{
    struct AreaMap* m_ = malloc(sizeof(struct AreaMap));
    // printf("Copying file name in map_new_areamap()\n");
    strcpy(m_->_area_name, area_name_);
    m_->_actor_count = 0;

    map_map_init(m_);

    return m_;
}

struct AreaMap* map_load_area(char* file_name_, int tile_size)
{
    FILE* fptr;
    int level_;
    int c;
    int link_data_[4];

    char buf[255];
    char link_name_[15];
    char map_path_[20];

    struct Actor* a_;
    struct Tile* t_;
    struct Tile* l_;
    

    /* Init new map */
    struct AreaMap* m_ = map_new_areamap(file_name_);

    /* Open map file */
    fptr = map_open_map_file(file_name_);

    /* Read map data into map */
    c = fgetc(fptr);
    while(c != EOF)
    {
        fgets(buf, sizeof(buf), fptr);
        switch(c)
        {
            case 't':
            // printf("Tile: %s\n", buf);
            t_ = tile_load(buf);
            m_->_map[t_->_row][t_->_col] = t_;
            // tile_identify(&(m_->_map[t_->_row][t_->_col]));
            break;

            case 'a':
            // printf("Actor: %s\n", buf);
            a_ = actor_load(buf);
            actor_set_location(a_, a_->_col * tile_size, a_->_row * tile_size);
            m_->_actors[m_->_actor_count] = a_;
            //actor_identify(&(m_->_actors[m_->_actor_count]));
            m_->_actor_count++;
            break;

            case 'c':
            // printf("Chest: %s\n", buf);
            map_tile_add_chest(m_, buf);
            break;

            case 'd':
            // printf("Door: %s\n", buf);
            map_tile_add_door(m_, buf);
            break;

            case 'm':
            // printf("Maplink: %s\n", buf);
            map_tile_add_link(m_, buf);
            break;

            case 'l':
            // printf("Level: %s\n", buf);
            sscanf(buf, "%d", &level_);
            m_->_map_height = level_;
            break;

            case '#':
            /* Ignore comments */
            //printf("Comment: %c%s\n", c, buf);
            break;
        }

        c = fgetc(fptr);
    }

    fclose(fptr);
    return m_;
}

struct Tile* map_area_transition(struct Actor* h_, struct AreaMap* m_)
{
    char log_msg_[255];

    struct Tile* t_ = m_->_map[h_->_row][h_->_col];

    // sprintf(log_msg_, "Hero grid: [%d, %d]\0", h_->_col, h_->_row);
    // system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);

    if(!h_->_on_link && !h_->_moving && strcmp(t_->_link_map, "NULL") != 0)
    {
        return t_;

        sprintf(log_msg_, "Transition tile: [%d/%d]\0", t_->_col, t_->_row);
        system_locator->get_log()->write_log(log_msg_, this_func, DQINFO);
        sprintf(log_msg_, "Linked map: %s\0", t_->_link_map);
        system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    }

    // sprintf(log_msg_, "No map transition required.");
    // system_locator->get_log()->write_log(log_msg_, this_func, DQDEBUG);
    return NULL;
}

void map_tile_add_link(struct AreaMap* m_, char b[255])
{
    struct Tile* l_;
    int link_data_[4];
    char link_name_[15];

    sscanf(b, "%d:%d:%d:%d:%s", &link_data_[0], &link_data_[1], &link_data_[2],
        &link_data_[3], link_name_);
    l_ = m_->_map[link_data_[0]][link_data_[1]];
    tile_add_link(l_, link_data_[2], link_data_[3], link_name_);
    m_->_map[link_data_[0]][link_data_[1]] = l_;
    //tile_identify(m_->_map[link_data_[0]][link_data_[1]]);
}

void map_tile_add_door(struct AreaMap* m_, char b[255])
{
    struct Tile* d_;
    int door_data_[3];

    sscanf(b, "%d:%d:%d", &door_data_[0], &door_data_[1], &door_data_[2]);
    d_ = m_->_map[door_data_[0]][door_data_[1]];
    tile_add_door(d_, door_data_[2]);
    m_->_map[door_data_[0]][door_data_[1]] = d_;
    //tile_identify(m_->_map[door_data_[0]][door_data_[1]]);
}

void map_tile_add_chest(struct AreaMap* m_, char b[255])
{
    struct Tile* c_;
    int chest_data_[3];

    sscanf(b, "%d:%d:%d", &chest_data_[0], &chest_data_[1], &chest_data_[2]);
    c_ = m_->_map[chest_data_[0]][chest_data_[1]];
    tile_add_chest(c_, chest_data_[2]);
    m_->_map[chest_data_[0]][chest_data_[1]] = c_;
    //tile_identify(m_->_map[chest_data_[0]][chest_data_[1]]);
}

struct Tile* map_get_tile(int col, int row)
{
    return _area_map->_map[row][col];
}