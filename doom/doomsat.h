#ifndef DOOMSAT
#define DOOMSAT

// doomsat_config is provided by meson so it won't be available when built by
// bindgen
#include "doomsat_config.h"

#include "doomdef.h"
#include "p_pspr.h"
#include <stdint.h>

struct doomsat_mobj
{
    int x;
    int y;
    int z;
    uint32_t angle;
    int sprite;
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

struct doomsat_psprite
{
    int sx;
    int sy;
    int state; // index into states array, see p_saveg.c:620
};

struct doomsat_mpoint
{
    int x;
    int y;
};

enum doomsat_menu
{
    DOOMSAT_MENU_NONE,
    DOOMSAT_MENU_MAIN,
    DOOMSAT_MENU_EPISODE,
    DOOMSAT_MENU_SKILL,
    DOOMSAT_MENU_OPTIONS,
    DOOMSAT_MENU_HELP1,
    DOOMSAT_MENU_HELP2,
    DOOMSAT_MENU_SOUND
};

struct doomsat_state
{
    int gameepisode;
    int gamemap;
    int gameskill;
    int gamestate;
    int gametic;
    int leveltime;
    boolean paused;
    int st_palette;
    char pagename[9];

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
    struct doomsat_psprite player_psprites[NUMPSPRITES];

    int setting_screenblocks;
    int setting_detailLevel;
    int setting_usegamma;
    int setting_showMessages;
    int setting_mouseSensitivity;
    int setting_sfxVolume;
    int setting_musicVolume;

    int hud_st_faceindex;
    int hud_message_on;
    int hud_message_line_length;
    char text[80];

    boolean automapactive;
    int automap_m_x;
    int automap_m_y;
    int automap_m_w;
    int automap_m_h;
    int automap_scale_mtof;
    int automap_grid;
    int automap_cheating;
    int automap_lightlev;
    struct doomsat_mpoint automap_markpoints[10];

    boolean menuactive;
    uint16_t menuid;
    short menu_itemOn;
    short menu_whichSkull;
    int menu_messageToPrint;
    int menu_dialog_length;
    char menu_dialog[160];

    int wi_state;
    int wi_epsd;
    boolean wi_didsecret;
    int wi_last;
    int wi_next;
    int wi_cnt_kills;
    int wi_cnt_items;
    int wi_cnt_secret;
    int wi_cnt_time;
    int wi_cnt_par;

    boolean wi_snl_pointeron;
    int wi_anim_ctr[10];

    int sectors_length;
    const struct doomsat_sector *sectors;

    int sides_length;
    const struct doomsat_side *sides;

    int lines_length;
    const struct doomsat_line *lines;

    int mobj_length;
    const struct doomsat_mobj *mobjs;
};

#if DOOMSAT_DOOMSTM
struct doomsat_state doomsat_GetState (void);

uint8_t *doomsat_wad_data;
size_t doomsat_wad_length;
#endif

#if DOOMSAT_DOOMCLIENT
void doomsat_Draw (struct doomsat_state state);
#endif

#endif