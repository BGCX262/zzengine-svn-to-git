/*
 ** @file	TinyFS.h
 ** @brief	
 **	@detail	
 **	@author	AnIcepIg
 ** @date	[2010/12/29 15:49:22]
 */
#ifndef _TINYFS_H
#define _TINYFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FSBase.h"

typedef struct TFS {
	fcc signature;		/* �ļ�ǩ��'tfs0' */
	ulong reserve0;
	ulong reserve1;
	ulong major;		/* ���汾�� */
	ulong minor;		/* �ΰ汾�� */
	lint timestamp;		/* ����޶�ʱ�� */
	ulong size;
	ulong chunk;		/* ��һ��chunk��ƫ�� */
	char name[64];		/* tfs�ļ����� */
}TFS, *tfs_t;

typedef struct TFSEX *tfsEx_t;
typedef struct TFSCHUNK *tfsC_t;

TFS_API void tfsEx_Destroy(tfsEx_t t);

TFS_API tfsC_t tfsEx_CreateChunk(c_str name);
TFS_API void tfsEx_DestroyChunk(tfsC_t pfsc);
TFS_API int tfsEx_AddChunk(tfsEx_t pfsex, tfsC_t pfsc);

TFS_API TValue* tfsEx_SetValue(tfsEx_t pfsex, tfsC_t pfsc, c_str key, TValue v);
TFS_API TValue* tfsEx_GetValue(tfsEx_t pfsex, tfsC_t pfsc, c_str key);

#ifdef __cplusplus
}
#endif

#endif