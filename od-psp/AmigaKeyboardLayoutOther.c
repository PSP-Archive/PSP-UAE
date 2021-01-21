 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Amiga keycodes
  *
  * (c) 1995 Bernd Schmidt
  */

/* First, two dummies */
#define AK_mousestuff 0x100
#define AK_inhibit 0x101
/* This mutates into AK_CTRL in keybuf.c. */
#define AK_RCTRL 0x103


/* The following have different mappings on national keyboards */


#define AK_QUOTE 0x2A
#define AK_NUMBERSIGN 0x2B
#define AK_LTGT 0x30
#define AK_BACKQUOTE 0x00
