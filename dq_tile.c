/* dq_tile.c */

#include "dq_tile.h"

Tile* tile_new(int texture_, int chest_id_, int door_id_, int link_row_, int link_col_, int mob_level_, float enc_rate_, bool is_active_, char* link_map_)
{
    Tile* t_ = malloc(sizeof(Tile));

    t_->_tile_texture    = texture_;
    t_->_chest_id        = chest_id_;
    t_->_door_id         = door_id_;
    t_->_link_row        = link_row_;
    t_->_link_col        = link_col_;
    t_->_mob_level       = mob_level_;
    t_->_encounter_rate  = enc_rate_;
    t_->_is_passable     = true;
    t_->_is_active       = is_active_;
    *strcpy(t_->_link_map, link_map_);

    if(texture_ < GRASS || door_id_ > 0)
    {
        t_->_is_passable = false;
    }

    // tile_identify(&t_);

    return t_;
}

Tile* tile_load(char b[255])
{
    int my_ints[5];
    float e_rate;
    char link[15];

    sscanf(b, "%d:%d:%d:%d:%d\n", 
        &my_ints[0], &my_ints[1], &my_ints[2], &my_ints[3], &my_ints[4]);

    // row;col;texture;chest_id;door_id;row:col:mob_level:encounter_rate:link_map
    Tile* t_ = malloc(sizeof(Tile));
    t_->_row             = my_ints[0];
    t_->_col             = my_ints[1];
    t_->_tile_texture    = my_ints[2];
    t_->_chest_id        = -1;
    t_->_door_id         = -1;
    t_->_link_row        = -1;
    t_->_link_col        = -1;
    t_->_mob_level       = my_ints[3];
    t_->_encounter_rate  = my_ints[4];
    t_->_is_passable     = true;
    t_->_is_active       = true;
    *strcpy(t_->_link_map, "NULL");

    if(t_->_tile_texture < GRASS || t_->_door_id > 0)
    {
        t_->_is_passable = false;
    }

    //tile_identify(t_);

    return t_;
}

void tile_add_door(Tile* t_, int type_)
{
    t_->_door_id = type_;
    t_->_is_passable = false;
}

void tile_add_chest(Tile* t_, int contents_)
{
    t_->_chest_id = contents_;
    t_->_is_passable = false;
}

void tile_add_link(Tile* t_, int row_, int col_, char link_map_[15])
{
    t_->_link_row = row_;
    t_->_link_col = col_;
    strcpy(t_->_link_map, link_map_);
}

void tile_identify(Tile* t_)
{
    printf("Tile: [%d,%d](%d)\n", t_->_row, t_->_col, t_->_is_active);
    printf("Texture: %d\n", t_->_tile_texture);
    printf("Mob Level: %d\n", t_->_mob_level);
    printf("Encounter Rate: %f\n", t_->_encounter_rate);
    printf("Door: %d\n", t_->_door_id);
    printf("Chest: %d\n", t_->_chest_id);
    printf("Link: [%d, %d] - %s\n", t_->_link_row, t_->_link_col, t_->_link_map);
    printf("\n");
}

void tile_open_door(Tile* t)
{

}

void tile_open_chest(Tile* t)
{

}

DestTile* tile_get_dest(int col_, int row_, int dir_)
{
    DestTile* d_ = malloc(sizeof(DestTile));
    d_->_row = row_;
    d_->_col = col_;
    d_->_dir = dir_;

    switch(dir_)
    {
        case 0: // North
        d_->_row = row_ - 1;
        // printf("Move North: [%d,%d] - [%d,%d]\n", row_, col_, d_->_row, d_->_col);
        break;

        case 1: // East
        d_->_col = col_ + 1;
        // printf("Move East: [%d,%d] - [%d,%d]\n", row_, col_, d_->_row, d_->_col);
        break;

        case 2: // South
        d_->_row = row_ + 1;
        // printf("Move South: [%d,%d] - [%d,%d]\n", row_, col_, d_->_row, d_->_col);
        break;

        case 3: // West
        d_->_col = col_ - 1;
        // printf("Move West: [%d,%d] - [%d,%d]\n", row_, col_, d_->_row, d_->_col);
        break;
    }

    return d_;
}

bool tile_is_passable(Tile* t_)
{
    return t_->_is_passable;
}

void tile_set_passable(Tile* t_, bool passable)
{
    t_->_is_passable = passable;
}