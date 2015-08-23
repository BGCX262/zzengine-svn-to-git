#ifndef _TINY_LOG_COMMON_H
#define _TINY_LOG_COMMON_H

#include "ntype.h"

#define null	0

inline uint32 RandNumber()
{
	static uint32 s_uNumber = (uint32)time(NULL);
	s_uNumber = s_uNumber * 214013L + 2531011L;
	return s_uNumber;
}

#if (defined(_MSC_VER) || defined(__ICL))
#define mkdir _mkdir
#else
inline int32 mkdir(c_str cszDir[])
{
	return mkdir(cszDir, 0777);
}
#endif

inline void ToLower(pstr psz)
{
	for (pstr pc = psz; *pc; ++pc)
	{
		if ((*pc) >= 'A' && (*pc) <= 'Z')
			*pc = *pc + ('a' - 'A');
	}
}

inline bool IsFileExist(const char* pcszFileName)
{
#ifdef WIN32
	return !(GetFileAttributesA(pcszFileName) & FILE_ATTRIBUTE_DIRECTORY);
#else
	int nFile = _sopen(pcszFileName, _O_BINARY, _SH_DENYNO, 0);
	if (nFile != -1)
	{
		_close(nFile);
		return true;
	}
	return false;
#endif
}

inline size_t GetFileSize(FILE* pFile)
{
	size_t uLen = 0;
	fseek(pFile, 0, SEEK_END);
	uLen = ftell(pFile);
	rewind(pFile);
	return uLen;
}

#ifndef PATH_COUNT
#define PATH_COUNT	256
#endif

#include <assert.h>

#ifndef ASSERT
#define ASSERT assert
#endif

#endif	//_TINY_LOG_COMMON_H