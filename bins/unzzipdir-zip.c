/*
 *	Copyright (c) 2003 Guido Draheim <guidod@gmx.de>
 *      Use freely under the restrictions of the ZLIB license.
 *
 *      This file is used as an example to clarify zzip api usage.
 */

#include <zzip/lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unzzipdir-zip.h"

#ifdef ZZIP_HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef ZZIP_HAVE_IO_H
#include <io.h>
#endif

#ifdef ZZIP_HAVE_FNMATCH_H
#include <fnmatch.h>
#else
#define fnmatch(x,y,z) strcmp(x,y)
#endif

static const char* comprlevel[] = {
    "stored",   "shrunk",   "redu:1",   "redu:2",   "redu:3",   "redu:4",
    "impl:N",   "toknze",   "defl:N",   "defl:B",   "impl:B" };


static int 
unzzip_list (int argc, char ** argv, int verbose)
{
    int argn;
    ZZIP_DIR* disk;
    zzip_error_t error;
    
    if (argc == 1)
    {
        printf (__FILE__" version "ZZIP_PACKAGE" "ZZIP_VERSION"\n");
        return -1; /* better provide an archive argument */
    }
    
    disk = zzip_dir_open (argv[1], &error);
    if (! disk) {
	perror(argv[1]);
	return -1;
    }

    if (argc == 2)
    {  /* list all */
	ZZIP_DIRENT entry;
	while(zzip_dir_read(disk, &entry))
	{
	    char* name = entry.d_name;
	    long long usize = entry.st_size;
	    if (!verbose)
	    {
		printf ("%22lli %s\n", usize, name);
	    } else
	    {
		long long csize = entry.d_csize;
		unsigned compr = entry.d_compr;
		const char* defl = (compr < sizeof(comprlevel)) ? comprlevel[compr] : "(redu)";
		printf ("%lli/%lli %s %s\n", usize, csize, defl, name);
	    }
	}
    }
    else
    {   /* list only the matching entries - in order of zip directory */
	ZZIP_DIRENT entry;
	while(zzip_dir_read(disk, &entry))
	{
	    char* name = entry.d_name;
	    for (argn=1; argn < argc; argn++)
	    {
		if (! fnmatch (argv[argn], name, 
			       FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD))
		{
		    long long usize = entry.st_size;
		    if (!verbose)
		    {
			printf ("%22lli %s\n", usize, name);
		    } else
		    {
			long long csize = entry.d_csize;
			unsigned compr = entry.d_compr;
			const char* defl = (compr < sizeof(comprlevel)) ? comprlevel[compr] : "(redu)";
			printf ("%lli/%lli %s %s\n", usize, csize, defl, name);
		    }
		    break; /* match loop */
		}
	    }
	}
    }
    zzip_dir_close(disk);
    return 0;
} 

int 
unzzip_long_list (int argc, char ** argv)
{
    return unzzip_list(argc, argv, 1);
}

int 
unzzip_show_list (int argc, char ** argv)
{
    return unzzip_list(argc, argv, 0);
}

/* 
 * Local variables:
 * c-file-style: "stroustrup"
 * End:
 */
