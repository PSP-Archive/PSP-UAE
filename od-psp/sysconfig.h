/* The number of bytes in a char.  */
#define SIZEOF_CHAR 1

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

#ifdef __GNUC__
/* The number of bytes in a long long.  */
#define SIZEOF_LONG_LONG 8
#define SIZEOF___INT64 0
#else
#define SIZEOF_LONG_LONG 0
#define SIZEOF___INT64 8
#endif

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/stat.h> header file.  */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/types.h> header file.  */
#define HAVE_SYS_TYPES_H 1

// Windows-specific #defines
//#ifndef __GNUC__
#define REGPARAM
//#define REGPARAM2
//#define __inline__ __inline
//#define __asm__(a) ;
//#define O_NDELAY 0
//#endif
#define lseek _lseek

#define R_OK 1
