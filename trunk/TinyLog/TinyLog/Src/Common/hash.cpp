#include "stdafx.h"
#include "hash.h"

uint32 hash_string(c_str cstr, uint32 uLen)
{
	unsigned int uValue = 0;
	unsigned int nSeed = 4;
	if (cstr)
	{
		while (uLen --)
		{
			uValue ^= ((uValue & 0x3F) + nSeed) * (*cstr++) + (uValue << 8);
			nSeed += 3;
		}
	}
	return uValue;
}