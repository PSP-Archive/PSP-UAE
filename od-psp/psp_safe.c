#include <pspkerneltypes.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspdebug.h>
#include <pspaudiolib.h>
#include <pspiofilemgr.h>
#include <stdlib.h>
#include <string.h>

#include "main_text.h"

#define VERSION "0.31 Kbd"

#define VRAM_ADDR	(0x04000000)
#define SCREEN_WIDTH	480
#define SCREEN_HEIGHT	272

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

long		scePowerSetClockFrequency(long,long,long); 
extern void	quitprogram();
extern void	uae_reset();
extern char	ActualKbdString[];


/* Define the module info section */
PSP_MODULE_INFO("SDKTEST", 0, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

/* Define printf, just to make typing easier */
#define printf	pspDebugScreenPrintf

void dump_threadstatus(void);

int g_exitUae = 0;
int g_autoframeskip = 1;
int g_draw_status = 1;
int nr_joysticks;
char *OPTIONSFILENAME;
char g_path[1024];
char g_elf_name[1024];

time_t time(time_t *t)
{
	return 0;
}

char copy_memory[1024];

#define	LOG_PLACE	"ms0:/PSP/GAME/PSPUAE/log.txt"

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
	if((fdout = sceIoOpen(LOG_PLACE, PSP_O_RDWR, 0777))<0)
	{
		if((fdout = sceIoOpen(LOG_PLACE, PSP_O_WRONLY | PSP_O_CREAT, 0777))<0)
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

	text_print( 0, 8*10, "Keyboard", rgb2col(155,255,155),rgb2col(0,0,0),1);

	//changed_prefs.gfx_framerate=999999;

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

#define	PIXELSIZE	1				//in short
#define	LINESIZE	512				//in short
#define	PIXELSIZE2	2
#define	LINESIZE2	1024

static unsigned char	KeyboardActive=0;		// 1 on bottom, 2 on top

char *pg_vramtop=(char *)0x04000000;

char *pgGetVramAddr(unsigned long x,unsigned long y)
{
	return pg_vramtop+x*PIXELSIZE*2+y*LINESIZE*2+0x40000000;
}

void flush_screen (int ystart, int ystop)
{ 
int i,si,ei;
char *src = gfxvidinfo.bufmem;
char *dst;

	if(KeyboardActive==0)
	{
		for(i=0;i<272;i++)
		{
			dst = ((60*PIXELSIZE2)+0x44000000+(i*LINESIZE2));	// Center
			memcpy(dst,src,720);
			src+=720;
		}
	}
}

static SceCtrlData ctl={0,};

void clearScreen()
{
	memset(pgGetVramAddr(0,0), 0, LINESIZE*2*272);
}

int doMenu(const char **menu, int defsel)
{
int i;
int sel = defsel;
int waitForKey = PSP_CTRL_CROSS | PSP_CTRL_CIRCLE;
int ofs = 0;

	clearScreen();

	text_print( 0, 0, "PSPUAE " VERSION, rgb2col(155,255,155),rgb2col(0,0,0),1);

#define NBFILESPERPAGE 31

	while(1)
	{
		int oldofs = ofs;
		if(sel >= (ofs+NBFILESPERPAGE-1)) ofs=sel-NBFILESPERPAGE+1;
		if(sel < ofs) ofs=sel;
		if(ofs != oldofs)
		{
			memset(pgGetVramAddr(0,8*2), 0, LINESIZE*2*(272-8*2));
		}

		for(i=0;menu[i+ofs] && i<NBFILESPERPAGE;i++)
		{
			text_print( 0, (2+i)*8, menu[i+ofs], ((i+ofs)==sel)?rgb2col(255,255,255):rgb2col(192,192,192),rgb2col(0,0,0),1);
		}

		if(waitForKey)
		{
			while(1)
			{
				sceCtrlReadBufferPositive(&ctl, 1);
				if(!(ctl.Buttons & waitForKey)) break;
			}
			waitForKey = 0;
		}

		while(1)
		{
			sceCtrlReadBufferPositive(&ctl, 1);
			if(ctl.Buttons & PSP_CTRL_CROSS) return -1;
			if(ctl.Buttons & PSP_CTRL_DOWN || ctl.Buttons & PSP_CTRL_RTRIGGER)
			{
				if(!menu[sel+1]) continue;
				sel++;
				waitForKey = PSP_CTRL_DOWN;
				break;
			}
			if(ctl.Buttons & PSP_CTRL_UP || ctl.Buttons & PSP_CTRL_LTRIGGER)
			{
				if(sel>0) sel --;
				waitForKey = PSP_CTRL_UP;
				break;
			}
			if(ctl.Buttons & PSP_CTRL_CIRCLE) return sel;
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
	"Auto frameskip: xxxxxxxxxx",
	"Frameskip: xx",
	"Sound emulation: xxxxxxxxxx",
	"Show drives status: xxxxxxxx",
	NULL
};

const char *frskipmenu[]={
	"0","1","2","3","4","5","6","7","8",NULL
};

SceIoDirent foundfile;

void handle_events (void)
{ 
	//update mouse pos.
int ax = (int)ctl.Lx-127;
int ay = (int)ctl.Ly-127;
int threshold = 30;

	if(ax < -threshold) lastmx+= (ax+threshold)/8;
	if(ax > threshold) lastmx+= (ax-threshold)/8;
	if(ay < -threshold) lastmy+= (ay+threshold)/8;
	if(ay > threshold) lastmy+= (ay-threshold)/8;

	if(ctl.Buttons & PSP_CTRL_SELECT)
	{
		static int lastsel = 0;
		//menu
		while(1)
		{
			int res = doMenu(mainmenu, lastsel);
			if(res == -1) break;
			lastsel = res;
			if(res == 0)
			{
				//insert disk
				while(1)
				{
					int i;
					for(i=0;i<4;i++)
						sprintf(floppyitems[i],"Insert in DF%i: (%s)",i,changed_prefs.df[i]);
	
					int res = doMenu(floppymenu, 0);
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

					int res2=doMenu((const char **)&tmpnames,0);
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
					sprintf(optmenu[0], "Auto frameskip: %s", g_autoframeskip?"ON":"OFF");
					sprintf(optmenu[1], "Frameskip: %d", changed_prefs.gfx_framerate-1);
					sprintf(optmenu[2], "Sound emulation: %s", changed_prefs.produce_sound?"ON":"OFF");
					sprintf(optmenu[3], "Show drives status: %s", g_draw_status?"ON":"OFF");
					int res = doMenu(optmenu, lastsel);
					if(res == -1) break;
					lastsel = res;
					if(res == 0) g_autoframeskip = !g_autoframeskip;
					if(res == 1)
					{
						int res2 = doMenu(frskipmenu, changed_prefs.gfx_framerate-1);
						if(res2 != -1) changed_prefs.gfx_framerate = res2+1;
					}
					if(res == 2)
					{
						if(changed_prefs.produce_sound) changed_prefs.produce_sound = 0;
						else changed_prefs.produce_sound = 2;
					}
					if(res == 3) g_draw_status = !g_draw_status;
				}
			}
			else if (res == 3)
			{
				//quit
				quitprogram();
				break;
			}
		}
		clearScreen();
	}
}

void read_joystick(int nr, unsigned int *dir, int *button)
{
*dir = 0;
*button = 0;


	if(nr > nr_joysticks) return;

	sceCtrlReadBufferPositive(&ctl, 1);

	if((ctl.Buttons & PSP_CTRL_SQUARE)>0)
	{
		CallKeyboardInput(ctl);
		return;
	}

	*button = (ctl.Buttons & PSP_CTRL_CIRCLE)>0;

	int left = 0, right = 0, top = 0, bot = 0; 

	left = (ctl.Buttons & PSP_CTRL_LEFT)>0;
	right = (ctl.Buttons & PSP_CTRL_RIGHT)>0;
	top = (ctl.Buttons & PSP_CTRL_UP)>0;
	bot = (ctl.Buttons & PSP_CTRL_DOWN)>0;

	if (left) top = !top;
	if (right) bot = !bot;
	*dir = bot | (right << 1) | (top << 8) | (left << 9);   

	buttonstate[0] = (ctl.Buttons & PSP_CTRL_LTRIGGER )>0;  //mouse left button
	buttonstate[2] = (ctl.Buttons & PSP_CTRL_RTRIGGER )>0;  //mouse right button
}

void finish_sound_buffer (void)
{
//  g_xbox->write_sound();
}


/* Exit callback */
int exit_callback(void)
{
	quitprogram();
	return 0;
}

/* Callback thread */
void CallbackThread(void *arg)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();
	return;
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

void sound_callback(void *buf, unsigned int reqn)
{
	int i;
	short *dst=buf;
	short *src=sndbuffer;
	while(reqn>((int)sndbufpt-(int)(&sndbuffer)))
	{
		sceKernelDelayThread(1000); 
	}

	for(i=0;i<reqn;i++)
	{
		short a=sndbuffer[i/2];
		dst[i*2]=a;
		dst[i*2+1]=a;
	}
	sndbufpt = sndbuffer; 
}

int main(void)
{

	ActualKbdString[0]=0;

	sceDisplaySetMode( 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	sceDisplaySetFrameBuf( (char*)VRAM_ADDR, 512, 1, 1 );

	SetupCallbacks();
	pspAudioInit();
	pspAudioSetChannelCallback(0, (void *)&sound_callback); 

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(1);
	scePowerSetClockFrequency(333,333,166);

	char optionsPath[1024];
	{
	  strcpy(g_path, g_elf_name);
  	  char *p=g_path+strlen(g_path)-1;
	  while(*p!='/') p--;
	  *p=0;
	}
	strcpy(optionsPath, g_path);
	strcat(optionsPath, "/config.uae");

	OPTIONSFILENAME = optionsPath;

	real_main (0, NULL);

	sceKernelExitGame();
	return 0;
} 
