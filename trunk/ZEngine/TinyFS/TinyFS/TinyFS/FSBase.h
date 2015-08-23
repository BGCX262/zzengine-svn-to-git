/*
 ** @file	FSBase.h
 ** @brief	Common/Base Define For Tiny File System
 **	@detail	
 **	@author	AnIcepIg
 ** @date	[2010/12/28]
 */
#ifndef _TINYFS_BASE_H
#define _TINYFS_BASE_H

typedef const char* c_str;
typedef LONG32	fcc;
typedef unsigned long ulong;
typedef LARGE_INTEGER lint;
typedef INT64 int64;

#define TFS_SIGNATURE	'tfs0'

#ifdef BUILD_AS_DLL
#define TFS_API	__declspec(dllexport)
#else
#define TFS_API	extern
#endif

#endif	//_TINYFS_BASE_H