#ifndef DOOM_GENERIC
#define DOOM_GENERIC

#include <stdint.h>
#include <stdlib.h>

#ifndef DOOMGENERIC_RESX
#define DOOMGENERIC_RESX 640
#endif // DOOMGENERIC_RESX

#ifndef DOOMGENERIC_RESY
#define DOOMGENERIC_RESY 400
#endif // DOOMGENERIC_RESY

#ifdef CMAP256

typedef uint8_t pixel_t;

#else // CMAP256

typedef uint32_t pixel_t;

#endif // CMAP256

extern pixel_t *DG_ScreenBuffer;

#ifdef __cplusplus
extern "C"
{
#endif

#if DOOMSAT_DOOMSTM
    void doomgeneric_Create (int argc, char **argv, const uint8_t *wad_data,
                             size_t wad_length);
#endif
#if DOOMSAT_DOOMCLIENT
    void doomgeneric_Create (int argc, char **argv);
#endif
    void doomgeneric_Tick ();

    // Implement below functions for your platform
    extern void DG_Init ();
    extern void DG_DrawFrame ();
    extern void DG_SleepMs (uint32_t ms);
    extern uint32_t DG_GetTicksMs ();
    extern int DG_GetKey (int *pressed, unsigned char *key);
    extern void DG_SetWindowTitle (const char *title);

#ifdef __cplusplus
}
#endif

#endif // DOOM_GENERIC
