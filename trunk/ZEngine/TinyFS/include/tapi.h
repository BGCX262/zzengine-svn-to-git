/*
 **	@file	tapi.h
 **	@brief	
 **	@detail	
 **	@author	AnIcepIg
 **	@date	[2010/12/31 14:25:49]
 */
#ifndef _TFS_TAPI_H
#define _TFS_TAPI_H

#include "tobject.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tfs_Header *tfs_t;
typedef struct tfs_TTable *tfs_Table;
typedef TValue tfs_Value;
typedef struct HeaderEx *tfsE_t;

/* ���º�����tfs�ļ����ж����� */
TFS_API tfs_t tfs_Load(c_str file);
TFS_API void tfs_Unload(tfs_t pfs);
TFS_API tfs_Table tfs_GetTable(tfs_t pfs, c_str name);				/* ����nameΪ���Ƿ����ܱ� */
TFS_API TValue tfs_GetValue(tfs_t pfs, tfs_Table tab, c_str name);	/* ����tabΪ�������ܱ��в�ѯ */

TFS_API tfs_Table tfs_totable(tfs_t pfs, tfs_Value v);
TFS_API c_str tfs_tostring(tfs_t pfs, tfs_Value v);
TFS_API TValue8 tfs_tovalue8(tfs_t pfs, tfs_Value v);
TFS_API TValue32 tfs_tovalue32(tfs_t pfs, tfs_Value v);
TFS_API TValue64 tfs_tovalue64(tfs_t pfs, tfs_Value v);

#define tfs_tobool(p, v)	(tfs_tovalue8((p), (v))).bl
#define tfs_tobyte(p, v)	(tfs_tovalue8((p), (v))).bt
#define tfs_toint32(p, v)	(tfs_tovalue32((p), (v))).n
#define tfs_tofloat(p, v)	(tfs_tovalue32((p), (v))).f
#define tfs_todouble(p, v)	(tfs_tovalue64((p), (v))).d
#define tfs_toint64(p, v)	(tfs_tovalue64((p), (v))).i64

#define tfs_isstring(v)	isstring(v)
#define tfs_istable(v)	istable(v)
#define tfs_isbool(v)	isbool(v)
#define tfs_isbyte(v)	isbyte(v)
#define tfs_isint32(v)	isint32(v)
#define tfs_isfloat(v)	isfloat(v)
#define tfs_isdouble(v)	isdouble(v)
#define tfs_isint64(v)	isint64(v)

/*	
	Ҫ��tfs�ļ�����д��������ʹ�����º�����
	ע��ֻ����tfs�ṹ��������д��������Ҫ������ʹ��ת��������
*/
TFS_API tfsE_t tfsE_Create(c_str name);
TFS_API tfsE_t tfsE_From(tfs_t tfs);			/* ��ֻ����tfsת��Ϊ��д�ģ�ת����ԭtfs�Կ��� */
TFS_API tfs_Table tfsE_GetTable(tfsE_t pfse, c_str name);
TFS_API tfs_t tfsE_GetTfs(tfsE_t pfse);
TFS_API tfsE_t tfsE_Load(c_str file);
TFS_API c_str tfsE_Save(c_str file, tfsE_t pfse);
TFS_API void tfsE_Destroy(tfsE_t pfse);

tfs_Value tfsE_SetValue8(tfsE_t pfse, tfs_Table tab, c_str name, TValue8 v8, SEG_TYPE st);
tfs_Value tfsE_SetValue32(tfsE_t pfse, tfs_Table tab, c_str name, TValue32 v32, SEG_TYPE st);
tfs_Value tfsE_SetValue64(tfsE_t pfse, tfs_Table tab, c_str name, TValue64 v64, SEG_TYPE st);

TFS_API tfs_Value tfsE_SetBool(tfsE_t pfse, tfs_Table tab, c_str name, bool b);
TFS_API tfs_Value tfsE_SetByte(tfsE_t pfse, tfs_Table tab, c_str name, byte b);
TFS_API tfs_Value tfsE_SetInt32(tfsE_t pfse, tfs_Table tab, c_str name, int n);
TFS_API tfs_Value tfsE_SetFloat(tfsE_t pfse, tfs_Table tab, c_str name, float f);
TFS_API tfs_Value tfsE_SetString(tfsE_t pfse, tfs_Table tab, c_str name, c_str s);
TFS_API tfs_Value tfsE_SetDouble(tfsE_t pfse, tfs_Table tab, c_str name, double d);
TFS_API tfs_Value tfsE_SetInt64(tfsE_t pfse, tfs_Table tab, c_str name, int64 i64);
TFS_API tfs_Value tfsE_SetTable(tfsE_t pfse, tfs_Table tab, c_str name);

/* 
 ��name���ó�null�൱��ɾ��name��Ӧ��ֵ 
 name�����ڱ��з���1�������ڷ���0
 */
TFS_API int tfsE_SetNull(tfsE_t pfse, tfs_Table tab, c_str name);

TFS_API c_str tfsE_GetTableName(tfs_Table tab);
TFS_API uint tfsE_GetTableSize(tfs_Table tab);
TFS_API c_str tfsE_GetKeyAt(tfs_Table tab, uint index);
TFS_API tfs_Value tfsE_GetValueAt(tfs_Table tab, uint index);

#ifdef __cplusplus
}
#endif

#endif