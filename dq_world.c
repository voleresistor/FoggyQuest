/* dq_world.c */

#include "dq_world.h"

/*
    _init()
*/
int world_init_game(void)
{
    tile_cols       = 16;
    SCREEN_WIDTH    = 1024;
    SCREEN_HEIGHT   = 768;
    tile_size       = SCREEN_WIDTH / tile_cols;
    grid_rows       = SCREEN_HEIGHT / tile_size;
    grid_cols       = SCREEN_WIDTH / tile_size;

    key_wait_buffer = 0;

    in_menu = false;

    /*
        Init move speed based on scale

        divide tile size by number of frames of animation. More
        frames results in slower movement.
    */
    move_speed      = tile_size / MOVE_UPDATES;

    if(init_sdl() != 0)
    {
        return EXIT_FAILURE;
    }

    /* Define game control function pointers */
    key_a = &world_action_left;
    key_b = &world_action_unbound;
    key_c = &world_action_cancel;
    key_d = &world_action_right;
    key_e = &world_action_select;
    key_f = &world_action_unbound;
    key_g = &world_action_unbound;
    key_h = &world_action_unbound;
    key_i = &world_action_unbound;
    key_j = &world_action_unbound;
    key_q = &world_action_interact;
    key_s = &world_action_down;
    key_w = &world_action_up;
    key_x = &world_action_unbound;

    //world_main_menu();
    int game_result = world_main_loop();
    close_sdl();

    return game_result;
}

int world_main_loop(void)
{
    /*
        load a test area map

        Eventually this will be fed a map name from
        a loaded savegame file.
    */
    AreaMap* _area_map = world_load_area("testmap1f");

    /*
        Init the hero

        This will also be initialized from a loaded
        savegame file.
    */
    Actor* _hero = actor_new("Buns", HERO, 0, false, 1, 1, tile_size);
    _hero->_idle_time = 0;
    _hero->_level = 5;
    _hero->_xp = 2334;
    _hero->_cur_hp = 41;
    _hero->_cur_mp = 30;
    _hero->_gold = 312;

    //actor_identify(_hero);

    quit = false;
    while(!quit)
    {
        /* Handle events so we can close the window */
        // event_handler();
        // world_controls_handle_controls();
        controls_handle_input();

        /*
            Transition map if necessary
        */
        Tile* _t = world_area_transition(_hero, _area_map);
        if(_t != NULL)
        {
            // printf("Transition: %s\n", _t->_link_map);
            world_actor_transition(_hero, _t);
            _area_map = world_load_area(_t->_link_map);
        }

        /* Update the hero */
        world_hero_move(_hero, _area_map);
        // world_hero_update(_hero);

        /* Update the actors */
        world_actors_move(_area_map);
        // world_actors_update(_area_map);

        /* Draw the window */
        world_draw_map(_area_map);
        world_draw_actor(_hero);
        world_draw_actors(_area_map);
        world_menu_draw_idle(_hero);
        world_menu_draw_action();
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
    int c;
    int link_data_[4];

    char buf[255];
    char link_name_[15];
    char map_path_[20];

    Actor* a_;
    Tile* t_;
    Tile* l_;
    

    /* Init new map */
    AreaMap* m_ = world_new_areamap(file_name_);

    /* Open map file */
    fptr = world_open_map_file(file_name_);

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
            m_->_map[t_->_row][t_->_col] = *t_;
            // tile_identify(&(m_->_map[t_->_row][t_->_col]));
            break;

            case 'a':
            // printf("Actor: %s\n", buf);
            a_ = actor_load(buf);
            actor_set_location(a_, a_->_col * tile_size, a_->_row * tile_size);
            m_->_actors[m_->_actor_count] = *a_;
            //actor_identify(&(m_->_actors[m_->_actor_count]));
            m_->_actor_count++;
            break;

            case 'c':
            // printf("Chest: %s\n", buf);
            world_tile_add_chest(m_, buf);
            break;

            case 'd':
            // printf("Door: %s\n", buf);
            world_tile_add_door(m_, buf);
            break;

            case 'm':
            // printf("Maplink: %s\n", buf);
            world_tile_add_link(m_, buf);
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
    //tile_identify(&(m_->_map[link_data_[0]][link_data_[1]]));
}

void world_tile_add_door(AreaMap* m_, char b[255])
{
    Tile* d_;
    int door_data_[3];

    sscanf(b, "%d:%d:%d", &door_data_[0], &door_data_[1], &door_data_[2]);
    d_ = &(m_->_map[door_data_[0]][door_data_[1]]);
    tile_add_door(d_, door_data_[2]);
    m_->_map[door_data_[0]][door_data_[1]] = *d_;
    //tile_identify(&(m_->_map[door_data_[0]][door_data_[1]]));
}

void world_tile_add_chest(AreaMap* m_, char b[255])
{
    Tile* c_;
    int chest_data_[3];

    sscanf(b, "%d:%d:%d", &chest_data_[0], &chest_data_[1], &chest_data_[2]);
    c_ = &(m_->_map[chest_data_[0]][chest_data_[1]]);
    tile_add_chest(c_, chest_data_[2]);
    m_->_map[chest_data_[0]][chest_data_[1]] = *c_;
    //tile_identify(&(m_->_map[chest_data_[0]][chest_data_[1]]));
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
    // Don't allow actor movement in menus
    if(in_menu)
    {
        return;
    }

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
        }
    }

    world_actors_update(m_);
}

void world_actor_move(Actor* a_, AreaMap* m_, DestTile* d_)
{
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
        }
    }
}

