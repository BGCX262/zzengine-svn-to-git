/*
 *	thread
 *	2010-11-18 by AnIcepIg
 */
#ifndef _ZENGINE_THREAD_H
#define _ZENGINE_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void thread_func(void *pv);

#ifdef WIN32
	#include "process.h"
	
	#define	thread_t HANDLE
#else
	#include <unistd.h>
	#include <pthread.h>
	
	#define thread_t pthread_t
#endif

ZENGINE_API thread_t thread_create(thread_func *fn, void *pv);
ZENGINE_API void thread_destroy(thread_t t);

#ifdef __cplusplus
}
#endif

#endif	//_ZENGINE_THREAD_H