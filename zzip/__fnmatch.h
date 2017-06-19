#ifndef __ZZIP_INTERNAL_FNMATCH_H
#define __ZZIP_INTERNAL_FNMATCH_H
/** included by fseeko.c, mmapped.c, memdisk.c */

#include <zzip/conf.h>
#include <stdio.h>
#include <string.h>

#ifdef ZZIP_HAVE_FNMATCH_H
#include <fnmatch.h>
#endif

#ifdef ZZIP_HAVE_FNMATCH_H
#define _zzip_fnmatch fnmatch
# ifdef FNM_CASEFOLD
# define _zzip_FNM_CASEFOLD FNM_CASEFOLD
# else
# define _zzip_FNM_CASEFOLD 0
# endif
#else
# define _zzip_FNM_CASEFOLD 0
#define FNM_PATHNAME    (1 << 0) /* No wildcard can ever match `/'.  */
#define FNM_NOESCAPE    (1 << 1) /* Backslashes don't quote special chars.  */
#define FNM_PERIOD      (1 << 2) /* Leading `.' is matched only explicitly.  */
/* if your system does not have fnmatch, we fall back to strcmp: */
static int _zzip_fnmatch(char* pattern, char* string, int flags)
{ 
#   ifdef DBG2
    DBG1("<zzip:mmapped:strcmp>");
#   endif
    return strcmp (pattern, string); 
}
#endif

#endif
