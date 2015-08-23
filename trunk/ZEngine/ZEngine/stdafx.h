/*
 *	Precompile Header File for Engine
 *	2010-11-14 By AnIcepIg
 */
#ifndef _ZENGINE_PRECOMPILE_H
#define _ZENGINE_PRECOMPILE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef WIN32
	#define _WIN32_WINNT 0x0500
	#include <windows.h>
#endif

#ifndef ASSERT
	#define ASSERT(x)	assert(x)
#endif

#include "stddef.h"

#endif	//_ZENGINE_PRECOMPILE_H