/* dq_world.c */

#include "dq_world.h"
#include "dq_tile.h"
#include "dq_map.h"
#include "dq_actor.h"
#include "dq_hero.h"

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

    controls_load_controls(NULL);

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
    _area_map = map_load_area("testmap1f", tile_size);

    /*
        Init the hero

        This will also be initialized from a loaded
        savegame file.
    */
    struct Hero* _hero = hero_new_hero("Buns", 1, 1, tile_size);
    _hero->_actor->_idle_time = 0;
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
        struct Tile* _t = map_area_transition(_hero->_actor, _area_map);
        if(_t != NULL)
        {
            // printf("Transition: %s\n", _t->_link_map);
            world_actor_transition(_hero->_actor, _t);
            _area_map = map_load_area(_t->_link_map, tile_size);
        }

        /* Update the hero */
        hero_move_hero(_hero->_actor, _area_map);
        // world_hero_update(_hero);

        /* Update the actors */
        world_actors_move(_area_map);
        // world_actors_update(_area_map);

        /* Draw the window */
        world_draw_map(_area_map);
        world_draw_actor(_hero->_actor);
        world_draw_actors(_area_map);
        world_menu_draw_idle(_hero);
        world_menu_draw_action();
        SDL_RenderPresent(gRenderer);

        /* Simple 30 fps delay for now */
        SDL_Delay(33);
    }

    return EXIT_SUCCESS;
}

void world_draw_map(struct AreaMap* cur_)
{
    /* Init color based on map height */
    world_draw_background(cur_->_map_height);
    SDL_RenderClear(gRenderer);

    for(int r = 0; r < MAX_ROWS; r++)
    {
        for(int c = 0; c < MAX_COLS; c++)
        {
            if(cur_->_map[r][c]->_is_active)
            {
                // Eventually this sets a texture?
                video_set_tile_texture(cur_->_map[r][c]->_tile_texture);
                SDL_Rect this_tile_;
                this_tile_.x = c * tile_size;
                this_tile_.y = r * tile_size;
                this_tile_.w = tile_size;
                this_tile_.h = tile_size;

                SDL_RenderFillRect(gRenderer, &this_tile_);

                if(cur_->_map[r][c]->_door_id > 0)
                {
                    SDL_SetRenderDrawColor(gRenderer, 0xC9, 0xC0, 0x18, 0x64);
                    SDL_RenderFillRect(gRenderer, &this_tile_);
                }

                if(cur_->_map[r][c]->_chest_id > 0)
                {
                    SDL_SetRenderDrawColor(gRenderer, 0x40, 0x3D, 0x02, 0x64);
                    SDL_RenderFillRect(gRenderer, &this_tile_);
                }
            }
        }
    }
}

void world_draw_actors(struct AreaMap* m_)
{
    for(int i = 0; i < m_->_actor_count; i++)
    {
        struct Actor* a_ = m_->_actors[i];

        // printf("%s:\nrow: %d\ty_pos: %d\ncol:%d\tx_pos:%d\n\n", a_->_name, a_->_row, a_->_y_pos, a_->_col, a_->_x_pos);
        world_draw_actor(a_);
    }
}

void world_draw_actor(struct Actor* a_)
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

void world_actors_move(struct AreaMap* m_)
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
            struct Actor* a_ = m_->_actors[i];
            struct Tile* t_ = m_->_map[a_->_row][a_->_col];
            // printf("Moving %s\n", a_->_name);
            /* Ensure that the tile the actor is standing on is impassable */
            if(t_->_is_passable)
            {
                tile_set_passable(t_, false);
            }

            if(actor_will_move(a_))
            {
                struct DestTile* d_ = tile_get_dest(a_->_col, a_->_row, rand_i(0, 4));
                struct Tile* dest = map_get_tile(d_->_row, d_->_col);
                struct Tile* cur = map_get_tile(a_->_row, a_->_col);
                actor_move_actor(a_, cur, dest, d_->_dir);
                free(d_);
            }
        }
    }

    world_actors_update(m_);
}

void world_actors_update(struct AreaMap* m_)
{
    if(m_->_actor_count > 0)
    {
        for(int i = 0; i < m_->_actor_count; i++)
        {
            struct Actor* a_ = m_->_actors[i];
            // printf("Updating %s\n", a_->_name);

            if(a_->_moving)
            {
                actor_update(a_, tile_size);
            }

            // Other stuff?
        }
    }
}

void world_actor_transition(struct Actor* a_, struct Tile* t_)
{
    // printf("Hero on link tile:\n");
    // printf("New map: %s\n", t_->_link_map);
    // printf("Set hero location: [%d, %d]\n", t_->_link_row * tile_size, t_->_link_col * tile_size);
    // printf("Set hero grid: [%d, %d]\n\n", t_->_link_row, t_->_link_col);
    actor_set_on_link(a_, true);
    actor_set_location(a_, t_->_link_col * tile_size, t_->_link_row * tile_size);
    actor_set_grid(a_, t_->_link_col, t_->_link_row);
}

struct Tile* world_tile_lookup(struct AreaMap* m_, int row_, int col_)
{
    struct Tile* t_ = m_->_map[row_][col_];
    return t_;
}

void world_menu_draw_idle(struct Hero* hero_)
{
    if(hero_->_actor->_idle_time >= IDLE_DELAY || in_menu)
    {
        /* Menu background */
        SDL_Rect idle_menu_bg;
        idle_menu_bg.x = SCREEN_WIDTH / 20;
        idle_menu_bg.y = SCREEN_HEIGHT / 12;
        idle_menu_bg.h = SCREEN_HEIGHT / 2;
        idle_menu_bg.w = SCREEN_WIDTH / 4;
        video_draw_menu_window(&idle_menu_bg);

        char data_int[7];
        get_int_string(hero_->_level, data_int, 6);

        int y_space = 64;
        video_draw_text(hero_->_actor->_name, (SCREEN_WIDTH / 20 + idle_menu_bg.w) / 2 - strlen(hero_->_actor->_name) * 8, idle_menu_bg.y + 10);

        /* Level */
        video_draw_text("LV", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space);
        get_int_string(hero_->_level, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space);

        /* HP */
        video_draw_text("HP", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space * 2);
        get_int_string(hero_->_cur_hp, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space * 2);

        /* MP */
        video_draw_text("MP", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space * 3);
        get_int_string(hero_->_cur_mp, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space * 3);

        /* Gold */
        video_draw_text("GD", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space * 4);
        get_int_string(hero_->_gold, data_int, 6);
        video_draw_text(data_int, (SCREEN_WIDTH / 20 + idle_menu_bg.w) - strlen(data_int) * 24 - 10, idle_menu_bg.y + y_space * 4);

        /* XP */
        video_draw_text("XP", SCREEN_WIDTH / 20 + 30, idle_menu_bg.y + y_space * 5);
        get_int_string(hero_->_xp, data_int, 6);
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