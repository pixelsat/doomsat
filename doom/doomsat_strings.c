#include "doomsat_strings.h"
#include "d_englsh.h"
#include <_stdio.h>

char
strcmp_ (char *str1, char *str2)
{
    while (*str1 && *str2)
        {
            if (*str1 != *str2)
                return 0;
            str1++;
            str2++;
        }
    return *str1 == *str2;
}

enum doomsat_string
doomsat_intern_string (char *str)
{
    if (str == NULL)
        return STRING_DOOMSAT_NULL;
    if (strcmp_ (str, D_DEVSTR))
        return STRING_D_DEVSTR;
    if (strcmp_ (str, D_CDROM))
        return STRING_D_CDROM;
    if (strcmp_ (str, PRESSKEY))
        return STRING_PRESSKEY;
    if (strcmp_ (str, PRESSYN))
        return STRING_PRESSYN;
    if (strcmp_ (str, QUITMSG))
        return STRING_QUITMSG;
    if (strcmp_ (str, LOADNET))
        return STRING_LOADNET;
    if (strcmp_ (str, QLOADNET))
        return STRING_QLOADNET;
    if (strcmp_ (str, QSAVESPOT))
        return STRING_QSAVESPOT;
    if (strcmp_ (str, SAVEDEAD))
        return STRING_SAVEDEAD;
    if (strcmp_ (str, QSPROMPT))
        return STRING_QSPROMPT;
    if (strcmp_ (str, QLPROMPT))
        return STRING_QLPROMPT;
    if (strcmp_ (str, NEWGAME))
        return STRING_NEWGAME;
    if (strcmp_ (str, NIGHTMARE))
        return STRING_NIGHTMARE;
    if (strcmp_ (str, SWSTRING))
        return STRING_SWSTRING;
    if (strcmp_ (str, MSGOFF))
        return STRING_MSGOFF;
    if (strcmp_ (str, MSGON))
        return STRING_MSGON;
    if (strcmp_ (str, NETEND))
        return STRING_NETEND;
    if (strcmp_ (str, ENDGAME))
        return STRING_ENDGAME;
    if (strcmp_ (str, DOSY))
        return STRING_DOSY;
    if (strcmp_ (str, DETAILHI))
        return STRING_DETAILHI;
    if (strcmp_ (str, DETAILLO))
        return STRING_DETAILLO;
    if (strcmp_ (str, GAMMALVL0))
        return STRING_GAMMALVL0;
    if (strcmp_ (str, GAMMALVL1))
        return STRING_GAMMALVL1;
    if (strcmp_ (str, GAMMALVL2))
        return STRING_GAMMALVL2;
    if (strcmp_ (str, GAMMALVL3))
        return STRING_GAMMALVL3;
    if (strcmp_ (str, GAMMALVL4))
        return STRING_GAMMALVL4;
    if (strcmp_ (str, EMPTYSTRING))
        return STRING_EMPTYSTRING;
    if (strcmp_ (str, GOTARMOR))
        return STRING_GOTARMOR;
    if (strcmp_ (str, GOTMEGA))
        return STRING_GOTMEGA;
    if (strcmp_ (str, GOTHTHBONUS))
        return STRING_GOTHTHBONUS;
    if (strcmp_ (str, GOTARMBONUS))
        return STRING_GOTARMBONUS;
    if (strcmp_ (str, GOTSTIM))
        return STRING_GOTSTIM;
    if (strcmp_ (str, GOTMEDINEED))
        return STRING_GOTMEDINEED;
    if (strcmp_ (str, GOTMEDIKIT))
        return STRING_GOTMEDIKIT;
    if (strcmp_ (str, GOTSUPER))
        return STRING_GOTSUPER;
    if (strcmp_ (str, GOTBLUECARD))
        return STRING_GOTBLUECARD;
    if (strcmp_ (str, GOTYELWCARD))
        return STRING_GOTYELWCARD;
    if (strcmp_ (str, GOTREDCARD))
        return STRING_GOTREDCARD;
    if (strcmp_ (str, GOTBLUESKUL))
        return STRING_GOTBLUESKUL;
    if (strcmp_ (str, GOTYELWSKUL))
        return STRING_GOTYELWSKUL;
    if (strcmp_ (str, GOTREDSKULL))
        return STRING_GOTREDSKULL;
    if (strcmp_ (str, GOTINVUL))
        return STRING_GOTINVUL;
    if (strcmp_ (str, GOTBERSERK))
        return STRING_GOTBERSERK;
    if (strcmp_ (str, GOTINVIS))
        return STRING_GOTINVIS;
    if (strcmp_ (str, GOTSUIT))
        return STRING_GOTSUIT;
    if (strcmp_ (str, GOTMAP))
        return STRING_GOTMAP;
    if (strcmp_ (str, GOTVISOR))
        return STRING_GOTVISOR;
    if (strcmp_ (str, GOTMSPHERE))
        return STRING_GOTMSPHERE;
    if (strcmp_ (str, GOTCLIP))
        return STRING_GOTCLIP;
    if (strcmp_ (str, GOTCLIPBOX))
        return STRING_GOTCLIPBOX;
    if (strcmp_ (str, GOTROCKET))
        return STRING_GOTROCKET;
    if (strcmp_ (str, GOTROCKBOX))
        return STRING_GOTROCKBOX;
    if (strcmp_ (str, GOTCELL))
        return STRING_GOTCELL;
    if (strcmp_ (str, GOTCELLBOX))
        return STRING_GOTCELLBOX;
    if (strcmp_ (str, GOTSHELLS))
        return STRING_GOTSHELLS;
    if (strcmp_ (str, GOTSHELLBOX))
        return STRING_GOTSHELLBOX;
    if (strcmp_ (str, GOTBACKPACK))
        return STRING_GOTBACKPACK;
    if (strcmp_ (str, GOTBFG9000))
        return STRING_GOTBFG9000;
    if (strcmp_ (str, GOTCHAINGUN))
        return STRING_GOTCHAINGUN;
    if (strcmp_ (str, GOTCHAINSAW))
        return STRING_GOTCHAINSAW;
    if (strcmp_ (str, GOTLAUNCHER))
        return STRING_GOTLAUNCHER;
    if (strcmp_ (str, GOTPLASMA))
        return STRING_GOTPLASMA;
    if (strcmp_ (str, GOTSHOTGUN))
        return STRING_GOTSHOTGUN;
    if (strcmp_ (str, GOTSHOTGUN2))
        return STRING_GOTSHOTGUN2;
    if (strcmp_ (str, PD_BLUEO))
        return STRING_PD_BLUEO;
    if (strcmp_ (str, PD_REDO))
        return STRING_PD_REDO;
    if (strcmp_ (str, PD_YELLOWO))
        return STRING_PD_YELLOWO;
    if (strcmp_ (str, PD_BLUEK))
        return STRING_PD_BLUEK;
    if (strcmp_ (str, PD_REDK))
        return STRING_PD_REDK;
    if (strcmp_ (str, PD_YELLOWK))
        return STRING_PD_YELLOWK;
    if (strcmp_ (str, GGSAVED))
        return STRING_GGSAVED;
    if (strcmp_ (str, HUSTR_MSGU))
        return STRING_HUSTR_MSGU;
    if (strcmp_ (str, HUSTR_E1M1))
        return STRING_HUSTR_E1M1;
    if (strcmp_ (str, HUSTR_E1M2))
        return STRING_HUSTR_E1M2;
    if (strcmp_ (str, HUSTR_E1M3))
        return STRING_HUSTR_E1M3;
    if (strcmp_ (str, HUSTR_E1M4))
        return STRING_HUSTR_E1M4;
    if (strcmp_ (str, HUSTR_E1M5))
        return STRING_HUSTR_E1M5;
    if (strcmp_ (str, HUSTR_E1M6))
        return STRING_HUSTR_E1M6;
    if (strcmp_ (str, HUSTR_E1M7))
        return STRING_HUSTR_E1M7;
    if (strcmp_ (str, HUSTR_E1M8))
        return STRING_HUSTR_E1M8;
    if (strcmp_ (str, HUSTR_E1M9))
        return STRING_HUSTR_E1M9;
    if (strcmp_ (str, HUSTR_E2M1))
        return STRING_HUSTR_E2M1;
    if (strcmp_ (str, HUSTR_E2M2))
        return STRING_HUSTR_E2M2;
    if (strcmp_ (str, HUSTR_E2M3))
        return STRING_HUSTR_E2M3;
    if (strcmp_ (str, HUSTR_E2M4))
        return STRING_HUSTR_E2M4;
    if (strcmp_ (str, HUSTR_E2M5))
        return STRING_HUSTR_E2M5;
    if (strcmp_ (str, HUSTR_E2M6))
        return STRING_HUSTR_E2M6;
    if (strcmp_ (str, HUSTR_E2M7))
        return STRING_HUSTR_E2M7;
    if (strcmp_ (str, HUSTR_E2M8))
        return STRING_HUSTR_E2M8;
    if (strcmp_ (str, HUSTR_E2M9))
        return STRING_HUSTR_E2M9;
    if (strcmp_ (str, HUSTR_E3M1))
        return STRING_HUSTR_E3M1;
    if (strcmp_ (str, HUSTR_E3M2))
        return STRING_HUSTR_E3M2;
    if (strcmp_ (str, HUSTR_E3M3))
        return STRING_HUSTR_E3M3;
    if (strcmp_ (str, HUSTR_E3M4))
        return STRING_HUSTR_E3M4;
    if (strcmp_ (str, HUSTR_E3M5))
        return STRING_HUSTR_E3M5;
    if (strcmp_ (str, HUSTR_E3M6))
        return STRING_HUSTR_E3M6;
    if (strcmp_ (str, HUSTR_E3M7))
        return STRING_HUSTR_E3M7;
    if (strcmp_ (str, HUSTR_E3M8))
        return STRING_HUSTR_E3M8;
    if (strcmp_ (str, HUSTR_E3M9))
        return STRING_HUSTR_E3M9;
    if (strcmp_ (str, HUSTR_E4M1))
        return STRING_HUSTR_E4M1;
    if (strcmp_ (str, HUSTR_E4M2))
        return STRING_HUSTR_E4M2;
    if (strcmp_ (str, HUSTR_E4M3))
        return STRING_HUSTR_E4M3;
    if (strcmp_ (str, HUSTR_E4M4))
        return STRING_HUSTR_E4M4;
    if (strcmp_ (str, HUSTR_E4M5))
        return STRING_HUSTR_E4M5;
    if (strcmp_ (str, HUSTR_E4M6))
        return STRING_HUSTR_E4M6;
    if (strcmp_ (str, HUSTR_E4M7))
        return STRING_HUSTR_E4M7;
    if (strcmp_ (str, HUSTR_E4M8))
        return STRING_HUSTR_E4M8;
    if (strcmp_ (str, HUSTR_E4M9))
        return STRING_HUSTR_E4M9;
    if (strcmp_ (str, HUSTR_1))
        return STRING_HUSTR_1;
    if (strcmp_ (str, HUSTR_2))
        return STRING_HUSTR_2;
    if (strcmp_ (str, HUSTR_3))
        return STRING_HUSTR_3;
    if (strcmp_ (str, HUSTR_4))
        return STRING_HUSTR_4;
    if (strcmp_ (str, HUSTR_5))
        return STRING_HUSTR_5;
    if (strcmp_ (str, HUSTR_6))
        return STRING_HUSTR_6;
    if (strcmp_ (str, HUSTR_7))
        return STRING_HUSTR_7;
    if (strcmp_ (str, HUSTR_8))
        return STRING_HUSTR_8;
    if (strcmp_ (str, HUSTR_9))
        return STRING_HUSTR_9;
    if (strcmp_ (str, HUSTR_10))
        return STRING_HUSTR_10;
    if (strcmp_ (str, HUSTR_11))
        return STRING_HUSTR_11;
    if (strcmp_ (str, HUSTR_12))
        return STRING_HUSTR_12;
    if (strcmp_ (str, HUSTR_13))
        return STRING_HUSTR_13;
    if (strcmp_ (str, HUSTR_14))
        return STRING_HUSTR_14;
    if (strcmp_ (str, HUSTR_15))
        return STRING_HUSTR_15;
    if (strcmp_ (str, HUSTR_16))
        return STRING_HUSTR_16;
    if (strcmp_ (str, HUSTR_17))
        return STRING_HUSTR_17;
    if (strcmp_ (str, HUSTR_18))
        return STRING_HUSTR_18;
    if (strcmp_ (str, HUSTR_19))
        return STRING_HUSTR_19;
    if (strcmp_ (str, HUSTR_20))
        return STRING_HUSTR_20;
    if (strcmp_ (str, HUSTR_21))
        return STRING_HUSTR_21;
    if (strcmp_ (str, HUSTR_22))
        return STRING_HUSTR_22;
    if (strcmp_ (str, HUSTR_23))
        return STRING_HUSTR_23;
    if (strcmp_ (str, HUSTR_24))
        return STRING_HUSTR_24;
    if (strcmp_ (str, HUSTR_25))
        return STRING_HUSTR_25;
    if (strcmp_ (str, HUSTR_26))
        return STRING_HUSTR_26;
    if (strcmp_ (str, HUSTR_27))
        return STRING_HUSTR_27;
    if (strcmp_ (str, HUSTR_28))
        return STRING_HUSTR_28;
    if (strcmp_ (str, HUSTR_29))
        return STRING_HUSTR_29;
    if (strcmp_ (str, HUSTR_30))
        return STRING_HUSTR_30;
    if (strcmp_ (str, HUSTR_31))
        return STRING_HUSTR_31;
    if (strcmp_ (str, HUSTR_32))
        return STRING_HUSTR_32;
    if (strcmp_ (str, PHUSTR_1))
        return STRING_PHUSTR_1;
    if (strcmp_ (str, PHUSTR_2))
        return STRING_PHUSTR_2;
    if (strcmp_ (str, PHUSTR_3))
        return STRING_PHUSTR_3;
    if (strcmp_ (str, PHUSTR_4))
        return STRING_PHUSTR_4;
    if (strcmp_ (str, PHUSTR_5))
        return STRING_PHUSTR_5;
    if (strcmp_ (str, PHUSTR_6))
        return STRING_PHUSTR_6;
    if (strcmp_ (str, PHUSTR_7))
        return STRING_PHUSTR_7;
    if (strcmp_ (str, PHUSTR_8))
        return STRING_PHUSTR_8;
    if (strcmp_ (str, PHUSTR_9))
        return STRING_PHUSTR_9;
    if (strcmp_ (str, PHUSTR_10))
        return STRING_PHUSTR_10;
    if (strcmp_ (str, PHUSTR_11))
        return STRING_PHUSTR_11;
    if (strcmp_ (str, PHUSTR_12))
        return STRING_PHUSTR_12;
    if (strcmp_ (str, PHUSTR_13))
        return STRING_PHUSTR_13;
    if (strcmp_ (str, PHUSTR_14))
        return STRING_PHUSTR_14;
    if (strcmp_ (str, PHUSTR_15))
        return STRING_PHUSTR_15;
    if (strcmp_ (str, PHUSTR_16))
        return STRING_PHUSTR_16;
    if (strcmp_ (str, PHUSTR_17))
        return STRING_PHUSTR_17;
    if (strcmp_ (str, PHUSTR_18))
        return STRING_PHUSTR_18;
    if (strcmp_ (str, PHUSTR_19))
        return STRING_PHUSTR_19;
    if (strcmp_ (str, PHUSTR_20))
        return STRING_PHUSTR_20;
    if (strcmp_ (str, PHUSTR_21))
        return STRING_PHUSTR_21;
    if (strcmp_ (str, PHUSTR_22))
        return STRING_PHUSTR_22;
    if (strcmp_ (str, PHUSTR_23))
        return STRING_PHUSTR_23;
    if (strcmp_ (str, PHUSTR_24))
        return STRING_PHUSTR_24;
    if (strcmp_ (str, PHUSTR_25))
        return STRING_PHUSTR_25;
    if (strcmp_ (str, PHUSTR_26))
        return STRING_PHUSTR_26;
    if (strcmp_ (str, PHUSTR_27))
        return STRING_PHUSTR_27;
    if (strcmp_ (str, PHUSTR_28))
        return STRING_PHUSTR_28;
    if (strcmp_ (str, PHUSTR_29))
        return STRING_PHUSTR_29;
    if (strcmp_ (str, PHUSTR_30))
        return STRING_PHUSTR_30;
    if (strcmp_ (str, PHUSTR_31))
        return STRING_PHUSTR_31;
    if (strcmp_ (str, PHUSTR_32))
        return STRING_PHUSTR_32;
    if (strcmp_ (str, THUSTR_1))
        return STRING_THUSTR_1;
    if (strcmp_ (str, THUSTR_2))
        return STRING_THUSTR_2;
    if (strcmp_ (str, THUSTR_3))
        return STRING_THUSTR_3;
    if (strcmp_ (str, THUSTR_4))
        return STRING_THUSTR_4;
    if (strcmp_ (str, THUSTR_5))
        return STRING_THUSTR_5;
    if (strcmp_ (str, THUSTR_6))
        return STRING_THUSTR_6;
    if (strcmp_ (str, THUSTR_7))
        return STRING_THUSTR_7;
    if (strcmp_ (str, THUSTR_8))
        return STRING_THUSTR_8;
    if (strcmp_ (str, THUSTR_9))
        return STRING_THUSTR_9;
    if (strcmp_ (str, THUSTR_10))
        return STRING_THUSTR_10;
    if (strcmp_ (str, THUSTR_11))
        return STRING_THUSTR_11;
    if (strcmp_ (str, THUSTR_12))
        return STRING_THUSTR_12;
    if (strcmp_ (str, THUSTR_13))
        return STRING_THUSTR_13;
    if (strcmp_ (str, THUSTR_14))
        return STRING_THUSTR_14;
    if (strcmp_ (str, THUSTR_15))
        return STRING_THUSTR_15;
    if (strcmp_ (str, THUSTR_16))
        return STRING_THUSTR_16;
    if (strcmp_ (str, THUSTR_17))
        return STRING_THUSTR_17;
    if (strcmp_ (str, THUSTR_18))
        return STRING_THUSTR_18;
    if (strcmp_ (str, THUSTR_19))
        return STRING_THUSTR_19;
    if (strcmp_ (str, THUSTR_20))
        return STRING_THUSTR_20;
    if (strcmp_ (str, THUSTR_21))
        return STRING_THUSTR_21;
    if (strcmp_ (str, THUSTR_22))
        return STRING_THUSTR_22;
    if (strcmp_ (str, THUSTR_23))
        return STRING_THUSTR_23;
    if (strcmp_ (str, THUSTR_24))
        return STRING_THUSTR_24;
    if (strcmp_ (str, THUSTR_25))
        return STRING_THUSTR_25;
    if (strcmp_ (str, THUSTR_26))
        return STRING_THUSTR_26;
    if (strcmp_ (str, THUSTR_27))
        return STRING_THUSTR_27;
    if (strcmp_ (str, THUSTR_28))
        return STRING_THUSTR_28;
    if (strcmp_ (str, THUSTR_29))
        return STRING_THUSTR_29;
    if (strcmp_ (str, THUSTR_30))
        return STRING_THUSTR_30;
    if (strcmp_ (str, THUSTR_31))
        return STRING_THUSTR_31;
    if (strcmp_ (str, THUSTR_32))
        return STRING_THUSTR_32;
    if (strcmp_ (str, HUSTR_CHATMACRO1))
        return STRING_HUSTR_CHATMACRO1;
    if (strcmp_ (str, HUSTR_CHATMACRO2))
        return STRING_HUSTR_CHATMACRO2;
    if (strcmp_ (str, HUSTR_CHATMACRO3))
        return STRING_HUSTR_CHATMACRO3;
    if (strcmp_ (str, HUSTR_CHATMACRO4))
        return STRING_HUSTR_CHATMACRO4;
    if (strcmp_ (str, HUSTR_CHATMACRO5))
        return STRING_HUSTR_CHATMACRO5;
    if (strcmp_ (str, HUSTR_CHATMACRO6))
        return STRING_HUSTR_CHATMACRO6;
    if (strcmp_ (str, HUSTR_CHATMACRO7))
        return STRING_HUSTR_CHATMACRO7;
    if (strcmp_ (str, HUSTR_CHATMACRO8))
        return STRING_HUSTR_CHATMACRO8;
    if (strcmp_ (str, HUSTR_CHATMACRO9))
        return STRING_HUSTR_CHATMACRO9;
    if (strcmp_ (str, HUSTR_CHATMACRO0))
        return STRING_HUSTR_CHATMACRO0;
    if (strcmp_ (str, HUSTR_TALKTOSELF1))
        return STRING_HUSTR_TALKTOSELF1;
    if (strcmp_ (str, HUSTR_TALKTOSELF2))
        return STRING_HUSTR_TALKTOSELF2;
    if (strcmp_ (str, HUSTR_TALKTOSELF3))
        return STRING_HUSTR_TALKTOSELF3;
    if (strcmp_ (str, HUSTR_TALKTOSELF4))
        return STRING_HUSTR_TALKTOSELF4;
    if (strcmp_ (str, HUSTR_TALKTOSELF5))
        return STRING_HUSTR_TALKTOSELF5;
    if (strcmp_ (str, HUSTR_MESSAGESENT))
        return STRING_HUSTR_MESSAGESENT;
    if (strcmp_ (str, HUSTR_PLRGREEN))
        return STRING_HUSTR_PLRGREEN;
    if (strcmp_ (str, HUSTR_PLRINDIGO))
        return STRING_HUSTR_PLRINDIGO;
    if (strcmp_ (str, HUSTR_PLRBROWN))
        return STRING_HUSTR_PLRBROWN;
    if (strcmp_ (str, HUSTR_PLRRED))
        return STRING_HUSTR_PLRRED;
    if (strcmp_ (str, AMSTR_FOLLOWON))
        return STRING_AMSTR_FOLLOWON;
    if (strcmp_ (str, AMSTR_FOLLOWOFF))
        return STRING_AMSTR_FOLLOWOFF;
    if (strcmp_ (str, AMSTR_GRIDON))
        return STRING_AMSTR_GRIDON;
    if (strcmp_ (str, AMSTR_GRIDOFF))
        return STRING_AMSTR_GRIDOFF;
    if (strcmp_ (str, AMSTR_MARKEDSPOT))
        return STRING_AMSTR_MARKEDSPOT;
    if (strcmp_ (str, AMSTR_MARKSCLEARED))
        return STRING_AMSTR_MARKSCLEARED;
    if (strcmp_ (str, STSTR_MUS))
        return STRING_STSTR_MUS;
    if (strcmp_ (str, STSTR_NOMUS))
        return STRING_STSTR_NOMUS;
    if (strcmp_ (str, STSTR_DQDON))
        return STRING_STSTR_DQDON;
    if (strcmp_ (str, STSTR_DQDOFF))
        return STRING_STSTR_DQDOFF;
    if (strcmp_ (str, STSTR_KFAADDED))
        return STRING_STSTR_KFAADDED;
    if (strcmp_ (str, STSTR_FAADDED))
        return STRING_STSTR_FAADDED;
    if (strcmp_ (str, STSTR_NCON))
        return STRING_STSTR_NCON;
    if (strcmp_ (str, STSTR_NCOFF))
        return STRING_STSTR_NCOFF;
    if (strcmp_ (str, STSTR_BEHOLD))
        return STRING_STSTR_BEHOLD;
    if (strcmp_ (str, STSTR_BEHOLDX))
        return STRING_STSTR_BEHOLDX;
    if (strcmp_ (str, STSTR_CHOPPERS))
        return STRING_STSTR_CHOPPERS;
    if (strcmp_ (str, STSTR_CLEV))
        return STRING_STSTR_CLEV;
    if (strcmp_ (str, E1TEXT))
        return STRING_E1TEXT;
    if (strcmp_ (str, E2TEXT))
        return STRING_E2TEXT;
    if (strcmp_ (str, E3TEXT))
        return STRING_E3TEXT;
    if (strcmp_ (str, E4TEXT))
        return STRING_E4TEXT;
    if (strcmp_ (str, C1TEXT))
        return STRING_C1TEXT;
    if (strcmp_ (str, C2TEXT))
        return STRING_C2TEXT;
    if (strcmp_ (str, C3TEXT))
        return STRING_C3TEXT;
    if (strcmp_ (str, C4TEXT))
        return STRING_C4TEXT;
    if (strcmp_ (str, C5TEXT))
        return STRING_C5TEXT;
    if (strcmp_ (str, C6TEXT))
        return STRING_C6TEXT;
    if (strcmp_ (str, P1TEXT))
        return STRING_P1TEXT;
    if (strcmp_ (str, P2TEXT))
        return STRING_P2TEXT;
    if (strcmp_ (str, P3TEXT))
        return STRING_P3TEXT;
    if (strcmp_ (str, P4TEXT))
        return STRING_P4TEXT;
    if (strcmp_ (str, P5TEXT))
        return STRING_P5TEXT;
    if (strcmp_ (str, P6TEXT))
        return STRING_P6TEXT;
    if (strcmp_ (str, T1TEXT))
        return STRING_T1TEXT;
    if (strcmp_ (str, T2TEXT))
        return STRING_T2TEXT;
    if (strcmp_ (str, T3TEXT))
        return STRING_T3TEXT;
    if (strcmp_ (str, T4TEXT))
        return STRING_T4TEXT;
    if (strcmp_ (str, T5TEXT))
        return STRING_T5TEXT;
    if (strcmp_ (str, T6TEXT))
        return STRING_T6TEXT;
    if (strcmp_ (str, CC_ZOMBIE))
        return STRING_CC_ZOMBIE;
    if (strcmp_ (str, CC_SHOTGUN))
        return STRING_CC_SHOTGUN;
    if (strcmp_ (str, CC_HEAVY))
        return STRING_CC_HEAVY;
    if (strcmp_ (str, CC_IMP))
        return STRING_CC_IMP;
    if (strcmp_ (str, CC_DEMON))
        return STRING_CC_DEMON;
    if (strcmp_ (str, CC_LOST))
        return STRING_CC_LOST;
    if (strcmp_ (str, CC_CACO))
        return STRING_CC_CACO;
    if (strcmp_ (str, CC_HELL))
        return STRING_CC_HELL;
    if (strcmp_ (str, CC_BARON))
        return STRING_CC_BARON;
    if (strcmp_ (str, CC_ARACH))
        return STRING_CC_ARACH;
    if (strcmp_ (str, CC_PAIN))
        return STRING_CC_PAIN;
    if (strcmp_ (str, CC_REVEN))
        return STRING_CC_REVEN;
    if (strcmp_ (str, CC_MANCU))
        return STRING_CC_MANCU;
    if (strcmp_ (str, CC_ARCH))
        return STRING_CC_ARCH;
    if (strcmp_ (str, CC_SPIDER))
        return STRING_CC_SPIDER;
    if (strcmp_ (str, CC_CYBER))
        return STRING_CC_CYBER;
    if (strcmp_ (str, CC_HERO))
        return STRING_CC_HERO;
    return STRING_DOOMSAT_UNKNOWN;
}

