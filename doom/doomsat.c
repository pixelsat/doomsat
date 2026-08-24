#include "doomsat.h"
#include "doomstat.h"
#include "info.h"
#include "p_local.h"
#include "p_mobj.h"
#include "p_pspr.h"
#include "r_defs.h"
#include "z_zone.h"
#include <stdio.h>
#include <string.h>

struct doomsat_thing
doomsat_wire_thing (mobj_t *mobj)
{
    struct doomsat_thing thing;
    thing.x = mobj->x;
    thing.y = mobj->y;
    thing.z = mobj->z;
    thing.angle = mobj->angle;
    thing.frame = mobj->frame;
    thing.flags = mobj->flags;
    return thing;
}
struct doomsat_thing *thinkercap_array = NULL;

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
    result.v1x = line.v1->x;
    result.v1y = line.v1->y;
    result.v2x = line.v2->x;
    result.v2y = line.v2->y;
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

    if (thinkercap_array != NULL)
        {
            Z_Free (thinkercap_array);
            thinkercap_array = NULL;
        }
    thinkercap_array = Z_Malloc (sizeof (struct doomsat_thing) * num_thinkers,
                                 PU_STATIC, &thinkercap_array);
    struct doomsat_thing *thing_arr_ptr = thinkercap_array;
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

    state.thinkercap_length = num_thinkers;
    state.thinkercap = thinkercap_array;
    return state;
}