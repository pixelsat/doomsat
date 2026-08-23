#include <stdio.h>

#include "m_argv.h"

#include "doomgeneric.h"

#include "info.h"
#include "p_mobj.h"
#include "p_local.h"
#include "d_think.h"
#include "z_zone.h"

pixel_t* DG_ScreenBuffer = NULL;

void M_FindResponseFile(void);
void D_DoomMain (void);


void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;

	M_FindResponseFile();

	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

	DG_Init();

	D_DoomMain ();
}

struct doomsat_thing {
    int x;
    int y;
    int z;
    uint32_t angle;
    int frame;
    int flags;
};
struct doomsat_thing doomsat_wire_thing(mobj_t *mobj) {
    struct doomsat_thing thing;
    thing.x = mobj->x;
    thing.y = mobj->y;
    thing.z = mobj->z;
    thing.angle = mobj->angle;
    thing.frame = mobj->frame;
    thing.flags = mobj->flags;
    return thing;
}

struct doomsat_thing* thinkercap_array = NULL;

struct doomsat_state {
	int thinkercap_length;
	struct doomsat_thing* thinkercap;
};
struct doomsat_state doomsat_State() {
    printf("doomsat_state called");
    int num_thinkers = 0;
    struct thinker_s *thinkercap_original = &thinkercap;
    for (struct thinker_s *ptr = thinkercap.next; ptr != thinkercap_original; ptr = ptr->next) {
        if (ptr->function.acp1 == (actionf_p1)P_MobjThinker) {
            num_thinkers++;
        }
    }
    printf("num_thinkers: %d\n", num_thinkers);

    if (thinkercap_array != NULL) {
        Z_Free(thinkercap_array);
        thinkercap_array = NULL;
    }
    thinkercap_array = Z_Malloc(sizeof(struct doomsat_thing) * num_thinkers, PU_STATIC, &thinkercap_array);
    struct doomsat_thing *arr_ptr = thinkercap_array;
    for (struct thinker_s *ptr = thinkercap.next; ptr != thinkercap_original; ptr = ptr->next) {
        if (ptr->function.acp1 == (actionf_p1)P_MobjThinker) {
            *arr_ptr = doomsat_wire_thing((mobj_t *)(ptr));
            arr_ptr++;
        }
    }

    struct doomsat_state state;
    state.thinkercap_length = num_thinkers;
    state.thinkercap = thinkercap_array;
    return state;
}