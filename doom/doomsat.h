#ifndef DOOMSAT
#define DOOMSAT

// the two main macros which doomsat uses are
// 1. DOOMSAT_DOOMSTM
// 2. DOOMSAT_DOOMCLIENT
// will eventually port this to use meson

#define DOOMSAT_DOOMSTM
#define DOOMSAT_DOOMCLIENT

#include "doomdef.h"
#include "p_pspr.h"
#include <stdint.h>

struct doomsat_mobj
{
    int x;
    int y;
    int z;
    uint32_t angle;
    int frame;
    int flags;
};

struct doomsat_sector
{
    int floorheight;
    int ceilingheight;
    short floorpic;
    short ceilingpic;
    short lightlevel;
};

struct doomsat_side
{
    int textureoffset;
    int rowoffset;
    short toptexture;
    short bottomtexture;
    short midtexture;
};

struct doomsat_line
{
    short flags;
};

struct doomsat_psprite {
    int sx;
    int sy;
    int state; // index into states array, see p_saveg.c:620
};

struct doomsat_state
{
    int gamestate;
    int gametic;
    int leveltime;
    boolean paused;
    boolean automapactive;
    boolean menuactive;

    int viewx;
    int viewy;
    int viewz;
    uint32_t viewangle;

    int player_viewz;
    int player_extralight;
    int player_fixedcolormap;

    int player_health;
    int player_armorpoints;
    int player_ammo[NUMAMMO];
    int player_maxammo[NUMAMMO];
    int player_readyweapon;
    boolean player_weaponowned[NUMWEAPONS];
    boolean player_cards[NUMCARDS];
    int player_frags[MAXPLAYERS];
    int player_powers[NUMPOWERS];
    int player_message; // TODO: is actually a string, need to figure out how to represetn

    struct doomsat_psprite player_psprites[NUMPSPRITES];

    int sectors_length;
    const struct doomsat_sector *sectors;

    int sides_length;
    const struct doomsat_side *sides;

    int lines_length;
    const struct doomsat_line *lines;

    int mobj_length;
    const struct doomsat_mobj *mobjs;
};

#ifdef DOOMSAT_DOOMSTM
struct doomsat_state doomsat_GetState (void);
#endif

#ifdef DOOMSAT_DOOMCLIENT
void doomsat_LoadState (struct doomsat_state state);
#endif

#endif