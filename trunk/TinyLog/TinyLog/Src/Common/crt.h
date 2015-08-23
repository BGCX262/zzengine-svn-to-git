#ifndef _TINY_LOG_CRT_H
#define _TINY_LOG_CRT_H

#if (defined(_MSC_VER) || defined(__ICL))	//window platform
#include <direct.h>
#include <io.h>
#else
#include <pthread.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#include <map>
#include <vector>


#define NELEMS(x)	(sizeof((x))/(sizeof(((x)[0]))))

#endif	//_TINY_LOG_CRT_H