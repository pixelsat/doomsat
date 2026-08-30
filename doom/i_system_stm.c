#include "config.h"
#include "doomtype.h"
#include "i_system.h"
#include "m_argv.h"
#include "m_misc.h"
#include "z_zone.h"
#include <stddef.h>
#include <string.h>

// never called lol
byte *
I_ZoneBase (int *size)
{
    return NULL;
}

boolean
I_ConsoleStdout (void)
{
    return false;
}

typedef struct atexit_listentry_s atexit_listentry_t;
struct atexit_listentry_s
{
    atexit_func_t func;
    boolean run_on_error;
    atexit_listentry_t *next;
};

static atexit_listentry_t *exit_funcs = NULL;

void
I_AtExit (atexit_func_t func, boolean run_on_error)
{
    atexit_listentry_t *entry;

    entry = Z_Malloc (sizeof (*entry), PU_STATIC, NULL);

    entry->func = func;
    entry->run_on_error = run_on_error;
    entry->next = exit_funcs;
    exit_funcs = entry;
}

void
I_Quit (void)
{
    atexit_listentry_t *entry;

    // Run through all exit functions

    entry = exit_funcs;

    while (entry != NULL)
        {
            entry->func ();
            entry = entry->next;
        }
}

static boolean already_quitting = false;

void
I_Error (char *error, ...)
{
    char msgbuf[512];
    va_list argptr;
    atexit_listentry_t *entry;
    boolean exit_gui_popup;

    if (already_quitting)
        {
            fprintf (stderr, "Warning: recursive call to I_Error detected.\n");
        }
    else
        {
            already_quitting = true;
        }

    // Message first.
    va_start (argptr, error);
    // fprintf(stderr, "\nError: ");
    vfprintf (stderr, error, argptr);
    fprintf (stderr, "\n\n");
    va_end (argptr);
    fflush (stderr);

    // Write a copy of the message into buffer.
    va_start (argptr, error);
    memset (msgbuf, 0, sizeof (msgbuf));
    M_vsnprintf (msgbuf, sizeof (msgbuf), error, argptr);
    va_end (argptr);

    // Shutdown. Here might be other errors.

    entry = exit_funcs;

    while (entry != NULL)
        {
            if (entry->run_on_error)
                {
                    entry->func ();
                }

            entry = entry->next;
        }

    exit_gui_popup = !M_ParmExists ("-nogui");

    // TODO(doomsat): setjmp/longjmp thingy
}

void
I_Tactile (int on, int off, int total)
{
}

#define DOS_MEM_DUMP_SIZE 10

static const unsigned char mem_dump_dos622[DOS_MEM_DUMP_SIZE]
    = { 0x57, 0x92, 0x19, 0x00, 0xF4, 0x06, 0x70, 0x00, 0x16, 0x00 };
static const unsigned char mem_dump_win98[DOS_MEM_DUMP_SIZE]
    = { 0x9E, 0x0F, 0xC9, 0x00, 0x65, 0x04, 0x70, 0x00, 0x16, 0x00 };
static const unsigned char mem_dump_dosbox[DOS_MEM_DUMP_SIZE]
    = { 0x00, 0x00, 0x00, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00 };
static unsigned char mem_dump_custom[DOS_MEM_DUMP_SIZE];

static const unsigned char *dos_mem_dump = mem_dump_dos622;
boolean
I_GetMemoryValue (unsigned int offset, void *value, int size)
{
    static boolean firsttime = true;

    if (firsttime)
        {
            int p, i, val;

            firsttime = false;
            i = 0;

            //!
            // @category compat
            // @arg <version>
            //
            // Specify DOS version to emulate for NULL pointer dereference
            // emulation.  Supported versions are: dos622, dos71, dosbox.
            // The default is to emulate DOS 7.1 (Windows 98).
            //

            p = M_CheckParmWithArgs ("-setmem", 1);

            if (p > 0)
                {
                    if (!strcasecmp (myargv[p + 1], "dos622"))
                        {
                            dos_mem_dump = mem_dump_dos622;
                        }
                    if (!strcasecmp (myargv[p + 1], "dos71"))
                        {
                            dos_mem_dump = mem_dump_win98;
                        }
                    else if (!strcasecmp (myargv[p + 1], "dosbox"))
                        {
                            dos_mem_dump = mem_dump_dosbox;
                        }
                    else
                        {
                            for (i = 0; i < DOS_MEM_DUMP_SIZE; ++i)
                                {
                                    ++p;

                                    if (p >= myargc || myargv[p][0] == '-')
                                        {
                                            break;
                                        }

                                    M_StrToInt (myargv[p], &val);
                                    mem_dump_custom[i++] = (unsigned char)val;
                                }

                            dos_mem_dump = mem_dump_custom;
                        }
                }
        }

    switch (size)
        {
        case 1:
            *((unsigned char *)value) = dos_mem_dump[offset];
            return true;
        case 2:
            *((unsigned short *)value)
                = dos_mem_dump[offset] | (dos_mem_dump[offset + 1] << 8);
            return true;
        case 4:
            *((unsigned int *)value) = dos_mem_dump[offset]
                                       | (dos_mem_dump[offset + 1] << 8)
                                       | (dos_mem_dump[offset + 2] << 16)
                                       | (dos_mem_dump[offset + 3] << 24);
            return true;
        }

    return false;
}

void
I_PrintStartupBanner (char *gamedescription)
{
    I_PrintDivider ();
    I_PrintBanner (gamedescription);
    I_PrintDivider ();

    printf (
        " " PACKAGE_NAME
        " is free software, covered by the GNU General Public\n"
        " License.  There is NO warranty; not even for MERCHANTABILITY or "
        "FITNESS\n"
        " FOR A PARTICULAR PURPOSE. You are welcome to change and distribute\n"
        " copies under certain conditions. See the source for more "
        "information.\n");

    I_PrintDivider ();
}

void
I_PrintBanner (char *msg)
{
    int i;
    int spaces = 35 - (strlen (msg) / 2);

    for (i = 0; i < spaces; ++i)
        putchar (' ');

    puts (msg);
}

void
I_PrintDivider (void)
{
    int i;

    for (i = 0; i < 75; ++i)
        {
            putchar ('=');
        }

    putchar ('\n');
}