char *
doomsat_unintern_string (enum doomsat_string interned)
{
    if (interned == STRING_D_DEVSTR)
        return D_DEVSTR;
    if (interned == STRING_D_CDROM)
        return D_CDROM;
    if (interned == STRING_PRESSKEY)
        return PRESSKEY;
    if (interned == STRING_PRESSYN)
        return PRESSYN;
    if (interned == STRING_QUITMSG)
        return QUITMSG;
    if (interned == STRING_LOADNET)
        return LOADNET;
    if (interned == STRING_QLOADNET)
        return QLOADNET;
    if (interned == STRING_QSAVESPOT)
        return QSAVESPOT;
    if (interned == STRING_SAVEDEAD)
        return SAVEDEAD;
    if (interned == STRING_QSPROMPT)
        return QSPROMPT;
    if (interned == STRING_QLPROMPT)
        return QLPROMPT;
    if (interned == STRING_NEWGAME)
        return NEWGAME;
    if (interned == STRING_NIGHTMARE)
        return NIGHTMARE;
    if (interned == STRING_SWSTRING)
        return SWSTRING;
    if (interned == STRING_MSGOFF)
        return MSGOFF;
    if (interned == STRING_MSGON)
        return MSGON;
    if (interned == STRING_NETEND)
        return NETEND;
    if (interned == STRING_ENDGAME)
        return ENDGAME;
    if (interned == STRING_DOSY)
        return DOSY;
    if (interned == STRING_DETAILHI)
        return DETAILHI;
    if (interned == STRING_DETAILLO)
        return DETAILLO;
    if (interned == STRING_GAMMALVL0)
        return GAMMALVL0;
    if (interned == STRING_GAMMALVL1)
        return GAMMALVL1;
    if (interned == STRING_GAMMALVL2)
        return GAMMALVL2;
    if (interned == STRING_GAMMALVL3)
        return GAMMALVL3;
    if (interned == STRING_GAMMALVL4)
        return GAMMALVL4;
    if (interned == STRING_EMPTYSTRING)
        return EMPTYSTRING;
    if (interned == STRING_GOTARMOR)
        return GOTARMOR;
    if (interned == STRING_GOTMEGA)
        return GOTMEGA;
    if (interned == STRING_GOTHTHBONUS)
        return GOTHTHBONUS;
    if (interned == STRING_GOTARMBONUS)
        return GOTARMBONUS;
    if (interned == STRING_GOTSTIM)
        return GOTSTIM;
    if (interned == STRING_GOTMEDINEED)
        return GOTMEDINEED;
    if (interned == STRING_GOTMEDIKIT)
        return GOTMEDIKIT;
    if (interned == STRING_GOTSUPER)
        return GOTSUPER;
    if (interned == STRING_GOTBLUECARD)
        return GOTBLUECARD;
    if (interned == STRING_GOTYELWCARD)
        return GOTYELWCARD;
    if (interned == STRING_GOTREDCARD)
        return GOTREDCARD;
    if (interned == STRING_GOTBLUESKUL)
        return GOTBLUESKUL;
    if (interned == STRING_GOTYELWSKUL)
        return GOTYELWSKUL;
    if (interned == STRING_GOTREDSKULL)
        return GOTREDSKULL;
    if (interned == STRING_GOTINVUL)
        return GOTINVUL;
    if (interned == STRING_GOTBERSERK)
        return GOTBERSERK;
    if (interned == STRING_GOTINVIS)
        return GOTINVIS;
    if (interned == STRING_GOTSUIT)
        return GOTSUIT;
    if (interned == STRING_GOTMAP)
        return GOTMAP;
    if (interned == STRING_GOTVISOR)
        return GOTVISOR;
    if (interned == STRING_GOTMSPHERE)
        return GOTMSPHERE;
    if (interned == STRING_GOTCLIP)
        return GOTCLIP;
    if (interned == STRING_GOTCLIPBOX)
        return GOTCLIPBOX;
    if (interned == STRING_GOTROCKET)
        return GOTROCKET;
    if (interned == STRING_GOTROCKBOX)
        return GOTROCKBOX;
    if (interned == STRING_GOTCELL)
        return GOTCELL;
    if (interned == STRING_GOTCELLBOX)
        return GOTCELLBOX;
    if (interned == STRING_GOTSHELLS)
        return GOTSHELLS;
    if (interned == STRING_GOTSHELLBOX)
        return GOTSHELLBOX;
    if (interned == STRING_GOTBACKPACK)
        return GOTBACKPACK;
    if (interned == STRING_GOTBFG9000)
        return GOTBFG9000;
    if (interned == STRING_GOTCHAINGUN)
        return GOTCHAINGUN;
    if (interned == STRING_GOTCHAINSAW)
        return GOTCHAINSAW;
    if (interned == STRING_GOTLAUNCHER)
        return GOTLAUNCHER;
    if (interned == STRING_GOTPLASMA)
        return GOTPLASMA;
    if (interned == STRING_GOTSHOTGUN)
        return GOTSHOTGUN;
    if (interned == STRING_GOTSHOTGUN2)
        return GOTSHOTGUN2;
    if (interned == STRING_PD_BLUEO)
        return PD_BLUEO;
    if (interned == STRING_PD_REDO)
        return PD_REDO;
    if (interned == STRING_PD_YELLOWO)
        return PD_YELLOWO;
    if (interned == STRING_PD_BLUEK)
        return PD_BLUEK;
    if (interned == STRING_PD_REDK)
        return PD_REDK;
    if (interned == STRING_PD_YELLOWK)
        return PD_YELLOWK;
    if (interned == STRING_GGSAVED)
        return GGSAVED;
    if (interned == STRING_HUSTR_MSGU)
        return HUSTR_MSGU;
    if (interned == STRING_HUSTR_E1M1)
        return HUSTR_E1M1;
    if (interned == STRING_HUSTR_E1M2)
        return HUSTR_E1M2;
    if (interned == STRING_HUSTR_E1M3)
        return HUSTR_E1M3;
    if (interned == STRING_HUSTR_E1M4)
        return HUSTR_E1M4;
    if (interned == STRING_HUSTR_E1M5)
        return HUSTR_E1M5;
    if (interned == STRING_HUSTR_E1M6)
        return HUSTR_E1M6;
    if (interned == STRING_HUSTR_E1M7)
        return HUSTR_E1M7;
    if (interned == STRING_HUSTR_E1M8)
        return HUSTR_E1M8;
    if (interned == STRING_HUSTR_E1M9)
        return HUSTR_E1M9;
    if (interned == STRING_HUSTR_E2M1)
        return HUSTR_E2M1;
    if (interned == STRING_HUSTR_E2M2)
        return HUSTR_E2M2;
    if (interned == STRING_HUSTR_E2M3)
        return HUSTR_E2M3;
    if (interned == STRING_HUSTR_E2M4)
        return HUSTR_E2M4;
    if (interned == STRING_HUSTR_E2M5)
        return HUSTR_E2M5;
    if (interned == STRING_HUSTR_E2M6)
        return HUSTR_E2M6;
    if (interned == STRING_HUSTR_E2M7)
        return HUSTR_E2M7;
    if (interned == STRING_HUSTR_E2M8)
        return HUSTR_E2M8;
    if (interned == STRING_HUSTR_E2M9)
        return HUSTR_E2M9;
    if (interned == STRING_HUSTR_E3M1)
        return HUSTR_E3M1;
    if (interned == STRING_HUSTR_E3M2)
        return HUSTR_E3M2;
    if (interned == STRING_HUSTR_E3M3)
        return HUSTR_E3M3;
    if (interned == STRING_HUSTR_E3M4)
        return HUSTR_E3M4;
    if (interned == STRING_HUSTR_E3M5)
        return HUSTR_E3M5;
    if (interned == STRING_HUSTR_E3M6)
        return HUSTR_E3M6;
    if (interned == STRING_HUSTR_E3M7)
        return HUSTR_E3M7;
    if (interned == STRING_HUSTR_E3M8)
        return HUSTR_E3M8;
    if (interned == STRING_HUSTR_E3M9)
        return HUSTR_E3M9;
    if (interned == STRING_HUSTR_E4M1)
        return HUSTR_E4M1;
    if (interned == STRING_HUSTR_E4M2)
        return HUSTR_E4M2;
    if (interned == STRING_HUSTR_E4M3)
        return HUSTR_E4M3;
    if (interned == STRING_HUSTR_E4M4)
        return HUSTR_E4M4;
    if (interned == STRING_HUSTR_E4M5)
        return HUSTR_E4M5;
    if (interned == STRING_HUSTR_E4M6)
        return HUSTR_E4M6;
    if (interned == STRING_HUSTR_E4M7)
        return HUSTR_E4M7;
    if (interned == STRING_HUSTR_E4M8)
        return HUSTR_E4M8;
    if (interned == STRING_HUSTR_E4M9)
        return HUSTR_E4M9;
    if (interned == STRING_HUSTR_1)
        return HUSTR_1;
    if (interned == STRING_HUSTR_2)
        return HUSTR_2;
    if (interned == STRING_HUSTR_3)
        return HUSTR_3;
    if (interned == STRING_HUSTR_4)
        return HUSTR_4;
    if (interned == STRING_HUSTR_5)
        return HUSTR_5;
    if (interned == STRING_HUSTR_6)
        return HUSTR_6;
    if (interned == STRING_HUSTR_7)
        return HUSTR_7;
    if (interned == STRING_HUSTR_8)
        return HUSTR_8;
    if (interned == STRING_HUSTR_9)
        return HUSTR_9;
    if (interned == STRING_HUSTR_10)
        return HUSTR_10;
    if (interned == STRING_HUSTR_11)
        return HUSTR_11;
    if (interned == STRING_HUSTR_12)
        return HUSTR_12;
    if (interned == STRING_HUSTR_13)
        return HUSTR_13;
    if (interned == STRING_HUSTR_14)
        return HUSTR_14;
    if (interned == STRING_HUSTR_15)
        return HUSTR_15;
    if (interned == STRING_HUSTR_16)
        return HUSTR_16;
    if (interned == STRING_HUSTR_17)
        return HUSTR_17;
    if (interned == STRING_HUSTR_18)
        return HUSTR_18;
    if (interned == STRING_HUSTR_19)
        return HUSTR_19;
    if (interned == STRING_HUSTR_20)
        return HUSTR_20;
    if (interned == STRING_HUSTR_21)
        return HUSTR_21;
    if (interned == STRING_HUSTR_22)
        return HUSTR_22;
    if (interned == STRING_HUSTR_23)
        return HUSTR_23;
    if (interned == STRING_HUSTR_24)
        return HUSTR_24;
    if (interned == STRING_HUSTR_25)
        return HUSTR_25;
    if (interned == STRING_HUSTR_26)
        return HUSTR_26;
    if (interned == STRING_HUSTR_27)
        return HUSTR_27;
    if (interned == STRING_HUSTR_28)
        return HUSTR_28;
    if (interned == STRING_HUSTR_29)
        return HUSTR_29;
    if (interned == STRING_HUSTR_30)
        return HUSTR_30;
    if (interned == STRING_HUSTR_31)
        return HUSTR_31;
    if (interned == STRING_HUSTR_32)
        return HUSTR_32;
    if (interned == STRING_PHUSTR_1)
        return PHUSTR_1;
    if (interned == STRING_PHUSTR_2)
        return PHUSTR_2;
    if (interned == STRING_PHUSTR_3)
        return PHUSTR_3;
    if (interned == STRING_PHUSTR_4)
        return PHUSTR_4;
    if (interned == STRING_PHUSTR_5)
        return PHUSTR_5;
    if (interned == STRING_PHUSTR_6)
        return PHUSTR_6;
    if (interned == STRING_PHUSTR_7)
        return PHUSTR_7;
    if (interned == STRING_PHUSTR_8)
        return PHUSTR_8;
    if (interned == STRING_PHUSTR_9)
        return PHUSTR_9;
    if (interned == STRING_PHUSTR_10)
        return PHUSTR_10;
    if (interned == STRING_PHUSTR_11)
        return PHUSTR_11;
    if (interned == STRING_PHUSTR_12)
        return PHUSTR_12;
    if (interned == STRING_PHUSTR_13)
        return PHUSTR_13;
    if (interned == STRING_PHUSTR_14)
        return PHUSTR_14;
    if (interned == STRING_PHUSTR_15)
        return PHUSTR_15;
    if (interned == STRING_PHUSTR_16)
        return PHUSTR_16;
    if (interned == STRING_PHUSTR_17)
        return PHUSTR_17;
    if (interned == STRING_PHUSTR_18)
        return PHUSTR_18;
    if (interned == STRING_PHUSTR_19)
        return PHUSTR_19;
    if (interned == STRING_PHUSTR_20)
        return PHUSTR_20;
    if (interned == STRING_PHUSTR_21)
        return PHUSTR_21;
    if (interned == STRING_PHUSTR_22)
        return PHUSTR_22;
    if (interned == STRING_PHUSTR_23)
        return PHUSTR_23;
    if (interned == STRING_PHUSTR_24)
        return PHUSTR_24;
    if (interned == STRING_PHUSTR_25)
        return PHUSTR_25;
    if (interned == STRING_PHUSTR_26)
        return PHUSTR_26;
    if (interned == STRING_PHUSTR_27)
        return PHUSTR_27;
    if (interned == STRING_PHUSTR_28)
        return PHUSTR_28;
    if (interned == STRING_PHUSTR_29)
        return PHUSTR_29;
    if (interned == STRING_PHUSTR_30)
        return PHUSTR_30;
    if (interned == STRING_PHUSTR_31)
        return PHUSTR_31;
    if (interned == STRING_PHUSTR_32)
        return PHUSTR_32;
    if (interned == STRING_THUSTR_1)
        return THUSTR_1;
    if (interned == STRING_THUSTR_2)
        return THUSTR_2;
    if (interned == STRING_THUSTR_3)
        return THUSTR_3;
    if (interned == STRING_THUSTR_4)
        return THUSTR_4;
    if (interned == STRING_THUSTR_5)
        return THUSTR_5;
    if (interned == STRING_THUSTR_6)
        return THUSTR_6;
    if (interned == STRING_THUSTR_7)
        return THUSTR_7;
    if (interned == STRING_THUSTR_8)
        return THUSTR_8;
    if (interned == STRING_THUSTR_9)
        return THUSTR_9;
    if (interned == STRING_THUSTR_10)
        return THUSTR_10;
    if (interned == STRING_THUSTR_11)
        return THUSTR_11;
    if (interned == STRING_THUSTR_12)
        return THUSTR_12;
    if (interned == STRING_THUSTR_13)
        return THUSTR_13;
    if (interned == STRING_THUSTR_14)
        return THUSTR_14;
    if (interned == STRING_THUSTR_15)
        return THUSTR_15;
    if (interned == STRING_THUSTR_16)
        return THUSTR_16;
    if (interned == STRING_THUSTR_17)
        return THUSTR_17;
    if (interned == STRING_THUSTR_18)
        return THUSTR_18;
    if (interned == STRING_THUSTR_19)
        return THUSTR_19;
    if (interned == STRING_THUSTR_20)
        return THUSTR_20;
    if (interned == STRING_THUSTR_21)
        return THUSTR_21;
    if (interned == STRING_THUSTR_22)
        return THUSTR_22;
    if (interned == STRING_THUSTR_23)
        return THUSTR_23;
    if (interned == STRING_THUSTR_24)
        return THUSTR_24;
    if (interned == STRING_THUSTR_25)
        return THUSTR_25;
    if (interned == STRING_THUSTR_26)
        return THUSTR_26;
    if (interned == STRING_THUSTR_27)
        return THUSTR_27;
    if (interned == STRING_THUSTR_28)
        return THUSTR_28;
    if (interned == STRING_THUSTR_29)
        return THUSTR_29;
    if (interned == STRING_THUSTR_30)
        return THUSTR_30;
    if (interned == STRING_THUSTR_31)
        return THUSTR_31;
    if (interned == STRING_THUSTR_32)
        return THUSTR_32;
    if (interned == STRING_HUSTR_CHATMACRO1)
        return HUSTR_CHATMACRO1;
    if (interned == STRING_HUSTR_CHATMACRO2)
        return HUSTR_CHATMACRO2;
    if (interned == STRING_HUSTR_CHATMACRO3)
        return HUSTR_CHATMACRO3;
    if (interned == STRING_HUSTR_CHATMACRO4)
        return HUSTR_CHATMACRO4;
    if (interned == STRING_HUSTR_CHATMACRO5)
        return HUSTR_CHATMACRO5;
    if (interned == STRING_HUSTR_CHATMACRO6)
        return HUSTR_CHATMACRO6;
    if (interned == STRING_HUSTR_CHATMACRO7)
        return HUSTR_CHATMACRO7;
    if (interned == STRING_HUSTR_CHATMACRO8)
        return HUSTR_CHATMACRO8;
    if (interned == STRING_HUSTR_CHATMACRO9)
        return HUSTR_CHATMACRO9;
    if (interned == STRING_HUSTR_CHATMACRO0)
        return HUSTR_CHATMACRO0;
    if (interned == STRING_HUSTR_TALKTOSELF1)
        return HUSTR_TALKTOSELF1;
    if (interned == STRING_HUSTR_TALKTOSELF2)
        return HUSTR_TALKTOSELF2;
    if (interned == STRING_HUSTR_TALKTOSELF3)
        return HUSTR_TALKTOSELF3;
    if (interned == STRING_HUSTR_TALKTOSELF4)
        return HUSTR_TALKTOSELF4;
    if (interned == STRING_HUSTR_TALKTOSELF5)
        return HUSTR_TALKTOSELF5;
    if (interned == STRING_HUSTR_MESSAGESENT)
        return HUSTR_MESSAGESENT;
    if (interned == STRING_HUSTR_PLRGREEN)
        return HUSTR_PLRGREEN;
    if (interned == STRING_HUSTR_PLRINDIGO)
        return HUSTR_PLRINDIGO;
    if (interned == STRING_HUSTR_PLRBROWN)
        return HUSTR_PLRBROWN;
    if (interned == STRING_HUSTR_PLRRED)
        return HUSTR_PLRRED;
    if (interned == STRING_AMSTR_FOLLOWON)
        return AMSTR_FOLLOWON;
    if (interned == STRING_AMSTR_FOLLOWOFF)
        return AMSTR_FOLLOWOFF;
    if (interned == STRING_AMSTR_GRIDON)
        return AMSTR_GRIDON;
    if (interned == STRING_AMSTR_GRIDOFF)
        return AMSTR_GRIDOFF;
    if (interned == STRING_AMSTR_MARKEDSPOT)
        return AMSTR_MARKEDSPOT;
    if (interned == STRING_AMSTR_MARKSCLEARED)
        return AMSTR_MARKSCLEARED;
    if (interned == STRING_STSTR_MUS)
        return STSTR_MUS;
    if (interned == STRING_STSTR_NOMUS)
        return STSTR_NOMUS;
    if (interned == STRING_STSTR_DQDON)
        return STSTR_DQDON;
    if (interned == STRING_STSTR_DQDOFF)
        return STSTR_DQDOFF;
    if (interned == STRING_STSTR_KFAADDED)
        return STSTR_KFAADDED;
    if (interned == STRING_STSTR_FAADDED)
        return STSTR_FAADDED;
    if (interned == STRING_STSTR_NCON)
        return STSTR_NCON;
    if (interned == STRING_STSTR_NCOFF)
        return STSTR_NCOFF;
    if (interned == STRING_STSTR_BEHOLD)
        return STSTR_BEHOLD;
    if (interned == STRING_STSTR_BEHOLDX)
        return STSTR_BEHOLDX;
    if (interned == STRING_STSTR_CHOPPERS)
        return STSTR_CHOPPERS;
    if (interned == STRING_STSTR_CLEV)
        return STSTR_CLEV;
    if (interned == STRING_E1TEXT)
        return E1TEXT;
    if (interned == STRING_E2TEXT)
        return E2TEXT;
    if (interned == STRING_E3TEXT)
        return E3TEXT;
    if (interned == STRING_E4TEXT)
        return E4TEXT;
    if (interned == STRING_C1TEXT)
        return C1TEXT;
    if (interned == STRING_C2TEXT)
        return C2TEXT;
    if (interned == STRING_C3TEXT)
        return C3TEXT;
    if (interned == STRING_C4TEXT)
        return C4TEXT;
    if (interned == STRING_C5TEXT)
        return C5TEXT;
    if (interned == STRING_C6TEXT)
        return C6TEXT;
    if (interned == STRING_P1TEXT)
        return P1TEXT;
    if (interned == STRING_P2TEXT)
        return P2TEXT;
    if (interned == STRING_P3TEXT)
        return P3TEXT;
    if (interned == STRING_P4TEXT)
        return P4TEXT;
    if (interned == STRING_P5TEXT)
        return P5TEXT;
    if (interned == STRING_P6TEXT)
        return P6TEXT;
    if (interned == STRING_T1TEXT)
        return T1TEXT;
    if (interned == STRING_T2TEXT)
        return T2TEXT;
    if (interned == STRING_T3TEXT)
        return T3TEXT;
    if (interned == STRING_T4TEXT)
        return T4TEXT;
    if (interned == STRING_T5TEXT)
        return T5TEXT;
    if (interned == STRING_T6TEXT)
        return T6TEXT;
    if (interned == STRING_CC_ZOMBIE)
        return CC_ZOMBIE;
    if (interned == STRING_CC_SHOTGUN)
        return CC_SHOTGUN;
    if (interned == STRING_CC_HEAVY)
        return CC_HEAVY;
    if (interned == STRING_CC_IMP)
        return CC_IMP;
    if (interned == STRING_CC_DEMON)
        return CC_DEMON;
    if (interned == STRING_CC_LOST)
        return CC_LOST;
    if (interned == STRING_CC_CACO)
        return CC_CACO;
    if (interned == STRING_CC_HELL)
        return CC_HELL;
    if (interned == STRING_CC_BARON)
        return CC_BARON;
    if (interned == STRING_CC_ARACH)
        return CC_ARACH;
    if (interned == STRING_CC_PAIN)
        return CC_PAIN;
    if (interned == STRING_CC_REVEN)
        return CC_REVEN;
    if (interned == STRING_CC_MANCU)
        return CC_MANCU;
    if (interned == STRING_CC_ARCH)
        return CC_ARCH;
    if (interned == STRING_CC_SPIDER)
        return CC_SPIDER;
    if (interned == STRING_CC_CYBER)
        return CC_CYBER;
    if (interned == STRING_CC_HERO)
        return CC_HERO;
    if (interned == STRING_DOOMSAT_NULL)
        return NULL;
    return "[Doomsat] Unknown message";
}