void world_hero_move(Actor* h_, AreaMap* m_)
{
    // Don't allow actor movement in menus
    if(in_menu)
    {
        hero_move = -1;
        h_->_idle_time = 0;
        return;
    }

    if(hero_move >= 0 && !h_->_moving)
    {
        DestTile* d_ = tile_get_dest(h_->_col, h_->_row, hero_move);
        world_actor_move(h_, m_, d_);
        h_->_idle_time = 0;

        hero_move = -1;
    }

    world_hero_update(h_);
}

void world_hero_update(Actor* h_)
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

Tile* world_area_transition(Actor* h_, AreaMap* m_)
{
    Tile* t_ = &m_->_map[h_->_row][h_->_col];
    if(!h_->_on_link && !h_->_moving && strcmp(t_->_link_map, "NULL") != 0)
    {
        return t_;
    }

    return NULL;
}

void world_actor_transition(Actor* a_, Tile* t_)
{
    // printf("Hero on link tile:\n");
    // printf("New map: %s\n", t_->_link_map);
    // printf("Set hero location: [%d, %d]\n", t_->_link_row * tile_size, t_->_link_col * tile_size);
    // printf("Set hero grid: [%d, %d]\n\n", t_->_link_row, t_->_link_col);
    actor_set_on_link(a_, true);
    actor_set_location(a_, t_->_link_col * tile_size, t_->_link_row * tile_size);
    actor_set_grid(a_, t_->_link_col, t_->_link_row);
}

Tile* world_tile_lookup(AreaMap* m_, int row_, int col_)
{
    Tile* t_ = &m_->_map[row_][col_];
    return t_;
}

void world_menu_draw_idle(Actor* a_)
{
    if(a_->_idle_time >= IDLE_DELAY || in_menu)
    {
        /* Menu background */
        SDL_Rect idle_menu_bg;
        idle_menu_bg.x = SCREEN_WIDTH / 20;
        idle_menu_bg.y = SCREEN_HEIGHT / 12;
        idle_menu_bg.h = SCREEN_HEIGHT / 2;
        idle_menu_bg.w = SCREEN_WIDTH / 4;
        video_draw_menu_window(&idle_menu_bg);

        char data_int[7];
        get_int_string(a_->_level, data_int, 6);

        int y_space = 64;
        video_draw_text(a_->_name, (SCREEN_WIDTH / 20 + idle_menu_bg.w) / 2 - strlen(a_->_name) * 8, idle_menu_bg.y + 10);

        /* Level */
        video_draw_text("LV", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space);
        get_int_string(a_->_level, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space);

        /* HP */
        video_draw_text("HP", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space * 2);
        get_int_string(a_->_cur_hp, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space * 2);

        /* MP */
        video_draw_text("MP", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space * 3);
        get_int_string(a_->_cur_mp, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space * 3);

        /* Gold */
        video_draw_text("GD", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space * 4);
        get_int_string(a_->_gold, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space * 4);

        /* XP */
        video_draw_text("XP", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space * 5);
        get_int_string(a_->_xp, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space * 5);

    }
}

void get_int_string(int i, char* data, int buf)
{
    char retstr[buf];
    sprintf(retstr, "% 5d", i);

    strncpy(data, retstr, buf - 1);
    data[buf - 1] = '\0';
}

void world_menu_draw_action(void)
{
    if(in_menu)
    {
        SDL_Rect action_menu_bg;
        action_menu_bg.x = (SCREEN_WIDTH / 5) * 2;
        action_menu_bg.y = SCREEN_HEIGHT / 10;
        action_menu_bg.h = SCREEN_HEIGHT / 3.5;
        action_menu_bg.w = SCREEN_WIDTH / 2.5;
        video_draw_menu_window(&action_menu_bg);

        int y_space = 48;

        /* Level */
        video_draw_text("TALK", (SCREEN_WIDTH / 5) * 2 + 40, action_menu_bg.y + y_space);
        video_draw_text("SPELL", (SCREEN_WIDTH / 5) * 2 + action_menu_bg.w - 140, action_menu_bg.y + y_space);
        video_draw_text("STATUS", (SCREEN_WIDTH / 5) * 2 + 40, action_menu_bg.y + y_space * 2);
        video_draw_text("ITEM", (SCREEN_WIDTH / 5) * 2 + action_menu_bg.w - 140, action_menu_bg.y + y_space * 2);
        video_draw_text("DOOR", (SCREEN_WIDTH / 5) * 2 + 40, action_menu_bg.y + y_space * 3);
        video_draw_text("SEARCH", (SCREEN_WIDTH / 5) * 2 + action_menu_bg.w - 140, action_menu_bg.y + y_space * 3);
    }
}

void world_action_up(void)
{
    printf("world_action_up()\n");
    if(in_menu)
    {

    }
    else
    {
        hero_move = 0;
    }
}

void world_action_down(void)
{
    printf("world_action_down()\n");
    if(in_menu)
    {

    }
    else
    {
        hero_move = 2;
    }
}

void world_action_left(void)
{
    printf("world_action_left()\n");
    if(in_menu)
    {

    }
    else
    {
        hero_move = 3;
    }
}

void world_action_right(void)
{
    printf("world_action_right()\n");
    if(in_menu)
    {

    }
    else
    {
        hero_move = 1;
    }
}

void world_action_interact(void)
{
    printf("world_action_interact()\n");
}

void world_action_cancel(void)
{
    printf("world_action_cancel()\n");
    if(in_menu)
    {
        in_menu = false;
    }
}

void world_action_select(void)
{
    printf("world_action_select()\n");
    if(!in_menu)
    {
        in_menu = true;
    }
}

void world_action_unbound(void)
{
    return;
}