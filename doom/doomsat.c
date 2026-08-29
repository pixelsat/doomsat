#include "doomsat.h"
#include "am_map.h"
#include "d_main.h"
#include "deh_main.h"
#include "doomsat_strings.h"
#include "doomstat.h"
#include "f_finale.h"
#include "hu_stuff.h"
#include "i_system.h"
#include "i_video.h"
#include "info.h"
#include "m_menu.h"
#include "m_misc.h"
#include "p_local.h"
#include "p_mobj.h"
#include "p_pspr.h"
#include "p_setup.h"
#include "p_spec.h"
#include "r_defs.h"
#include "r_draw.h"
#include "r_main.h"
#include "r_state.h"
#include "st_stuff.h"
#include "v_video.h"
#include "w_wad.h"
#include "wi_stuff.h"
#include "z_zone.h"
#include <stdio.h>
#include <string.h>

extern int showMessages; // m_menu.c:73, not exported
extern int screenSize;   // :81
extern int st_palette;   // st_stuff.c:934
extern int st_faceindex; // :380
extern char *pagename;   // d_main.c:544
void R_ExecuteSetViewSize (void);

#if DOOMSAT_DOOMSTM
struct doomsat_mobj
doomsat_wire_thing (mobj_t *mobj)
{
    struct doomsat_mobj thing;
    thing.x = mobj->x;
    thing.y = mobj->y;
    thing.z = mobj->z;
    thing.angle = mobj->angle;
    thing.sprite = mobj->sprite;
    thing.frame = mobj->frame;
    thing.flags = mobj->flags;
    return thing;
}
struct doomsat_mobj *mobj_array = NULL;

struct doomsat_sector
doomsat_wire_sector (sector_t sector)
{
    struct doomsat_sector result;
    result.floorheight = sector.floorheight;
    result.ceilingheight = sector.ceilingheight;
    result.floorpic = sector.floorpic;
    result.ceilingpic = sector.ceilingpic;
    result.lightlevel = sector.lightlevel;
    return result;
}
struct doomsat_sector *sector_array = NULL;

struct doomsat_side
doomsat_wire_side (side_t sector)
{
    struct doomsat_side result;
    result.textureoffset = sector.textureoffset;
    result.rowoffset = sector.rowoffset;
    result.toptexture = sector.toptexture;
    result.bottomtexture = sector.bottomtexture;
    result.midtexture = sector.midtexture;
    return result;
}
struct doomsat_side *side_array = NULL;

struct doomsat_line
doomsat_wire_line (line_t line)
{
    struct doomsat_line result;
    result.flags = line.flags;
    return result;
}
struct doomsat_line *line_array = NULL;

struct doomsat_psprite
doomsat_wire_psprite (pspdef_t psprite)
{
    struct doomsat_psprite result;
    result.sx = psprite.sx;
    result.sy = psprite.sy;
    if (psprite.state == NULL)
        result.state = -1;
    else
        result.state = psprite.state - states;
    return result;
}
struct doomsat_psprite *psprite_array = NULL;

