/*
 *	Log System for Engine
 *	2010-11-18 by AnIcepIg
 */
#ifndef _ZENGINE_LOG_H
#define _ZENGINE_LOG_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

enum LOG_OPTIONS
{
	LOG_OPTIONS_NONE	= 0x00,
	LOG_OPTIONS_FILE	= 0x01,
	LOG_OPTIONS_STDERR	= 0x02
};

typedef struct LOG_PARAM {
	int cb;
	char path[max_path];
	char ident[max_path];
	int options;		/* options = 0 && path is not empty for file */
}LOG_PARAM;

ZENGINE_API int log_init(LOG_PARAM *param);
ZENGINE_API void log_uninit();

ZENGINE_API int log_print(const char fmt[], ...);
ZENGINE_API int logI_print(const char fmt[], ...);	/* write immediately */
ZENGINE_API int log_setpriority(int pri);

#ifdef __cplusplus
}
#endif

#endif	//_ZENGINE_LOG_H