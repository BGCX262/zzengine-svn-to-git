#ifndef _TINY_LOG_MEMORY_H
#define _TINY_LOG_MEMORY_H

#include "ntype.h"

typedef struct TL_MEMORY *memS_t;

memS_t memS_create();
void memS_destroy(memS_t t);
void* memS_alloc(memS_t t, int nSize);

#endif	//_TINY_LOG_MEMORY_H