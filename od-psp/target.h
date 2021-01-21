 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Target specific stuff, Win32 version
  *
  * Copyright 1997 Mathias Ortmann
  */

#define TARGET_NAME "PSP"
#define TARGET_NO_AUTOCONF
#define TARGET_NO_ZFILE
#define DONT_PARSE_CMDLINE
#define TARGET_PROVIDES_DEFAULT_PREFS
#define TARGET_NO_DITHER

#define NO_MAIN_IN_MAIN_C

extern char *OPTIONSFILENAME; //SnaX:14/06/03

#define DEFPRTNAME "" // No default printer-name
#define DEFSERNAME "COM1"

//#define PICASSO96_SUPPORTED
//#define BSDSOCKET_SUPPORTED
