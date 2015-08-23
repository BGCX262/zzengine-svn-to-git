/*
 ** @file	tobject.h
 ** @brief	
 **	@detail	
 **	@author	AnIcepIg
 ** @date	[2010/12/31 9:21:35]
 */
#ifndef _TFS_TOBJECT_H
#define _TFS_TOBJECT_H

#include "FSBase.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;
typedef unsigned int uint;

typedef union tfs_Bit32 {
	int n;
	float f;
}TValue32;

typedef union tfs_Bit8 {
	byte bt;
	bool bl;
}TValue8;

typedef union tfs_Bit64 {
	double d;
	int64 i64;
}TValue64;

typedef void* TValueTable;
typedef c_str TValueString;

typedef struct tfs_Key {
	c_str s;
	int h;
}Key;

typedef int TValue;

#define VALUE_INDEX		0x00ffffff	/* ȡ��value�ڸ����е����� */
#define VALUE_SEGMENT	0xff000000	/* value�Ķ����� */

#define value_getindex(v)		((v) & VALUE_INDEX)
#define value_getsegment(v)		(((v) & VALUE_SEGMENT) >> 24)
#define make_value(s, i)		(((s) << 24) | ((i) & VALUE_INDEX))

typedef enum {
	SEG_NONE = 0,

	SEG_VALUE8_BOOL,
	SEG_VALUE8_BYTE,
	SEG_VALUE8_RESERVE0,
	SEG_VALUE8_RESERVE1,

	SEG_VALUE16_RESERVE0,
	SEG_VALUE16_RESERVE1,

	SEG_VALUE32_INT,
	SEG_VALUE32_FLOAT,
	SEG_VALUE32_RESERVE0,
	SEG_VALUE32_RESERVE1,

	SEG_VALUE64_INT,
	SEG_VALUE64_DOUBLE,
	SEG_VALUE64_RESERVE0,
	SEG_VALUE64_RESERVE1,

	SEG_KEY,
	SEG_TABLE,
	SEG_STRING,
	SEG_DATA,
	SEG_TOTAL
}SEG_TYPE;

#define isnull(o)		(value_getsegment(o) == SEG_NONE)
#define isbool(o)		(value_getsegment(o) == SEG_VALUE8_BOOL)
#define isbyte(o)		(value_getsegment(o) == SEG_VALUE8_BYTE)
#define isint32(o)		(value_getsegment(o) == SEG_VALUE32_INT)
#define isfloat(o)		(value_getsegment(o) == SEG_VALUE32_FLOAT)
#define isint64(o)		(value_getsegment(o) == SEG_VALUE64_INT)
#define isdouble(o)		(value_getsegment(o) == SEG_VALUE64_DOUBLE)
#define istable(o)		(value_getsegment(o) == SEG_TABLE)
#define isstring(o)		(value_getsegment(o) == SEG_STRING)

typedef struct tfs_Node {
	TValue v;
	Key k;
}Node;

typedef struct tfs_TTable {
	Key name;
	ulong count;
	ulong capacity;
	Node* pn;
}Table;

/* tfs�ļ����� */
typedef struct tfs_Header {
	fcc signature;			/* �ļ�ǩ�� */
	ulong reserve0;
	ulong reserve1;
	ulong major;			/* ���汾�� */
	ulong minor;			/* �ΰ汾�� */
	lint timestamp;			/* ���һ���޸�ʱ��� */
	ulong size;
	char name[64];
	Table* pt;				/* �ܱ� */
	ulong v8c;
	ulong v8ca;
	TValue8* pv8;			/* 8λ���ݶ� */
	ulong v32c;
	ulong v32ca;
	TValue32* pv32;			/* 32λ���ݶ� */
	ulong v64c;
	ulong v64ca;
	TValue64* pv64;			/* 64λ���ݶ� */
	ulong tablec;
	ulong tableca;
	TValueTable* ptable;	/* Table�� */
	ulong stringc;
	ulong stringca;
	TValueString* pstring;	/* String�� */
}Header;

#ifdef __cplusplus
}
#endif

#endif