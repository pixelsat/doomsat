#include "doomsat.h"
#include "doomstat.h"
#include "i_system.h"
#include "info.h"
#include "p_local.h"
#include "p_mobj.h"
#include "p_pspr.h"
#include "r_defs.h"
#include "r_main.h"
#include "r_state.h"
#include "z_zone.h"
#include <stdio.h>
#include <string.h>

#ifdef DOOMSAT_DOOMSTM
struct doomsat_mobj
doomsat_wire_thing (mobj_t *mobj)
{
    struct doomsat_mobj thing;
    thing.x = mobj->x;
    thing.y = mobj->y;
    thing.z = mobj->z;
    thing.angle = mobj->angle;
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
    if (psprite.state != NULL)
        result.state = -1;
    else
        result.state = psprite.state - states;
    return result;
}
struct doomsat_psprite *psprite_array = NULL;

struct doomsat_state
doomsat_State (void)
{
    // MARK: - thinkers
    int num_thinkers = 0;
    struct thinker_s *thinkercap_original = &thinkercap;
    for (struct thinker_s *ptr = thinkercap.next; ptr != thinkercap_original;
         ptr = ptr->next)
        {
            if (ptr->function.acp1 == (actionf_p1)P_MobjThinker)
                {
                    num_thinkers++;
                }
        }

    if (mobj_array != NULL)
        {
            Z_Free (mobj_array);
            mobj_array = NULL;
        }
    mobj_array = Z_Malloc (sizeof (struct doomsat_mobj) * num_thinkers,
                           PU_STATIC, &mobj_array);
    struct doomsat_mobj *thing_arr_ptr = mobj_array;
    for (struct thinker_s *ptr = thinkercap.next; ptr != thinkercap_original;
         ptr = ptr->next)
        {
            if (ptr->function.acp1 == (actionf_p1)P_MobjThinker)
                {
                    *thing_arr_ptr = doomsat_wire_thing ((mobj_t *)(ptr));
                    thing_arr_ptr++;
                }
        }

    // MARK: - sectors
    if (sector_array != NULL)
        {
            Z_Free (sector_array);
            sector_array = NULL;
        }
    sector_array = Z_Malloc (sizeof (struct doomsat_sector) * numsectors,
                             PU_STATIC, &sector_array);
    struct doomsat_sector *sector_arr_ptr = sector_array;
    sector_t *sector_src_ptr = sectors;
    for (int i = 0; i < numsectors; i++)
        {
            *sector_arr_ptr = doomsat_wire_sector (*sector_src_ptr);
            sector_arr_ptr++;
            sector_src_ptr++;
        }
    fflush (stdout);

    // MARK: - sides
    if (side_array != NULL)
        {
            Z_Free (side_array);
            side_array = NULL;
        }
    side_array = Z_Malloc (sizeof (struct doomsat_side) * numsides, PU_STATIC,
                           &side_array);
    struct doomsat_side *side_arr_ptr = side_array;
    side_t *side_src_ptr = sides;
    for (int i = 0; i < numsides; i++)
        {
            *side_arr_ptr = doomsat_wire_side (*side_src_ptr);
            side_arr_ptr++;
            side_src_ptr++;
        }

    // MARK: - sides
    if (line_array != NULL)
        {
            Z_Free (line_array);
            line_array = NULL;
        }
    line_array = Z_Malloc (sizeof (struct doomsat_line) * numlines, PU_STATIC,
                           &line_array);
    struct doomsat_line *line_arr_ptr = line_array;
    line_t *line_src_ptr = lines;
    for (int i = 0; i < numlines; i++)
        {
            *line_arr_ptr = doomsat_wire_line (*line_src_ptr);
            line_arr_ptr++;
            line_src_ptr++;
        }

    player_t *player = &players[displayplayer];

    // MARK: - construct state
    struct doomsat_state state;
    state.gamestate = gamestate;
    state.gametic = gametic;
    state.leveltime = leveltime;
    state.paused = paused;
    state.automapactive = automapactive;
    state.menuactive = menuactive;

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
    state.player_message = 0;

    state.sectors_length = numsectors;
    state.sectors = sector_array;

    state.sides_length = numsides;
    state.sides = side_array;

    state.lines_length = numlines;
    state.lines = line_array;

    for (int i = 0; i < NUMPSPRITES; i++)
        {
            state.player_psprites[i]
                = doomsat_wire_psprite (player->psprites[i]);
        }

    state.mobj_length = num_thinkers;
    state.mobjs = mobj_array;
    return state;
}
#endif

#ifdef DOOMSAT_DOOMCLIENT
mobj_t *mobj_storage = NULL;

void
doomsat_LoadState (struct doomsat_state state)
{
    gamestate = state.gamestate;
    gametic = state.gametic;
    leveltime = state.leveltime;
    paused = state.paused;
    automapactive = state.automapactive;
    menuactive = state.menuactive;

    viewx = state.viewx;
    viewy = state.viewy;
    viewz = state.viewz;
    viewangle = state.viewangle;

    player_t *player = &players[displayplayer];
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
    player->message = "message"; // TODO: implement properly

    numsectors = state.sectors_length;
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
            lines[numlines].flags = state.lines[i].flags;
        }

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

    for (int i = 0; i < numsectors; ++i)
        {
            sectors[i].thinglist = NULL;
        }

    if (mobj_storage)
        {
            Z_Free (mobj_storage);
            mobj_storage = NULL;
        }
    mobj_storage = Z_Malloc (sizeof (struct doomsat_mobj) * state.mobj_length,
                             PU_STATIC, mobj_storage);

    for (int i = 0; i < state.mobj_length; ++i)
        {
            const struct doomsat_mobj *src = &state.mobjs[i];

            mobj_t *mo = &mobj_storage[i];
            mo->x = src->x;
            mo->y = src->y;
            mo->z = src->z;
            mo->angle = src->angle;
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
#endif