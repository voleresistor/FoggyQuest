/* dq_main.h */

#include "dq_world.h"       // Load and own area maps and main loop


// #define MIN(a,b) ((a) > (b)) ? b : a
// #define MAX(a,b) ((a) > (b)) ? a : b

extern int main(void);

/*
    TODO (In no particular order)

    01. Monsters
    02. Menus
    ***03. Inventory - Complete with simple int inventory
    ***03a. Stackable items? Using new struct and pointer chains
    03b. Party bag with large/unlimited space for silly shit?
    04. Chest contents
    05. Combat
    06. Dialogue
    07. Map editor
    08. Textures
    09. Sound
    10. Improve controls
    11. Cave light?
    12. Vehicles (Boat, balloon, etc)
    13. Multiple party members
    14. Items (including Weapons/Armor)
    15. Merchants
    16. Inns
    17. Doors
    18. Save/Load
    19. Title screen
    20. Spells
    21. Scripting API for cutscenes/chapters/Gwaelin's Love
    22. Move SDL calls to dq_sdl module
    23. organize c files
*/

/*
    NOTES

    Controls
        define functions for controls in dq_world
        dq_controls provides function pointers
        dq_world populates function pointers

        dq_world calls dq_sdl:event_handler directly to get *event updated
        dq_world:world_controls_handle_controls passes keypress to dq_controls
        dq_controls parses keypress to call appropriate function pointer
*/