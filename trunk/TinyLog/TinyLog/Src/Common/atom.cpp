#include "stdafx.h"
#include "atom.h"
#include "hash.h"
#include "memory.h"

#define BUCKET_COUNT	2048

struct TL_ATOM_UNIT
{
	TL_ATOM_UNIT* pNext;
	uint32 uLen;
	char* pStr;
};

struct TL_ATOM
{
	uint32 cbSize;

	memS_t memST;
	TL_ATOM_UNIT* pBuckets[BUCKET_COUNT];
};

atom_t atom_create()
{
	TL_ATOM* pAtom = (TL_ATOM*)malloc(sizeof(TL_ATOM));
	assert(pAtom);

	pAtom->cbSize = sizeof(TL_ATOM);
	pAtom->memST = memS_create();
	assert(pAtom->memST);
	memset(pAtom->pBuckets, 0, sizeof(TL_ATOM_UNIT*) * BUCKET_COUNT);

	return pAtom;
}

void atom_destroy(atom_t t)
{
	assert(t);
	assert(t->cbSize == sizeof(TL_ATOM));
	memS_destroy(t->memST);
	free(t);
}

c_str atom_new(atom_t t, c_str cstr, uint32 uLen)
{
	assert(t);
	assert(t->cbSize == sizeof(TL_ATOM));
	assert(cstr);

	uint32 uPos = hash_string(cstr, uLen);
	uPos &= NELEMS(t->pBuckets) - 1;

	TL_ATOM_UNIT* pAtomUnit = NULL;
	uint32 uIndex = 0;
	for (pAtomUnit = t->pBuckets[uPos]; pAtomUnit; pAtomUnit = pAtomUnit->pNext)
	{
		if (uLen == pAtomUnit->uLen)
		{
			uIndex = 0;
			while (uIndex <= uLen && pAtomUnit->pStr[uIndex] == cstr[uIndex])
				++uIndex;
			if (uIndex == uLen + 1)
				return pAtomUnit->pStr;
		}
	}

	pAtomUnit = (TL_ATOM_UNIT*)memS_alloc(t->memST, sizeof(TL_ATOM_UNIT) + uLen + 1);
	assert(pAtomUnit);
	pAtomUnit->uLen = uLen;
	pAtomUnit->pStr = (char*)(pAtomUnit + 1);
	pAtomUnit->pNext = t->pBuckets[uPos];
	memcpy(pAtomUnit->pStr, cstr, uLen + 1);
	t->pBuckets[uPos] = pAtomUnit;
	return pAtomUnit->pStr;
}

c_str atom_string(atom_t t, c_str cstr)
{
	assert(t);
	assert(t->cbSize == sizeof(TL_ATOM));
	assert(cstr);
	return atom_new(t, cstr, strlen(cstr));
}

