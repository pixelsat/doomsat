#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "doomsat.h"
#include "m_argv.h"

#include "doomgeneric.h"

pixel_t *DG_ScreenBuffer = NULL;

void M_FindResponseFile (void);
void D_DoomMain (void);

#if DOOMSAT_DOOMSTM
void
doomgeneric_Create (int argc, char **argv, const uint8_t *wad_data,
                    size_t wad_length)
{
    doomsat_wad_data = (byte *)wad_data;
    doomsat_wad_length = wad_length;

    // save arguments
    myargc = argc;
    myargv = argv;

    M_FindResponseFile ();

    DG_ScreenBuffer = malloc (DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

    DG_Init ();

    D_DoomMain ();
}
#endif

#if DOOMSAT_DOOMCLIENT
void
doomgeneric_Create (int argc, char **argv)
{

    // save arguments
    myargc = argc;
    myargv = argv;

    M_FindResponseFile ();

    DG_ScreenBuffer = malloc (DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

    DG_Init ();

    D_DoomMain ();
}
#endif