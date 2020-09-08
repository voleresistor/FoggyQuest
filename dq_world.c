/* dq_world.c */

#include "dq_world.h"

/*
    _init()
*/
int world_init_game()
{
    tile_cols       = 16;
    SCREEN_WIDTH    = 1024;
    SCREEN_HEIGHT   = 768;
    tile_size       = SCREEN_WIDTH / tile_cols;
    grid_rows       = SCREEN_HEIGHT / tile_size;
    grid_cols       = SCREEN_WIDTH / tile_size;

    if(init_sdl() != 0)
    {
        return EXIT_FAILURE;
    }

    //world_main_menu();
    int game_result = world_main_loop();
    close_sdl();

    return game_result;
}

int world_main_loop(void)
{
    /*
        Init move speed based on scale

        divide tile size by number of frames of animation. More
        frames results in slower movement.
    */
    move_speed = tile_size / 12;

    /* load a test area map */
    AreaMap* _area_map = world_load_area("testmap1f");

    /* Init the hero */
    Actor* _hero = actor_new("Buns", HERO, 0, false, 1, 1, tile_size);
    actor_identify(_hero);

    quit = false;
    while(!quit)
    {
        /* Handle events so we can close the window */
        event_handler();

        /*
            Transition map if necessary
        */
        if(world_area_transition(_hero, _area_map))
        {
            Tile* t_ = &(_area_map->_map[_hero->_row][_hero->_col]);
            //free(_area_map);
            _area_map = world_load_area(t_->_link_map);
        }

        /* Update the hero */
        world_hero_move(_hero, _area_map);
        world_hero_update(_hero);

        /* Update the actors */
        world_actors_move(_area_map);
        world_actors_update(_area_map);

        /* Draw the window */
        world_draw_map(_area_map);
        world_draw_actor(_hero);
        world_draw_actors(_area_map);
        SDL_RenderPresent(gRenderer);

        /* Simple 30 fps delay for now */
        SDL_Delay(33);
    }

    return EXIT_SUCCESS;
}

AreaMap* world_load_area(char* file_name_)
{
    FILE* fptr;
    int level_;
    char buf[255];
    int c;

    char map_path_[20];

    Actor* a_;
    Tile* t_;

    /* Init new map */
    AreaMap* m_ = world_new_areamap(file_name_);
    // AreaMap* m_ = malloc(sizeof(AreaMap));
    // strcpy(m_->_area_name, file_name_);
    //world_map_init(m_);

    /* Open map file */
    //fptr = world_open_map_file(file_name_);
    strcpy(map_path_, "map/");
    strcat(map_path_, file_name_);
    fptr = fopen(map_path_, "r");
    if(fptr == NULL)
    {
        printf("Couldn't load map: %s\n", map_path_);
        return NULL;
    }

    /* Read map data into map */
    c = fgetc(fptr);
    while(c != EOF)
    {
        fgets(buf, sizeof(buf), fptr);
        if(c == 't')
        {
            printf("Tile: %s\n", buf);
            t_ = tile_load(buf);
            m_->_map[t_->_row][t_->_col] = *t_;
            // tile_identify(&(m_->_map[t_->_row][t_->_col]));
        }

        if(c == 'a')
        {
            printf("Actor: %s\n", buf);
            a_ = actor_load(buf);
            actor_set_location(a_, a_->_col * tile_size, a_->_row * tile_size);
            m_->_actors[m_->_actor_count] = *a_;
            actor_identify(&(m_->_actors[m_->_actor_count]));
            m_->_actor_count++;
        }

        if(c == 'l')
        {
            printf("Level: %s\n", buf);
            sscanf(buf, "%d", &level_);
            m_->_map_height = level_;
        }

        if(c == 'c')
        {
            printf("Chest: %s\n", buf);
            world_tile_add_chest(m_, buf);
        }

        if(c == 'd')
        {
            printf("Door: %s\n", buf);
            world_tile_add_door(m_, buf);
        }

        if(c == 'm')
        {
            int link_data_[4];
            char link_name_[15];
            Tile* l_;

            printf("Maplink: %s\n", buf);
            world_tile_add_link(m_, buf);
        }

        if(c == '#')
        {
            /* Ignore comments */
            //printf("Comment: %c%s\n", c, buf);
        }

        c = fgetc(fptr);
    }

    fclose(fptr);
    return m_;
}