struct doomsat_state
doomsat_GetState (void)
{
    // MARK: - thinkers
    int level_active = gamestate == GS_LEVEL;
    int num_thinkers = 0;
    int state_numsectors = level_active ? numsectors : 0;
    int state_numsides = level_active ? numsides : 0;
    int state_numlines = level_active ? numlines : 0;
    struct thinker_s *thinkercap_original = &thinkercap;

    if (level_active)
        {
            for (struct thinker_s *ptr = thinkercap.next;
                 ptr != thinkercap_original; ptr = ptr->next)
                {
                    if (ptr->function.acp1 == (actionf_p1)P_MobjThinker)
                        num_thinkers++;
                }
        }

    if (mobj_array != NULL)
        {
            Z_Free (mobj_array);
            mobj_array = NULL;
        }
    if (num_thinkers > 0)
        {
            mobj_array = Z_Malloc (sizeof (struct doomsat_mobj) * num_thinkers,
                                   PU_STATIC, &mobj_array);
            struct doomsat_mobj *thing_arr_ptr = mobj_array;
            for (struct thinker_s *ptr = thinkercap.next;
                 ptr != thinkercap_original; ptr = ptr->next)
                {
                    if (ptr->function.acp1 == (actionf_p1)P_MobjThinker)
                        {
                            *thing_arr_ptr
                                = doomsat_wire_thing ((mobj_t *)(ptr));
                            thing_arr_ptr++;
                        }
                }
        }

    // MARK: - sectors
    if (sector_array != NULL)
        {
            Z_Free (sector_array);
            sector_array = NULL;
        }
    if (state_numsectors > 0)
        {
            sector_array
                = Z_Malloc (sizeof (struct doomsat_sector) * state_numsectors,
                            PU_STATIC, &sector_array);
            struct doomsat_sector *sector_arr_ptr = sector_array;
            sector_t *sector_src_ptr = sectors;
            for (int i = 0; i < state_numsectors; i++)
                {
                    *sector_arr_ptr = doomsat_wire_sector (*sector_src_ptr);
                    sector_arr_ptr++;
                    sector_src_ptr++;
                }
        }

    // MARK: - sides
    if (side_array != NULL)
        {
            Z_Free (side_array);
            side_array = NULL;
        }
    if (state_numsides > 0)
        {
            side_array
                = Z_Malloc (sizeof (struct doomsat_side) * state_numsides,
                            PU_STATIC, &side_array);
            struct doomsat_side *side_arr_ptr = side_array;
            side_t *side_src_ptr = sides;
            for (int i = 0; i < state_numsides; i++)
                {
                    *side_arr_ptr = doomsat_wire_side (*side_src_ptr);
                    side_arr_ptr++;
                    side_src_ptr++;
                }
        }

    // MARK: - sides
    if (line_array != NULL)
        {
            Z_Free (line_array);
            line_array = NULL;
        }
    if (state_numlines > 0)
        {
            line_array
                = Z_Malloc (sizeof (struct doomsat_line) * state_numlines,
                            PU_STATIC, &line_array);
            struct doomsat_line *line_arr_ptr = line_array;
            line_t *line_src_ptr = lines;
            for (int i = 0; i < state_numlines; i++)
                {
                    *line_arr_ptr = doomsat_wire_line (*line_src_ptr);
                    line_arr_ptr++;
                    line_src_ptr++;
                }
        }

    player_t *player = &players[displayplayer];

    // MARK: - construct state
    struct doomsat_state state;
    state.gameepisode = gameepisode;
    state.gamemap = gamemap;
    state.gameskill = gameskill;
    state.gamestate = gamestate;
    state.gametic = gametic;
    state.leveltime = leveltime;
    state.paused = paused;
    state.st_palette = st_palette;
    M_StringCopy (state.pagename, pagename, sizeof (state.pagename));

    state.viewx = viewx;
    state.viewy = viewy;
    state.viewz = viewz;
    state.viewangle = viewangle;

    state.player_viewz = player->viewz;
    state.player_extralight = player->extralight;
    state.player_fixedcolormap = player->fixedcolormap;

    state.player_health = player->health;
    state.player_armorpoints = player->armorpoints;
    memcpy (state.player_ammo, player->ammo, sizeof (player->ammo));
    memcpy (state.player_maxammo, player->maxammo, sizeof (player->maxammo));
    state.player_readyweapon = player->readyweapon;
    memcpy (state.player_weaponowned, player->weaponowned,
            sizeof (player->weaponowned));
    memcpy (state.player_cards, player->cards, sizeof (player->cards));
    memcpy (state.player_frags, player->frags, sizeof (player->frags));
    memcpy (state.player_powers, player->powers, sizeof (player->powers));
    for (int i = 0; i < NUMPSPRITES; i++)
        {
            state.player_psprites[i]
                = doomsat_wire_psprite (player->psprites[i]);
        }

    state.setting_screenblocks = screenblocks;
    state.setting_detailLevel = detailLevel;
    state.setting_usegamma = usegamma;
    state.setting_showMessages = showMessages;
    state.setting_mouseSensitivity = mouseSensitivity;
    state.setting_sfxVolume = sfxVolume;
    state.setting_musicVolume = musicVolume;

    state.hud_st_faceindex = st_faceindex;
    HU_DoomsatGetMessage (&state);

    state.automapactive = automapactive;
    AM_DoomsatGetState (&state);

    state.menuactive = menuactive;
    state.menuid = M_DoomsatWireMenu ();
    M_DoomsatWireDialog (&state);

    WI_DoomsatGetState (&state);

    state.sectors_length = state_numsectors;
    state.sectors = sector_array;

    state.sides_length = state_numsides;
    state.sides = side_array;

    state.lines_length = state_numlines;
    state.lines = line_array;

    state.mobj_length = num_thinkers;
    state.mobjs = mobj_array;
    return state;
}
#endif

