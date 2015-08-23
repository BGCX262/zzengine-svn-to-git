#include "stdafx.h"
#include "ntype.h"
#include "memory.h"

struct TL_MEMORY_UNIT
{
	TL_MEMORY_UNIT* pNext;
	uint32 uCapacity;
	uint32 uOffset;
	char* pBuffer;
};

struct TL_MEMORY
{
	uint32 cbSize;
	TL_MEMORY_UNIT* pPool;
};

#define CAPACITY_SIZE	128

memS_t memS_create()
{
	TL_MEMORY* ptlMem = (TL_MEMORY*)malloc(sizeof(TL_MEMORY));
	assert(ptlMem);

	ptlMem->cbSize = sizeof(TL_MEMORY);
	ptlMem->pPool = (TL_MEMORY_UNIT*)malloc(sizeof(TL_MEMORY_UNIT) + CAPACITY_SIZE);
	assert(ptlMem->pPool);

	TL_MEMORY_UNIT* pPool = ptlMem->pPool;
	pPool->pNext = NULL;
	pPool->uCapacity = CAPACITY_SIZE;
	pPool->uOffset = 0;
	pPool->pBuffer = (char*)(ptlMem->pPool + 1);

	return ptlMem;
}

void memS_destroy(memS_t t)
{
	assert(t);
	assert(t->cbSize == sizeof(TL_MEMORY));

	TL_MEMORY_UNIT* ptlMem = t->pPool;
	TL_MEMORY_UNIT* ptlTmp = NULL;

	while (ptlMem)
	{
		ptlTmp = ptlMem->pNext;
		free(ptlMem);
		ptlMem = ptlTmp;
	}
	free(t);
	t = NULL;
}

void* memS_alloc(memS_t t, int nSize)
{
	assert(t);
	assert(t->cbSize == sizeof(TL_MEMORY));
	assert(nSize > 0);

	TL_MEMORY_UNIT* ptlMem = t->pPool;
	uint32 uSize = (uint32)nSize;
	char* pBuffer = NULL;

	if (uSize <= ptlMem->uCapacity - ptlMem->uOffset)
	{
		pBuffer = ptlMem->pBuffer + ptlMem->uOffset;
		ptlMem->uOffset += uSize;
		return pBuffer;
	}
	else
	{
		uint32 uNewSize = (ptlMem->uCapacity + uSize) << 1;
		TL_MEMORY_UNIT* ptlNew = (TL_MEMORY_UNIT*)malloc(sizeof(TL_MEMORY_UNIT) + uNewSize);
		assert(ptlNew);

		ptlNew->uCapacity = uNewSize;
		ptlNew->uOffset = 0;
		ptlNew->pBuffer = (char*)(ptlNew + 1);
		ptlNew->pNext = ptlMem;
		t->pPool = ptlNew;
		return memS_alloc(t, nSize);
	}
}