void world_tile_add_link(AreaMap* m_, char b[255])
{
    Tile* l_;
    int link_data_[4];
    char link_name_[15];

    sscanf(b, "%d:%d:%d:%d:%s", &link_data_[0], &link_data_[1], &link_data_[2],
        &link_data_[3], link_name_);
    l_ = &(m_->_map[link_data_[0]][link_data_[1]]);
    tile_add_link(l_, link_data_[2], link_data_[3], link_name_);
    m_->_map[link_data_[0]][link_data_[1]] = *l_;
    tile_identify(&(m_->_map[link_data_[0]][link_data_[1]]));
}

void world_tile_add_door(AreaMap* m_, char b[255])
{
    Tile* d_;
    int door_data_[3];

    sscanf(b, "%d:%d:%d", &door_data_[0], &door_data_[1], &door_data_[2]);
    d_ = &(m_->_map[door_data_[0]][door_data_[1]]);
    tile_add_door(d_, door_data_[2]);
    m_->_map[door_data_[0]][door_data_[1]] = *d_;
    tile_identify(&(m_->_map[door_data_[0]][door_data_[1]]));
}

void world_tile_add_chest(AreaMap* m_, char b[255])
{
    Tile* c_;
    int chest_data_[3];

    sscanf(b, "%d:%d:%d", &chest_data_[0], &chest_data_[1], &chest_data_[2]);
    c_ = &(m_->_map[chest_data_[0]][chest_data_[1]]);
    tile_add_chest(c_, chest_data_[2]);
    m_->_map[chest_data_[0]][chest_data_[1]] = *c_;
    tile_identify(&(m_->_map[chest_data_[0]][chest_data_[1]]));
}

FILE* world_open_map_file(char file_name_[20])
{
    FILE* ptr;
    char map_path_[20];

    strcpy(map_path_, "map/");
    strcat(map_path_, file_name_);
    ptr = fopen(map_path_, "r");
    if(ptr == NULL)
    {
        printf("Couldn't load map: %s\n", map_path_);
        return NULL;
    }

    return ptr;
}

AreaMap* world_new_areamap(char area_name_[15])
{
    AreaMap* m_ = malloc(sizeof(AreaMap));
    // printf("Copying file name in world_new_areamap()\n");
    strcpy(m_->_area_name, area_name_);
    m_->_actor_count = 0;

    world_map_init(m_);

    return m_;
}

/*
    world_map_init()

    Init an AreaMap _map array with inactive zero Tile
    objects.
*/
void world_map_init(AreaMap* m_)
{
    Tile* t_;

    for(int r = 0; r < MAX_ROWS; r++)
    {
        for(int c = 0; c < MAX_COLS; c++)
        {
            // printf("Setting inactive tile on [%d][%d]...\t", r, c);
            t_ = tile_new(r, c, -1, -1, -1, 0, 0, false, "NULL");
            m_->_map[r][c] = *t_;
            // printf("DONE\n");
        }
    }

    // printf("Completed initiating new map.\n");
}

void world_draw_map(AreaMap* cur_)
{
    /* Init color based on map height */
    world_draw_background(cur_->_map_height);
    SDL_RenderClear(gRenderer);

    for(int r = 0; r < MAX_ROWS; r++)
    {
        for(int c = 0; c < MAX_COLS; c++)
        {
            if(cur_->_map[r][c]._is_active)
            {
                // Eventually this sets a texture?
                video_set_tile_texture(cur_->_map[r][c]._tile_texture);
                SDL_Rect this_tile_;
                this_tile_.x = c * tile_size;
                this_tile_.y = r * tile_size;
                this_tile_.w = tile_size;
                this_tile_.h = tile_size;

                SDL_RenderFillRect(gRenderer, &this_tile_);

                if(cur_->_map[r][c]._door_id > 0)
                {
                    SDL_SetRenderDrawColor(gRenderer, 0xC9, 0xC0, 0x18, 0x64);
                    SDL_RenderFillRect(gRenderer, &this_tile_);
                }

                if(cur_->_map[r][c]._chest_id > 0)
                {
                    SDL_SetRenderDrawColor(gRenderer, 0x40, 0x3D, 0x02, 0x64);
                    SDL_RenderFillRect(gRenderer, &this_tile_);
                }
            }
        }
    }
}

void world_draw_actors(AreaMap* m_)
{
    for(int i = 0; i < m_->_actor_count; i++)
    {
        Actor* a_ = &m_->_actors[i];

        // printf("%s:\nrow: %d\ty_pos: %d\ncol:%d\tx_pos:%d\n\n", a_->_name, a_->_row, a_->_y_pos, a_->_col, a_->_x_pos);
        world_draw_actor(a_);

        // free(a_);
    }
}

