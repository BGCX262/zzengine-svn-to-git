/*
 *	Memory Management for Engine
 *	2010-11-15 By AnIcepIg
 */
#ifndef _ZENGINE_MEMORY_H
#define _ZENGINE_MEMORY_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Implementation for Singleton */
typedef struct ST *memS_t;

ZENGINE_API memS_t memS_create();
ZENGINE_API void memS_destroy(memS_t t);
ZENGINE_API void* memS_alloc(memS_t t, int size);

#ifdef __cplusplus
}
#endif

#endif	//_ZENGINE_MEMORY_H