#include "m_config.h"

char *configdir = "";

void
M_LoadDefaults (void)
{
}

void
M_SaveDefaults (void)
{
}

void
M_SaveDefaultsAlternate (char *main, char *extra)
{
}

void
M_SetConfigDir (char *dir)
{
}

void
M_BindVariable (char *name, void *variable)
{
}

boolean
M_SetVariable (char *name, char *value)
{
    return false;
}

int
M_GetIntVariable (char *name)
{
    return 0;
}

const char *
M_GetStrVariable (char *name)
{
    return 0;
}

float
M_GetFloatVariable (char *name)
{
    return 0;
}

void
M_SetConfigFilenames (char *main_config, char *extra_config)
{
}

char *
M_GetSaveGameDir (char *iwadname)
{
    return configdir;
}
