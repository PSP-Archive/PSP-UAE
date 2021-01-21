/*
  Main for PSP   Christophe and MIB.42 /2005
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspkerneltypes.h>
#include <pspkernel.h>
#include <pspthreadman.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspmoduleinfo.h>
#include <psppower.h>
#include <pspaudiolib.h>
#include <pspiofilemgr.h>
#include <malloc.h>

#include "main_text.h"

#define VERSION "v0.41"

#define VRAM_ADDR	(0x4000000)
#define SCREEN_WIDTH	480
#define SCREEN_HEIGHT	272
#define FRAMESIZE	0x44000
#define HALF_FRAMESIZE	0x22000

#include "sysconfig.h"
#include "sysdeps.h"
#include "gensound.h"
#include "fsdb.h"

#include "uae.h"
#include "xwin.h"
#include "gensound.h"
#include "custom.h"
#include "options.h"

#include <time.h>

typedef struct KeyStruct {	// I am not in the mood of implementing this nicely, just copied it from my kbd.c ...
	int	AmigaKeyCode;
	int	x1,y1,x2,y2;
	char*	Name;
} KeyStruct;

//in drawing.c
extern void	force_update_frame(void);
extern void finish_drawing_frame (void);

extern void	quitprogram();
extern void	uae_reset();
extern void	DrawKeyboard(void);
extern void	DrawKeyboardD(void);
extern void	CallKeyboardInput(SceCtrlData ctl);
extern char	ActualKbdString[];
extern unsigned char	SolidKeyboard;
extern int lof_changed;
extern KeyStruct AmigaKeyboard[];

/* Define the module info section */