#if DOOMSAT_DOOMCLIENT
mobj_t *mobj_storage = NULL;
int last_map = INT_MIN;
int last_gamestate = INT_MIN;

char pagename_storage[9];
void
doomsat_LoadState (struct doomsat_state state)
{
    gameepisode = state.gameepisode;
    gamemap = state.gamemap;
    if (state.gamestate == GS_LEVEL && gamemap != last_map)
        {
            P_SetupLevel (state.gameepisode, state.gamemap, 0,
                          state.gameskill);
            last_map = gamemap;
        }
    gameskill = state.gameskill;
    gamestate = state.gamestate;
    gametic = state.gametic;
    leveltime = state.leveltime;
    paused = state.paused;
    menuactive = state.menuactive;
    if (state.st_palette != st_palette)
        {
            st_palette = state.st_palette;
            ST_DoomsatLoadPalette ();
        }
    memcpy(pagename_storage, state.pagename, sizeof(pagename_storage));
    pagename = pagename_storage;

    if (state.gamestate != GS_LEVEL)
        {
            screenblocks = state.setting_screenblocks;
            screenSize = screenblocks - 3;
            detailLevel = state.setting_detailLevel;
            usegamma = state.setting_usegamma;
            showMessages = state.setting_showMessages;
            mouseSensitivity = state.setting_mouseSensitivity;
            sfxVolume = state.setting_sfxVolume;
            musicVolume = state.setting_musicVolume;

            M_DoomsatLoadMenu (state.menuid);
            M_DoomsatLoadDialog (&state);
            return;
        }

    player_t *player = &players[displayplayer];

    player->mo->x = state.viewx;
    player->mo->y = state.viewy;
    player->mo->subsector = R_PointInSubsector (state.viewx, state.viewy);
    player->viewz = state.viewz;
    player->mo->angle = state.viewangle;

    player->viewz = state.player_viewz;
    player->extralight = state.player_extralight;
    player->fixedcolormap = state.player_fixedcolormap;

    player->health = state.player_health;
    player->armorpoints = state.player_armorpoints;
    memcpy (player->ammo, state.player_ammo, sizeof (state.player_ammo));
    memcpy (player->maxammo, state.player_maxammo,
            sizeof (state.player_maxammo));
    player->readyweapon = state.player_readyweapon;
    memcpy (player->weaponowned, state.player_weaponowned,
            sizeof (state.player_weaponowned));
    memcpy (player->cards, state.player_cards, sizeof (state.player_cards));
    memcpy (player->frags, state.player_frags, sizeof (state.player_frags));
    memcpy (player->powers, state.player_powers, sizeof (state.player_powers));
    for (int i = 0; i < NUMPSPRITES; ++i)
        {
            const struct doomsat_psprite *src = &state.player_psprites[i];
            pspdef_t *dst = &player->psprites[i];

            dst->sx = src->sx;
            dst->sy = src->sy;

            if (src->state == -1)
                dst->state = NULL;
            else if (src->state < NUMSTATES)
                dst->state = &states[src->state];
            else
                I_Error ("invalid psprite state");
        }

    screenblocks = state.setting_screenblocks;
    screenSize = screenblocks - 3;
    detailLevel = state.setting_detailLevel;
    usegamma = state.setting_usegamma;
    showMessages = state.setting_showMessages;
    mouseSensitivity = state.setting_mouseSensitivity;
    sfxVolume = state.setting_sfxVolume;
    musicVolume = state.setting_musicVolume;

    st_faceindex = state.hud_st_faceindex;
    HU_DoomsatLoadMessage (&state);

    automapactive = state.automapactive;
    AM_DoomsatLoadState (&state);

    M_DoomsatLoadMenu (state.menuid);
    M_DoomsatLoadDialog (&state);

    WI_DoomsatLoadState (&state);
    if (state.wi_state != last_gamestate)
        WI_loadData ();
    last_gamestate = state.wi_state;

    if (state.sectors_length != numsectors)
        I_Error ("sector count mismatch");

    for (int i = 0; i < numsectors; ++i)
        {
            const struct doomsat_sector *src = &state.sectors[i];
            sector_t *dst = &sectors[i];

            dst->floorheight = src->floorheight;
            dst->ceilingheight = src->ceilingheight;
            dst->floorpic = src->floorpic;
            dst->ceilingpic = src->ceilingpic;
            dst->lightlevel = src->lightlevel;

            dst->thinglist = NULL;
        }

    if (state.sides_length != numsides)
        {
            I_Error ("side count mismatch");
        }

    for (int i = 0; i < numsides; ++i)
        {
            const struct doomsat_side *src = &state.sides[i];
            side_t *dst = &sides[i];

            dst->textureoffset = src->textureoffset;
            dst->rowoffset = src->rowoffset;
            dst->toptexture = src->toptexture;
            dst->bottomtexture = src->bottomtexture;
            dst->midtexture = src->midtexture;
        }

    if (state.lines_length != numlines)
        {
            I_Error ("line count mismatch");
        }

    for (int i = 0; i < numlines; ++i)
        {
            lines[i].flags = state.lines[i].flags;
        }

    for (int i = 0; i < numsectors; ++i)
        {
            sectors[i].thinglist = NULL;
        }

    if (mobj_storage)
        {
            Z_Free (mobj_storage);
            mobj_storage = NULL;
        }
    mobj_storage = Z_Malloc (sizeof (mobj_t) * state.mobj_length, PU_STATIC,
                             mobj_storage);

    for (int i = 0; i < state.mobj_length; ++i)
        {
            const struct doomsat_mobj *src = &state.mobjs[i];

            mobj_t *mo = &mobj_storage[i];
            mo->x = src->x;
            mo->y = src->y;
            mo->z = src->z;
            mo->angle = src->angle;
            mo->sprite = src->sprite;
            mo->frame = src->frame;
            mo->flags = src->flags;

            mo->subsector = R_PointInSubsector (mo->x, mo->y);

            sector_t *sector = mo->subsector->sector;
            mo->snext = sector->thinglist;
            mo->sprev = NULL;

            if (mo->snext != NULL)
                mo->snext->sprev = mo;

            sector->thinglist = mo;
        }
}