void world_draw_actor(Actor* a_)
{
    // printf("%s:\nrow: %d\ty_pos: %d\ncol:%d\tx_pos:%d\n\n", a_->_name, a_->_row, a_->_y_pos, a_->_col, a_->_x_pos);
    video_set_actor_texture(a_->_type_id);
    SDL_Rect this_actor_;
    this_actor_.x = a_->_x_pos;
    this_actor_.y = a_->_y_pos;
    this_actor_.w = tile_size;
    this_actor_.h = tile_size;

    SDL_RenderFillRect(gRenderer, &this_actor_);

    // free(a_);
}

void world_draw_background(int map_height_)
{
    SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xFF);

    if(map_height_ == 0)
    {
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0xFF, 0x0, 0xFF);
    }

    if(map_height_ > 0)
    {
        SDL_SetRenderDrawColor(gRenderer, 0xBF, 0xDD, 0xF7, 0xFF);
    }
}

void world_actors_move(AreaMap* m_)
{
    if(m_->_actor_count > 0)
    {
        for(int i = 0; i < m_->_actor_count; i++)
        {
            Actor* a_ = &m_->_actors[i];
            Tile* t_ = &(m_->_map[a_->_row][a_->_col]);
            // printf("Moving %s\n", a_->_name);
            if(t_->_is_passable)
            {
                tile_set_passable(t_, false);
            }

            if(actor_will_move(a_))
            {
                DestTile* d_ = tile_get_dest(a_->_col, a_->_row, rand_i(0, 4));
                world_actor_move(a_, m_, d_);
            }

            // printf("free(a_)\n");
            // free(a_);
        }
    }
}

void world_actor_move(Actor* a_, AreaMap* m_, DestTile* d_)
{
    // int col_ = d_->_col;
    // int row_ = d_->_row;
    Tile* t_ = &m_->_map[d_->_row][d_->_col];

    if(tile_is_passable(t_))
    {
        // printf("[%d, %d] is passable\n", row_, col_);
        if(a_->_row != d_->_row || a_->_col != d_->_col)
        {
            /* Get the values before they're updated */
            int col_ = a_->_col;
            int row_ = a_->_row;

            /* Update the actor */
            // printf("[%d, %d] is different\n", row_, col_);
            actor_move(a_, d_->_dir, d_->_row, d_->_col);

            /* Make destination tile impassable */
            tile_set_passable(t_, false);
            // printf("Tile [%d, %d] now impassable\n", d_->_row, d_->_col);

            /* Make current tile passable again */
            Tile* t_ = &m_->_map[row_][col_];
            tile_set_passable(t_, true);
            // printf("Tile [%d, %d] now passable\n", row_, col_);
        }
        else
        {
            actor_face(a_, d_->_dir);
        }
    }

    // printf("free(d_)\n");
    // free(d_);
    // printf("free(t_)\n");
    // free(t_);
}

void world_actors_update(AreaMap* m_)
{
    if(m_->_actor_count > 0)
    {
        for(int i = 0; i < m_->_actor_count; i++)
        {
            Actor* a_ = &m_->_actors[i];
            // printf("Updating %s\n", a_->_name);

            if(a_->_moving)
            {
                actor_update(a_, tile_size);
            }

            // Other stuff?

            // printf("update: free(a_)\n");
            // free(a_);
        }
    }
}

void world_hero_move(Actor* h_, AreaMap* m_)
{
    if(hero_move >= 0 && !h_->_moving)
    {
        DestTile* d_ = tile_get_dest(h_->_col, h_->_row, hero_move);
        world_actor_move(h_, m_, d_);

        hero_move = -1;
    }
}

void world_hero_update(Actor* h_)
{
    if(h_->_moving)
    {
        actor_update(h_, tile_size);
        return;
    }
}

bool world_area_transition(Actor* h_, AreaMap* m_)
{
    Tile* t_ = &m_->_map[h_->_row][h_->_col];
    if(!h_->_on_link && !h_->_moving && strcmp(t_->_link_map, "NULL") != 0)
    {
        printf("Hero on link tile:\n");
        printf("New map: %s\n", t_->_link_map);
        printf("Set hero location: [%d, %d]\n", t_->_link_row * tile_size, t_->_link_col * tile_size);
        printf("Set hero grid: [%d, %d]\n\n", t_->_link_row, t_->_link_col);
        actor_set_on_link(h_, true);
        actor_set_location(h_, t_->_link_col * tile_size, t_->_link_row * tile_size);
        actor_set_grid(h_, t_->_link_col, t_->_link_row);
        return true;
    }

    return false;
}

Tile* world_tile_lookup(AreaMap* m_, int row_, int col_)
{
    Tile* t_ = &m_->_map[row_][col_];
    return t_;
}