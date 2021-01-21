#
# Makefile.in for UAE
#

INCDIR = include od-psp
CFLAGS = -O3 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) --no-trap -mno-shared -O --no-break -non_shared
USE_NEWLIB_LIBC = 1

LIBDIR =
LIBS = -lpspaudiolib -lpspaudio -lpsppower

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSPUAE v0.41
PSP_EBOOT_ICON = ICON0.PNG
PSP_EBOOT_PIC1 = PIC1.PNG
 
TARGET    = uae
LIBRARIES = 
MATHLIB   = 
RESOBJS   = @RESOBJS@

NO_SCHED_CFLAGS = @NO_SCHED_CFLAGS@

INSTALL         = @INSTALL@
INSTALL_PROGRAM = @INSTALL_PROGRAM@
INSTALL_DATA    = @INSTALL_DATA@
prefix          = @prefix@
exec_prefix     = @exec_prefix@
bindir          = @bindir@
sysconfdir      = @sysconfdir@

VPATH = @top_srcdir@/src

.SUFFIXES: .o .c .h .m .i .S .rc .res

INCLUDES=-I. -I./include/

CPUOBJS	  = newcpu.o cpuemu.o cpustbl.o cpudefs.o readcpu.o 
#CPUOBJS = c68k\c68k.o c68k\c68kexec.o c68kemu.o

OBJS = main.o memory.o $(CPUOBJS) custom.o cia.o serial.o blitter.o \
       autoconf.o ersatz.o hardfile.o keybuf.o expansion.o zfile.o \
       fpp.o gfxutil.o gfxlib.o blitfunc.o blittable.o \
       disk.o audio.o uaelib.o drawing.o picasso96.o  \
       uaeexe.o bsdsocket.o missing.o \
       cfgfile.o native2amiga.o identify.o debug.o \
       savestate.o writelog.o od-psp/psp.o od-psp/kbd.o od-psp/machdep/support.o \
       nogui.o od-psp/libcglue.o od-psp/sbrk.o od-psp/main_text.o
       
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