PSP_MODULE_INFO("PSPUAE", 0, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

/* Define printf, just to make typing easier */
#define printf	pspDebugScreenPrintf

void dump_threadstatus(void);
int CheckActive(int this,int that);
extern void record_key (int);

char *bt_strings[8] = {"Left Mouse Button","Right Mouse Button","Middle Mouse Button","Joystick 0 Fire","Joystick 1 Fire","Activate Keyboard","HiRes Mouse Movement","Screenshot"};
enum {BT_LEFT_MOUSE=0,BT_RIGHT_MOUSE,BT_MIDDLE_MOUSE,BT_JOY0_FIRE,BT_JOY1_FIRE,BT_ACTIVATE_KEYBOARD,BT_HIRES_MOUSE,BT_SCREEN_SHOT,BT_KEY_ENTRIES};

#define AS_JOY0		0
#define	AS_JOY1		1
#define	AS_MOUSE	2

int g_exitUae = 0;
int g_autoframeskip = 1;
int g_draw_status = 1;
int g_display_statistics = 0;
int g_solidkeyboard = 0;
int g_adaptive_border = 0;

int g_analogstick = AS_MOUSE;
int g_dirbuttons = AS_JOY0;

int g_start = BT_ACTIVATE_KEYBOARD;
int g_square = 88 + BT_KEY_ENTRIES;
int g_triangle = BT_HIRES_MOUSE;	// Enter
int g_cross = BT_LEFT_MOUSE;
int g_circle = BT_RIGHT_MOUSE;
int g_lshoulder = BT_LEFT_MOUSE;
int g_rshoulder = BT_RIGHT_MOUSE;

int	nr_joysticks;
int 	do_screen_shot=0;

char	*OPTIONSFILENAME;
char	TmpFileName[1024];
char	g_path[1024];
char	LaunchPath[512];
int	LaunchPathEndPtr=0;

time_t time(time_t *t)
{
	return 0;
}

char copy_memory[1024];

void write_log (const char *s,...)
{
SceUID	fdout;
int	c;

return;

#if 0
	for(c=0;((c<1023)&&(s[c]!='\0'));c++) copy_memory[c]=s[c];
	copy_memory[c]='\n';
#else
va_list ap;

	va_start (ap, s);
	vsprintf (copy_memory, s, ap);
	for(c=0;((c<1023)&&(copy_memory[c]!='\0'));c++);
	copy_memory[c++]=13;
	copy_memory[c++]=10;
#endif
	LaunchPath[LaunchPathEndPtr]='\0';
	sprintf(TmpFileName,"%slog.txt",LaunchPath);
	if((fdout = sceIoOpen(TmpFileName, PSP_O_RDWR, 0777))<0)
	{
		if((fdout = sceIoOpen(TmpFileName, PSP_O_WRONLY | PSP_O_CREAT, 0777))<0)
			return;
	}
	else
		sceIoLseek32(fdout, 0, PSP_SEEK_END);
	sceIoWrite(fdout, copy_memory, c);
	sceIoClose(fdout);
}

void setup_brkhandler (void)
{ 
}

int debuggable (void)
{
  return 0;
} 

void usage (void)
{
} 

static int nblockframes=0;
static int nblockframes_sav=0;
static unsigned long lockscrstarttime=0;
static unsigned long lockscrstarttime_sav=0;

unsigned long sys_gettime(void)
{
    struct timeval tv;

    sceKernelLibcGettimeofday (&tv, NULL);
	return ((unsigned long) (tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void vsync_callback()
{
char tmp[128];

  if(sys_gettime()-lockscrstarttime>3000 && !nblockframes_sav)
	{
	  nblockframes_sav=nblockframes;
	  lockscrstarttime_sav=sys_gettime();
	}

  if(sys_gettime()-lockscrstarttime>4000)
	{
		nblockframes-=nblockframes_sav;
		lockscrstarttime=lockscrstarttime_sav;
		nblockframes_sav=0;
		lockscrstarttime_sav=0;
	}


  nblockframes++;
  if(!lockscrstarttime)
	{
		lockscrstarttime=sys_gettime();
	}

	unsigned long diff=sys_gettime()-lockscrstarttime;
//	if(diff>500)
	if(nblockframes>2)
	{
		int fps = (int)(nblockframes*1000/diff);
		//autoframerate
		if(g_autoframeskip)
		{
			if(fps<50)
			{
				changed_prefs.gfx_framerate++;
				if(changed_prefs.gfx_framerate>9) changed_prefs.gfx_framerate=9;
			}
			else
			{
				if(changed_prefs.gfx_framerate>1) changed_prefs.gfx_framerate--;
			}
		}
	}

	//changed_prefs.gfx_framerate=999999;

	if(g_display_statistics)
	{
		//display fps
		unsigned long diff=sys_gettime()-lockscrstarttime;
		if(diff>500)
		{
			int fps = (int)(nblockframes*1000/diff);
			int speed = fps*100/50;

			text_print( 0, 0, "speed", rgb2col(155,255,155),rgb2col(0,0,0),1);
			sprintf(tmp,"%d%%  ",speed);
			text_print( 0, 8*1, tmp, rgb2col(255,255,255),rgb2col(0,0,0),1);
	
			text_print( 0, 8*3, "gen.fps", rgb2col(155,255,155),rgb2col(0,0,0),1);
			sprintf(tmp,"%d ",fps);
			text_print( 0, 8*4, tmp, rgb2col(255,255,255),rgb2col(0,0,0),1);

			text_print( 0, 8*6, "frameskip", rgb2col(155,255,155),rgb2col(0,0,0),1);
			sprintf(tmp,"%d ",currprefs.gfx_framerate-1);
			text_print( 0, 8*7, tmp, rgb2col(255,255,255),rgb2col(0,0,0),1);
		}
	}
}

int lockscr (void)
{
  return 1;
} 

void unlockscr (void)
{
} 

//floating point
void fpp_opp (uae_u32 opcode, uae_u16 extra)
{
}

void fscc_opp (uae_u32 opcode, uae_u16 extra)
{
}

void fbcc_opp (uae_u32 opcode, uae_u16 extra)
{
}

void fdbcc_opp (uae_u32 opcode, uae_u16 extra)
{
}

void ftrapcc_opp (uae_u32 opcode, uaecptr oldpc)
{
}

void fsave_opp (uae_u32 opcode)
{
}

void frestore_opp (uae_u32 opcode)
{
}

//gfx stuff
int check_prefs_changed_gfx (void)
{
  return 0;
}

int graphics_init(void)
{ 
  //we use a 16-bit surface
  //todo> make it write directly into the DX texture
  gfxvidinfo.width = 360;
  gfxvidinfo.height = 272; 
  gfxvidinfo.pixbytes = 2; 
  gfxvidinfo.rowbytes = gfxvidinfo.width*gfxvidinfo.pixbytes;
  gfxvidinfo.bufmem = (char *)malloc(gfxvidinfo.width*gfxvidinfo.height*gfxvidinfo.pixbytes);
  gfxvidinfo.linemem = 0;
  gfxvidinfo.emergmem = (char *)malloc (gfxvidinfo.rowbytes);
  gfxvidinfo.maxblocklines = 10000;
  currprefs.gfx_lores = 1;
  alloc_colors64k(5,5,5,0,5,10);
  return 1;
}

int graphics_setup (void)
{ 
  return 1;
}

void graphics_leave(void)
{
} 

void flush_line (int y)
{ 
}

void flush_block (int ystart, int ystop)
{ 
}

//filesys stuff

//SnaX:14/06/03

//SnaX: Some fsdb stuff that is not defined due to non inclusion of fsdb.c

int fsdb_name_invalid(const char *a)
{
	return 0;
}

uae_u32 filesys_parse_mask(uae_u32 mask)
{
	return mask ^ 0xF;
}

struct a_inode;

void fsdb_dir_writeback(a_inode *a)
{
}

int fsdb_used_as_nname(a_inode *a,const char *b)
{
	return 0;
}

void fsdb_clean_dir(a_inode *a)
{
}

a_inode *fsdb_lookup_aino_aname(a_inode *a,const char *b)
{
	return 0;
}

a_inode *fsdb_lookup_aino_nname(a_inode *a,const char *b)
{
	return 0;
}

char *fsdb_search_dir(const char *a,char *b)
{
	return 0;
}

void fsdb_fill_file_attrs(a_inode *a)
{
}

int fsdb_set_file_attrs(a_inode *a,int b)
{
	return 0;
}

int fsdb_mode_representable_p(const a_inode *a)
{
	return 0;
}

char *fsdb_create_unique_nname(a_inode *a,const char *b)
{
	return 0;
}

//SnaX:End:14/06/03

//sound stuff
uae_u16 sndbuffer[44100];
uae_u16 *sndbufpt;
int sndbufsize; 

int init_sound (void)
{
	sample_evtime = (long)maxhpos * maxvpos * 50 / currprefs.sound_freq;

  if (currprefs.sound_bits == 16) {
	  init_sound_table16 ();
	  sample_handler = currprefs.stereo ? sample16s_handler : sample16_handler;
  } else {
	  init_sound_table8 ();
	  sample_handler = currprefs.stereo ? sample8s_handler : sample8_handler;
  }
  
  sound_available = 1;
  sndbufsize=(44100*4)/50;
  sndbufpt = sndbuffer; 

	return 1;
}

int setup_sound (void)
{
  sound_available = 1;
  return 1;
}

void close_sound(void)
{
}

//joystick stuff
void init_joystick(void)
{
  nr_joysticks = 2;
}

void close_joystick(void)
{
}

//misc stuff
unsigned int flush_icache(void)
{ 
  return 0;
}

int needmousehack (void)
{ 
  return 0;
}

void LED (int a)
{
} 

void target_save_options (FILE *f, struct uae_prefs *p)
{ 
}

int target_parse_option (struct uae_prefs *p, char *option, char *value)
{ 
  return 0;
}

void parse_cmdline (int argc, char **argv)
{ 
}

//filesys stuff

struct uaedev_mount_info *alloc_mountinfo (void)
{
	return 0;
}

void free_mountinfo (struct uaedev_mount_info *mip)
{
}

void filesys_reset (void)
{
}

void filesys_start_threads (void)
{
}

void filesys_install (void)
{
}

uaecptr filesys_initcode;
void filesys_install_code (void)
{
}

struct hardfiledata *get_hardfile_data (int nr)
{
	return 0;
}

void write_filesys_config (struct uaedev_mount_info *mountinfo,
			   const char *unexpanded, const char *default_path, FILE *f)
{
}

char *add_filesys_unit (struct uaedev_mount_info *mountinfo,
			char *volname, char *rootdir, int readonly,
			int secspertrack, int surfaces, int reserved,
			int blocksize)
{
	return 0;
}

int nr_units (struct uaedev_mount_info *mountinfo)
{
	return 0;
}

void filesys_prepare_reset (void)
{
}



void scsidev_reset (void)
{ 
}
void scsidev_start_threads (void)
{ 
}
void scsidev_install (void)
{ 
}
uaecptr scsidev_startup (uaecptr resaddr)
{
	return 0;
}

unsigned char BMPHeader[57] = {
0x42,0x4d,0x38,0xfa,0x05,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
0x00,0x00,0xe0,0x01,0x00,0x00,0x10,0x01,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x2e,0x00,0x00,0x20,0x2e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x27,0x00 };

#define	PIXELSIZE	1				//in short
#define	LINESIZE	512				//in short
#define	PIXELSIZE2	2
#define	LINESIZE2	1024

static unsigned char	KeyboardActive=0;
unsigned char		KeyboardPosition=0;		// 0=Down, 1=Up; 

char	OneLine[SCREEN_WIDTH*3];

void *pgGetVramAddr(unsigned long x,unsigned long y)
{
	return((char*)(x*PIXELSIZE2+y*LINESIZE2+0x44000000));
}

void DefaultPSPUAEOptions(void)
{
	g_draw_status = 1;
	g_display_statistics = 0;
	g_solidkeyboard = 0;
	g_adaptive_border = 1;
	g_analogstick = AS_MOUSE;
	g_dirbuttons = AS_JOY0;
	g_start = BT_SCREEN_SHOT;
	g_square = BT_ACTIVATE_KEYBOARD;
	g_triangle = BT_HIRES_MOUSE;
	g_cross = BT_LEFT_MOUSE;
	g_circle = BT_RIGHT_MOUSE;
	g_lshoulder = BT_LEFT_MOUSE;
	g_rshoulder = BT_RIGHT_MOUSE;
	changed_prefs.gfx_framerate=7;
	changed_prefs.produce_sound=1;
}


int LoadPSPUAEOptions(int slot)
{
SceUID	fdin;
int	lngth,c;

	LaunchPath[LaunchPathEndPtr]='\0';
	sprintf(TmpFileName,"%spspuae%01d.options",LaunchPath,(slot+1));
	if((fdin = sceIoOpen(TmpFileName, PSP_O_RDONLY, 0777))<0) return(1);
	lngth=sceIoLseek32(fdin,0,PSP_SEEK_END);
	if(lngth>1023) lngth=1023;
	sceIoLseek32(fdin,0,PSP_SEEK_SET);
	sceIoRead(fdin,TmpFileName,lngth);
	sceIoClose(fdin);
	c=0;
	g_autoframeskip=(int)((unsigned char)TmpFileName[c++]);
	g_draw_status=(int)((unsigned char)TmpFileName[c++]);
	g_display_statistics=(int)((unsigned char)TmpFileName[c++]);
	g_solidkeyboard=(int)((unsigned char)TmpFileName[c++]);
	g_adaptive_border=(int)((unsigned char)TmpFileName[c++]);
	g_analogstick=(int)((unsigned char)TmpFileName[c++]);
	g_dirbuttons=(int)((unsigned char)TmpFileName[c++]);
	g_start=(int)((unsigned char)TmpFileName[c++]);
	g_square=(int)((unsigned char)TmpFileName[c++]);
	g_triangle=(int)((unsigned char)TmpFileName[c++]);
	g_cross=(int)((unsigned char)TmpFileName[c++]);
	g_circle=(int)((unsigned char)TmpFileName[c++]);
	g_lshoulder=(int)((unsigned char)TmpFileName[c++]);
	g_rshoulder=(int)((unsigned char)TmpFileName[c++]);
	changed_prefs.gfx_framerate=(int)((unsigned char)TmpFileName[c++]);
	changed_prefs.produce_sound=(int)((unsigned char)TmpFileName[c++]);
	
return(0);
}

int SavePSPUAEOptions(int slot)
{
SceUID	fdout;
int	c;

	LaunchPath[LaunchPathEndPtr]='\0';
	sprintf(TmpFileName,"%spspuae%01d.options",LaunchPath,(slot+1));
	if((fdout = sceIoOpen(TmpFileName, PSP_O_WRONLY | PSP_O_CREAT, 0777))<0) return(1);

	c=0;
	TmpFileName[c++]=(unsigned char)g_autoframeskip;
	TmpFileName[c++]=(unsigned char)g_draw_status;
	TmpFileName[c++]=(unsigned char)g_display_statistics;
	TmpFileName[c++]=(unsigned char)g_solidkeyboard;
	TmpFileName[c++]=(unsigned char)g_adaptive_border;
	TmpFileName[c++]=(unsigned char)g_analogstick;
	TmpFileName[c++]=(unsigned char)g_dirbuttons;
	TmpFileName[c++]=(unsigned char)g_start;
	TmpFileName[c++]=(unsigned char)g_square;
	TmpFileName[c++]=(unsigned char)g_triangle;
	TmpFileName[c++]=(unsigned char)g_cross;
	TmpFileName[c++]=(unsigned char)g_circle;
	TmpFileName[c++]=(unsigned char)g_lshoulder;
	TmpFileName[c++]=(unsigned char)g_rshoulder;
	TmpFileName[c++]=(unsigned char)changed_prefs.gfx_framerate;
	TmpFileName[c++]=(unsigned char)changed_prefs.produce_sound;

	sceIoWrite(fdout,TmpFileName,c);
	sceIoClose(fdout);
return(0);
}

void SaveScreen(void)
{
SceUID		fdout;
int		a,b,c;
unsigned short	*clrptr,color;

	LaunchPath[LaunchPathEndPtr]='\0';
	sprintf(TmpFileName,"%s%d.bmp",LaunchPath,(int)(sys_gettime()));
	if((fdout = sceIoOpen(TmpFileName, PSP_O_WRONLY | PSP_O_CREAT, 0777))<0) return;
	sceIoWrite(fdout, BMPHeader, 57);
	for(b=0;b<SCREEN_HEIGHT;b++)
	{
		clrptr=(unsigned short *)(0x44000000+((SCREEN_HEIGHT-b-1)*LINESIZE2));
		c=0;
		for(a=0;a<SCREEN_WIDTH;a++)
		{
			color=*clrptr;
			OneLine[c]=(unsigned char)(((color>>10)&0x1F)<<3);c++;
			OneLine[c]=(unsigned char)(((color>>5)&0x1F)<<3);c++;
			OneLine[c]=(unsigned char)(((color>>0)&0x1F)<<3);c++;
			++clrptr;
		}
		sceIoWrite(fdout, OneLine, (SCREEN_WIDTH*3));
	}
	sceIoClose(fdout);
}

unsigned short __attribute__((aligned(16))) ColorLineColor=0;
unsigned short __attribute__((aligned(16))) ColorLine[60];

void flush_screen(int ystart, int ystop)
{ 
int i;
char *src = gfxvidinfo.bufmem;
char *dst;
unsigned short	color;

//src = (char *)((int)src|0x40000000); //for the ME

	if(g_adaptive_border)
	{
		color=*(unsigned short *)(gfxvidinfo.bufmem+722);
		if(ColorLineColor!=color)
		{
			ColorLineColor=color;
			for(i=0;i<60;i++) ColorLine[i]=ColorLineColor;
			dst = (char*)0x44000000;
			for(i=0;i<SCREEN_HEIGHT;i++)
			{
				memcpy(dst,(char*)ColorLine,120);
				dst+=0x348;
				memcpy(dst,(char*)ColorLine,120);
				dst+=0xB8;
			}
		}
	}

	if(KeyboardActive==0)
	{
		dst = (char*)0x44000078;	// Center
		for(i=0;i<SCREEN_HEIGHT;i++)
		{
			memcpy(dst,src,720);
			src+=720;
			dst+=LINESIZE2;
		}
	}
	else
	{
		dst = (char*)0x44000078;		// Top, Center
		if(KeyboardPosition)
		{
			dst+=133120;
			src+=93600;				// (130*(AMIGA_SCREEN_WIDTH<<1));
		}
		for(i=0;i<142;i++)				// 142 = 272-130 (130-height of keyboard)
		{
			memcpy(dst,src,720);
			src+=720;
			dst+=LINESIZE2;
		}
		DrawKeyboard();
	}
	if(do_screen_shot==1)
	{
		SaveScreen();
		do_screen_shot=2;
	}
//	sceDisplayWaitVblankStart();
}

static SceCtrlData ctl={0,};
int	MenuGo=0;


#define	ClearScreen	memset((void*)0x44000000,0,LINESIZE2*SCREEN_HEIGHT)

#define	MENU_HEIGHT	10

void showMenu(const char **menu, int defsel, char *extrainfo)
{
int	i;
int	sel=defsel;
int	ofs=0;
int	go1=1;

	ClearScreen;

	text_print( 0, 0, "PSPUAE " VERSION, rgb2col(155,255,155),rgb2col(0,0,0),1);

#define NBFILESPERPAGE 25

	int oldofs = ofs;
	if(sel >= (ofs+NBFILESPERPAGE-1)) ofs=sel-NBFILESPERPAGE+1;
	if(sel < ofs) ofs=sel;
	if(ofs != oldofs)
	{
		memset(pgGetVramAddr(0,MENU_HEIGHT*2), 0, LINESIZE*2*(SCREEN_HEIGHT-8*2));
	}
	for(i=0;menu[i+ofs] && i<NBFILESPERPAGE;i++)
	{
		text_print( 0, (2+i)*MENU_HEIGHT, (char*)(menu[i+ofs]), (((i+ofs)==sel)?rgb2col(255,255,255):rgb2col(192,192,192)),rgb2col(0,0,0),1);
	}
	while(go1)
	{
		sceCtrlReadBufferPositive(&ctl,1);
		if(!(ctl.Buttons&PSP_CTRL_SELECT)) go1=0;
		sceDisplayWaitVblankStart();
	}
}	

int	Select_Exit=0;

int doMenu(const char **menu, int defsel, char *extrainfo)
{
int	i;
int	sel = defsel;
int	waitForKey = PSP_CTRL_CROSS | PSP_CTRL_CIRCLE;
int	ofs = 0, cntr, cntr_limit, go1;

	ClearScreen;

	text_print( 0, 0, "PSPUAE " VERSION, rgb2col(155,255,155),rgb2col(0,0,0),1);

#define NBFILESPERPAGE 25

	cntr_limit=25;
	while(1)
	{
		int oldofs = ofs;
		if(sel >= (ofs+NBFILESPERPAGE-1)) ofs=sel-NBFILESPERPAGE+1;
		if(sel < ofs) ofs=sel;
		if(ofs != oldofs)
		{
			memset(pgGetVramAddr(0,MENU_HEIGHT*2), 0, LINESIZE*2*(SCREEN_HEIGHT-8*2));
		}

		for(i=0;menu[i+ofs] && i<NBFILESPERPAGE;i++)
		{
			text_print( 0, (2+i)*MENU_HEIGHT, (char*)(menu[i+ofs]), (((i+ofs)==sel)?rgb2col(255,255,255):rgb2col(192,192,192)),rgb2col(0,0,0),1);
		}

		if(waitForKey)
		{
			cntr=0;go1=1;
			while(go1)
			{
				++cntr;
				sceCtrlReadBufferPositive(&ctl,1);
				if(!(ctl.Buttons & waitForKey)) go1=0;
//				sprintf(Buffer,"  %d   ",cntr);
//				text_print( 300, 20, Buffer, rgb2col(255,255,255),rgb2col(0,0,0),1);
				if(cntr>cntr_limit)
				{
					go1=0;
					cntr_limit=5;
				}
				sceDisplayWaitVblankStart();

			}
			waitForKey = 0;
		}

		while(1)
		{
			sceCtrlReadBufferPositive(&ctl, 1);
			if(CheckActive(ctl.Buttons,BT_SCREEN_SHOT)) SaveScreen();
			if(ctl.Buttons & PSP_CTRL_CIRCLE)
			{
				if(extrainfo!=NULL) *extrainfo=0;
				Select_Exit=0;
				return -1;
			}

			if(ctl.Buttons & PSP_CTRL_SELECT)
			{
				if(extrainfo!=NULL) *extrainfo=0;
				Select_Exit=1;
				MenuGo=0;
				return -1;
			}
			
			if(ctl.Buttons & PSP_CTRL_CROSS)
			{
				if(extrainfo!=NULL) *extrainfo=1;
				Select_Exit=0;
				return sel;
			}

			if((ctl.Buttons & PSP_CTRL_SQUARE)&&(ctl.Buttons & PSP_CTRL_TRIANGLE))
			{
				text_print( 0, 0, "PSPUAE " VERSION " by MIB.42", rgb2col(155,255,155),rgb2col(0,0,0),1);
			}

			if(extrainfo!=NULL)
			{
				if(ctl.Buttons & PSP_CTRL_LEFT)
				{
					go1=1;
					while(go1)
					{
						sceCtrlReadBufferPositive(&ctl,1);
						if(!(ctl.Buttons & PSP_CTRL_LEFT)) go1=0;
					}
					*extrainfo=-1;
					Select_Exit=0;	
					return sel;
				}
				if(ctl.Buttons & PSP_CTRL_RIGHT)
				{
					go1=1;
					while(go1)
					{
						sceCtrlReadBufferPositive(&ctl,1);
						if(!(ctl.Buttons & PSP_CTRL_RIGHT)) go1=0;
					}
					*extrainfo=1;
					Select_Exit=0;	
					return sel;
				}
			}
			if(ctl.Buttons & PSP_CTRL_DOWN || ctl.Buttons & PSP_CTRL_RTRIGGER)
			{
				if(!menu[sel+1]) continue;
				sel++;
				waitForKey = PSP_CTRL_DOWN;
				break;
			}
			else
				if(ctl.Buttons & PSP_CTRL_UP || ctl.Buttons & PSP_CTRL_LTRIGGER)
				{
					if(sel>0) sel --;
					waitForKey = PSP_CTRL_UP;
					break;
				}
				else cntr_limit=25;
		}
	}

	return sel;
}

const char *mainmenu[]={
  "Insert floppy",
  "Reset amiga",
//  "Map keys",
//  "Simulate keypress",
//  "Load state",
//  "Save state",
  "Options",
//  configstr,		//SnaX:14/06/03 - Multi Config Support
  "Quit PSPUAE",
  NULL
};

char floppyitems[4][256];
char floppynr[256];
const char *floppymenu[]={
  floppyitems[0],
  floppyitems[1],
  floppyitems[2],
  floppyitems[3],
//  floppynr,
  NULL
};

const char *optmenu[]={
	"Auto frameskip: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Frameskip: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Sound emulation: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Show drives status: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Show statistics: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Transparent Keyboard: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Adaptive Borders: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Analog stick: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Directional Buttons: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Start   : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Square  : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Triangle: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Cross   : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Circle  : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Left Shoulder Button : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"Right Shoulder Button: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"  Load Options #1",
	"  Load Options #2",
	"  Save Options #1",
	"  Save Options #2",
	"  Default Options",
	NULL
};

const char *inputselection[3]={	// g_analogstick
"Joystick 0     ",
"Joystick 1     ",
"Mouse          "
};


const char *frskipmenu[]={
	"0","1","2","3","4","5","6","7","8",NULL
};

SceIoDirent foundfile;

static int prev_ctl_buttons=0;

int CheckActive(int this,int that)
{
	if(g_cross==that) {if(this&PSP_CTRL_CROSS) return(1);}
	if(g_circle==that) {if(this&PSP_CTRL_CIRCLE) return(1);}
	if(g_square==that) {if(this&PSP_CTRL_SQUARE) return(1);}
	if(g_triangle==that) {if(this&PSP_CTRL_TRIANGLE) return(1);}
	if(g_lshoulder==that) {if(this&PSP_CTRL_LTRIGGER) return(1);}
	if(g_rshoulder==that) {if(this&PSP_CTRL_RTRIGGER) return(1);}
	if(g_start==that) {if(this&PSP_CTRL_START) return(1);}
return(0);
}


void InvokeKeyPress(int raw_value)
{
	record_key((AmigaKeyboard[(raw_value-BT_KEY_ENTRIES)].AmigaKeyCode)<<1);
	record_key(((AmigaKeyboard[(raw_value-BT_KEY_ENTRIES)].AmigaKeyCode)<<1)|1);
}

#define THRESHOLD 30

void handle_events (void)	//update mouse pos.
{
int	ax;
int	ay;
int	res2;
char	extrainfo;

	sceCtrlReadBufferPositive(&ctl, 1);

	if(g_analogstick==AS_MOUSE)		// Mouse on Analog
	{
		ax = (int)ctl.Lx-127;
		ay = (int)ctl.Ly-127;

		if(CheckActive(ctl.Buttons,BT_HIRES_MOUSE))	// If HiresMouse button is pressed, extra precision is required
		{
			if(ax < -THRESHOLD) lastmx+= ((ax+THRESHOLD)>>5);
			if(ax > THRESHOLD) lastmx+= ((ax-THRESHOLD)>>5);
			if(ay < -THRESHOLD) lastmy+= ((ay+THRESHOLD)>>5);
			if(ay > THRESHOLD) lastmy+= ((ay-THRESHOLD)>>5);
		}
		else
		{
			if(ax < -THRESHOLD) lastmx+= ((ax+THRESHOLD)>>3);
			if(ax > THRESHOLD) lastmx+= ((ax-THRESHOLD)>>3);
			if(ay < -THRESHOLD) lastmy+= ((ay+THRESHOLD)>>3);
			if(ay > THRESHOLD) lastmy+= ((ay-THRESHOLD)>>3);
		}
	}
	else
	if(g_dirbuttons==AS_MOUSE)
	{
		if(CheckActive(ctl.Buttons,BT_HIRES_MOUSE)) ax=2; else ax=5;
		if(ctl.Buttons&PSP_CTRL_LEFT) lastmx-=ax;
		if(ctl.Buttons&PSP_CTRL_RIGHT) lastmx+=ax;
		if(ctl.Buttons&PSP_CTRL_UP) lastmy-=ax;
		if(ctl.Buttons&PSP_CTRL_DOWN) lastmy+=ax;
	}

	if((CheckActive(ctl.Buttons,BT_ACTIVATE_KEYBOARD))&&(!(CheckActive(prev_ctl_buttons,BT_ACTIVATE_KEYBOARD))))	// Turn ON/OFF Keyboard
//	if(((ctl.Buttons & PSP_CTRL_SQUARE)>0)&&((prev_ctl_buttons & PSP_CTRL_SQUARE)==0))	// Turn ON/OFF Keyboard
	{
		ClearScreen;
		if(KeyboardActive)
			KeyboardActive=0;
		else
		{
			KeyboardActive=1;
			if(SolidKeyboard) DrawKeyboardD();
		}
		flush_screen(0,0);
	}

	if(KeyboardActive)
	{
		CallKeyboardInput(ctl);
		if(g_analogstick==AS_MOUSE)	// mouse is on analog, so temporarily transfer dir_buttons
		{
			buttonstate[0] = (ctl.Buttons & PSP_CTRL_LTRIGGER )>0;			//mouse left button
			buttonstate[2] = (ctl.Buttons & PSP_CTRL_RTRIGGER )>0;			//mouse right button
		}
	}
	else
	{
		if(CheckActive(ctl.Buttons,BT_LEFT_MOUSE))			//mouse left button
			buttonstate[0] = 1;
		else
			buttonstate[0] = 0;

		if(CheckActive(ctl.Buttons,BT_MIDDLE_MOUSE))			//mouse middle button
			buttonstate[1] = 1;
		else
			buttonstate[1] = 0;

		if(CheckActive(ctl.Buttons,BT_RIGHT_MOUSE))			//mouse right button
			buttonstate[2] = 1;
		else
			buttonstate[2] = 0;

		if(g_cross>=BT_KEY_ENTRIES) {if(ctl.Buttons&PSP_CTRL_CROSS) InvokeKeyPress(g_cross);}
		if(g_circle>=BT_KEY_ENTRIES) {if(ctl.Buttons&PSP_CTRL_CIRCLE) InvokeKeyPress(g_circle);}
		if(g_square>=BT_KEY_ENTRIES) {if(ctl.Buttons&PSP_CTRL_SQUARE) InvokeKeyPress(g_square);}
		if(g_triangle>=BT_KEY_ENTRIES) {if(ctl.Buttons&PSP_CTRL_TRIANGLE) InvokeKeyPress(g_triangle);}
		if(g_lshoulder>=BT_KEY_ENTRIES) {if(ctl.Buttons&PSP_CTRL_LTRIGGER) InvokeKeyPress(g_lshoulder);}
		if(g_rshoulder>=BT_KEY_ENTRIES) {if(ctl.Buttons&PSP_CTRL_RTRIGGER) InvokeKeyPress(g_rshoulder);}
		if(g_start>=BT_KEY_ENTRIES) {if(ctl.Buttons&PSP_CTRL_START) InvokeKeyPress(g_start);}
	}

	if(CheckActive(ctl.Buttons,BT_SCREEN_SHOT))
	{
		if(do_screen_shot==0)
		{
			do_screen_shot=1;
		}
		else
			flush_screen(0,0);
	}
	else
	{
		if(do_screen_shot==2) do_screen_shot=0;		// Can take an other if previous is done...
	}

	if(ctl.Buttons & PSP_CTRL_SELECT)		// MENU
	{
	static int lastsel = 0;
		//menu
		MenuGo=1;
		showMenu(mainmenu, lastsel, NULL);
		while(MenuGo)
		{
			int res = doMenu(mainmenu, lastsel, NULL);
			if(res == -1) {MenuGo=0;break;}
			lastsel = res;
			if(res == 0)
			{
				//insert disk
				while(1)
				{
					int i;
					for(i=0;i<4;i++)
						sprintf(floppyitems[i],"Insert in DF%i: (%s)",i,changed_prefs.df[i]);
	
					int res = doMenu(floppymenu, 0, NULL);
					if(res == -1) break;

					//file menu
					char *tmpnames[4096];
					memset(tmpnames,0,sizeof(tmpnames));

					//scan directory
					int nb=0;
					tmpnames[nb++]=strdup("<empty>");

					char path[4096];
					strcpy(path, g_path);
					strcat(path, "/DISKS/");

					int fd = sceIoDopen(path); 
					if(fd>=0)
					{
						while(nb<4000)
						{
							if(sceIoDread(fd, &foundfile)<=0) break;
							if(foundfile.d_name[0] == '.') continue;
							if(FIO_SO_ISDIR(foundfile.d_stat.st_mode)) continue;
							tmpnames[nb++]=strdup(foundfile.d_name);
						}
						sceIoDclose(fd); 
					}

					int res2=doMenu((const char **)&tmpnames,0,NULL);
					if(res2 != -1)
					{
						if(!strcmp(tmpnames[res2],"<empty>")) 
						{
							//eject floppy
							changed_prefs.df[res][0]=0;
			            }
						else
						{
							strcpy(changed_prefs.df[res], g_path);
							strcat(changed_prefs.df[res], "/DISKS/");
							strcat(changed_prefs.df[res], tmpnames[res2]);
						}
					}
				}
			}
			else if (res == 1)
			{
				//reset amiga
				uae_reset();
				break;
			}
			else if (res == 2)
			{
				//options
				static int lastsel = 0;
				while(1)
				{
					sprintf((char*)(optmenu[0]), "Auto frameskip: %s", g_autoframeskip?"ON":"OFF");
					sprintf((char*)(optmenu[1]), "Frameskip: %d", changed_prefs.gfx_framerate-1);
					sprintf((char*)(optmenu[2]), "Sound emulation: %s", changed_prefs.produce_sound?"ON":"OFF");
					sprintf((char*)(optmenu[3]), "Show drives status: %s", g_draw_status?"ON":"OFF");
					sprintf((char*)(optmenu[4]), "Show statistics: %s", g_display_statistics?"ON":"OFF");
					sprintf((char*)(optmenu[5]), "Transparent Keyboard: %s", g_solidkeyboard?"OFF":"ON");
					sprintf((char*)(optmenu[6]), "Adaptive Borders: %s", g_adaptive_border?"ON":"OFF");
					sprintf((char*)(optmenu[7]), "Analog stick: %s",inputselection[g_analogstick]);
					sprintf((char*)(optmenu[8]), "Directional Buttons: %s",inputselection[g_dirbuttons]);
					if(g_start<BT_KEY_ENTRIES)
						sprintf((char*)(optmenu[9]), "Start   : %s",bt_strings[g_start]);
					else
						sprintf((char*)(optmenu[9]), "Start   : %s",AmigaKeyboard[g_start-BT_KEY_ENTRIES].Name);
					if(g_square<BT_KEY_ENTRIES)
						sprintf((char*)(optmenu[10]), "Square  : %s",bt_strings[g_square]);
					else
						sprintf((char*)(optmenu[10]), "Square  : %s",AmigaKeyboard[g_square-BT_KEY_ENTRIES].Name);
					if(g_triangle<BT_KEY_ENTRIES)
						sprintf((char*)(optmenu[11]), "Triangle: %s",bt_strings[g_triangle]);
					else
						sprintf((char*)(optmenu[11]), "Triangle: %s",AmigaKeyboard[g_triangle-BT_KEY_ENTRIES].Name);
					if(g_cross<BT_KEY_ENTRIES)
						sprintf((char*)(optmenu[12]),"Cross   : %s",bt_strings[g_cross]);
					else
						sprintf((char*)(optmenu[12]),"Cross   : %s",AmigaKeyboard[g_cross-BT_KEY_ENTRIES].Name);
					if(g_circle<BT_KEY_ENTRIES)
						sprintf((char*)(optmenu[13]),"Circle  : %s",bt_strings[g_circle]);
					else
						sprintf((char*)(optmenu[13]),"Circle  : %s",AmigaKeyboard[g_circle-BT_KEY_ENTRIES].Name);
					if(g_lshoulder<BT_KEY_ENTRIES)
						sprintf((char*)(optmenu[14]),"Left Shoulder Button : %s",bt_strings[g_lshoulder]);
					else
						sprintf((char*)(optmenu[14]),"Left Shoulder Button : %s",AmigaKeyboard[g_lshoulder-BT_KEY_ENTRIES].Name);
					if(g_rshoulder<BT_KEY_ENTRIES)
						sprintf((char*)(optmenu[15]),"Right Shoulder Button: %s",bt_strings[g_rshoulder]);
					else
						sprintf((char*)(optmenu[15]),"Right Shoulder Button: %s",AmigaKeyboard[g_rshoulder-BT_KEY_ENTRIES].Name);

					int res = doMenu(optmenu, lastsel, &extrainfo);
					if(res == -1) break;
					lastsel = res;
					switch(res)
					{
						case 0 :
							g_autoframeskip = !g_autoframeskip;
						break;
						case 1 :
							res2 = doMenu(frskipmenu, changed_prefs.gfx_framerate-1,NULL);
							if(res2 != -1) changed_prefs.gfx_framerate = res2+1;
						break;
						case 2 :
							if(changed_prefs.produce_sound) changed_prefs.produce_sound = 0;
							else changed_prefs.produce_sound = 2;
						break;
						case 3 :
							g_draw_status = !g_draw_status;
						break;
						case 4 :
							g_display_statistics = !g_display_statistics;
						break;
						case 5 :
							if(++g_solidkeyboard>1) g_solidkeyboard=0;
							SolidKeyboard=g_solidkeyboard;
						break;
						case 6 :
							if(++g_adaptive_border>1) g_adaptive_border=0;
						break;
						case 7 :
							if((g_analogstick+=extrainfo)>AS_MOUSE) g_analogstick=AS_JOY0;
							if(g_analogstick<0) g_analogstick=AS_MOUSE;
						break;
						case 8 :
							if((g_dirbuttons+=extrainfo)>AS_MOUSE) g_dirbuttons=AS_JOY0;
							if(g_dirbuttons<0) g_dirbuttons=AS_MOUSE;
						break;
						case 9 :
							if((g_start+=extrainfo)>(BT_KEY_ENTRIES+92)) g_start=0;
							if(g_start<0) g_start=(BT_KEY_ENTRIES+92);
						break;
						case 10 :
							if((g_square+=extrainfo)>(BT_KEY_ENTRIES+92)) g_square=0;
							if(g_square<0) g_square=(BT_KEY_ENTRIES+92);
						break;
						case 11 :
							if((g_triangle+=extrainfo)>(BT_KEY_ENTRIES+92)) g_triangle=0;
							if(g_triangle<0) g_triangle=(BT_KEY_ENTRIES+92);
						break;
						case 12 :
							if((g_cross+=extrainfo)>(BT_KEY_ENTRIES+92)) g_cross=0;
							if(g_cross<0) g_cross=(BT_KEY_ENTRIES+92);
						break;
						case 13 :
							if((g_circle+=extrainfo)>(BT_KEY_ENTRIES+92)) g_circle=0;
							if(g_circle<0) g_circle=(BT_KEY_ENTRIES+92);
						break;
						case 14 :
							if((g_lshoulder+=extrainfo)>(BT_KEY_ENTRIES+92)) g_lshoulder=0;
							if(g_lshoulder<0) g_lshoulder=(BT_KEY_ENTRIES+92);
						break;
						case 15 :
							if((g_rshoulder+=extrainfo)>(BT_KEY_ENTRIES+92)) g_rshoulder=0;
							if(g_rshoulder<0) g_rshoulder=(BT_KEY_ENTRIES+92);
						break;
						case 16 :
						case 17 :
							LoadPSPUAEOptions((res-16));
						break;
						case 18 :
						case 19 :
							SavePSPUAEOptions((res-18));
						break;
						case 20 :
							DefaultPSPUAEOptions();
						break;
					}
				}
			}
			else if (res == 3)
			{
				//quit
				quitprogram();
				break;
			}
		}
		ClearScreen;
		if(SolidKeyboard) DrawKeyboardD();
		ColorLineColor=0xFFFF;
		lof_changed=1;
		force_update_frame();
		flush_screen(0,0);
		if(Select_Exit)
		{
			res2=1;
			while(res2)
			{
				sceCtrlReadBufferPositive(&ctl,1);
				if(!(ctl.Buttons&PSP_CTRL_SELECT)) res2=0;
				sceDisplayWaitVblankStart();
			}
		}
	}

	prev_ctl_buttons=ctl.Buttons;
}

void read_joystick(int nr, unsigned int *dir, int *button)	// 0, 1
{
int	left = 0, right = 0, up = 0, down = 0; 

	if(nr > nr_joysticks) return;

//	sceCtrlReadBufferPositive(&ctl, 1);	// handle_events already took care of this...

	if(g_analogstick==nr)		// Joy0 / Joy1
	{
		if(ctl.Lx<(128-THRESHOLD)) left=1;
		if(ctl.Lx>(128+THRESHOLD)) right=1;
		if(ctl.Ly<(128-THRESHOLD)) up=1;
		if(ctl.Ly>(128+THRESHOLD)) down=1;

	}
	else
	if((g_dirbuttons==nr)&&(!KeyboardActive))
	{
		if(ctl.Buttons&PSP_CTRL_LEFT) left=1;
		if(ctl.Buttons&PSP_CTRL_RIGHT) right=1;
		if(ctl.Buttons&PSP_CTRL_UP) up=1;
		if(ctl.Buttons&PSP_CTRL_DOWN) down=1;
	}

	if(CheckActive(ctl.Buttons,(BT_JOY0_FIRE+nr))) *button=1; else *button=0;
	if(left) up = !up;
	if(right) down = !down;
	*dir = down | (right << 1) | (up << 8) | (left << 9);	
}

void finish_sound_buffer (void)
{
//  g_xbox->write_sound();
}


/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
	quitprogram();
	return(0);
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return(0);
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

void sound_callback(void *buf, unsigned int reqn, void *pdata)
{
int i;
short *dst=buf;
short *src=(short *)sndbuffer;

	while(reqn>((int)sndbufpt-(int)(&sndbuffer)))
	{
		sceKernelDelayThread(1000); 
	}

	for(i=0;i<reqn;i++)
	{
		short a=src[i/2];
		dst[i*2]=a;
		dst[i*2+1]=a;
	}
	sndbufpt = (uae_u16*)src; 
}

int main(int argc, char* argv[])
{
int	a;

	DefaultPSPUAEOptions();

	for(a=0;((a<512)&&(argv[0][a]!='\0'));a++)
	{
		LaunchPath[a]=argv[0][a];
	}
	while((a>0)&&(LaunchPath[a]!='/')) --a;
	LaunchPath[a++]='/';
	LaunchPathEndPtr=a;
	LaunchPath[a]='\0';

	sceDisplaySetMode( 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	sceDisplaySetFrameBuf( (char*)VRAM_ADDR, 512, 1, 1 );

	SetupCallbacks();
	pspAudioInit();
	pspAudioSetChannelCallback(0, (void *)&sound_callback, 0); 

	LoadPSPUAEOptions(0);

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(1);
	scePowerSetClockFrequency(333,333,166);

	char optionsPath[1024];
	{
	  strcpy(g_path, LaunchPath);
  	  char *p=g_path+strlen(g_path)-1;
	  while(*p!='/') p--;
	  *p=0;
	}
	strcpy(optionsPath, g_path);
	strcat(optionsPath, "/config.uae");

	OPTIONSFILENAME = optionsPath;

	real_main (0, NULL);

	pspAudioEnd();

	sceKernelExitGame();
	return 0;
} 