void
doomsat_Draw (struct doomsat_state state)
{
    static int last_screenblocks = -1;
    static int last_detaillevel = -1;
    static int last_gamma = -1;
    int pause_y;

    doomsat_LoadState (state);

    if (screenblocks != last_screenblocks || detailLevel != last_detaillevel)
        {
            R_SetViewSize (screenblocks, detailLevel);
            R_ExecuteSetViewSize ();
            R_FillBackScreen ();
            last_screenblocks = screenblocks;
            last_detaillevel = detailLevel;
        }

    if (usegamma != last_gamma)
        {
            ST_DoomsatLoadPalette ();
            last_gamma = usegamma;
        }

    if (gamestate == GS_LEVEL)
        {
            int animationtime = leveltime > 0 ? leveltime - 1 : 0;
            P_UpdatePicAnimations (animationtime);
        }

    if (gamestate == GS_LEVEL && gametic)
        HU_Erase ();

    switch (gamestate)
        {
        case GS_LEVEL:
            if (!gametic)
                break;

            if (automapactive)
                {
                    AM_DoomsatInitRenderer ();
                    AM_Drawer ();
                }

            ST_Drawer (viewheight == SCREENHEIGHT, false);
            break;

        case GS_INTERMISSION:
            WI_Drawer ();
            break;

        case GS_FINALE:
            F_Drawer ();
            break;

        case GS_DEMOSCREEN:
            D_PageDrawer ();
            break;
        }

    I_UpdateNoBlit ();

    if (gamestate == GS_LEVEL && gametic)
        {
            if (!automapactive)
                {
                    R_RenderPlayerView (&players[displayplayer]);

                    if (scaledviewwidth != SCREENWIDTH)
                        R_DrawViewBorder ();
                }

            HU_Drawer ();
        }

    if (paused)
        {
            pause_y = automapactive ? 4 : viewwindowy + 4;
            V_DrawPatchDirect (
                viewwindowx + (scaledviewwidth - 68) / 2, pause_y,
                W_CacheLumpName (DEH_String ("M_PAUSE"), PU_CACHE));
        }

    M_Drawer ();
    I_FinishUpdate ();
}
#endif
