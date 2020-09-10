/* dq_menu.h */

enum GameMode {
    TESTMODE = 0,
    MAIN,
    MAP,
    COMBAT,
    IDLE,
    BUY
};

enum MapOpt {
    MENUTEST = 0,
    MENUTALK,
    MENUSPELL,
    MENUSTATUS,
    MENUITEM,
    MENUDOOR,
    MENUSEARCH
};

enum IdleOpt {
    TESTIDLEM = 0,
    NAME,
    LEVEL,
    HP,
    MP,
    GOLD,
    XP
};

typedef struct {
    char _name[20];
    int _level;
    int _hp;
    int _mp;
    int _gold;
    int _xp;
}IdleMenu;

/* GameMode control */
int game_mode;

// Delete?
extern void menu_open_menu(int game_mode_);

extern void menu_open_idle(char name_[15], int level_, int hp_, int mp_, int gold_, int xp_);