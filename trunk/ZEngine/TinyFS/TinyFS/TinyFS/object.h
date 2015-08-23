/*
 ** @file	object.h
 ** @brief	
 **	@detail	
 **	@author	AnIcepIg
 ** @date	[2010/12/30 11:36:48]
 */
#ifndef _TINYFS_OBJECT_H
#define _TINYFS_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef union tfs_Object {
	int n;
	float f;
	double d;
	byte b;
	c_str s;
	void* pv;
}TObject;

typedef struct tfs_TValue {
	int type;
	TObject obj;
}TValue;

#define TFS_NULL		0
#define TFS_INT32		1
#define TFS_FLOAT		2
#define TFS_DOUBLE		3
#define TFS_BYTE		4
#define TFS_STRING		5
#define TFS_USERDATA	6

#define typeof(o)		((o)->type)
#define isnil(o)		(typeof(o) == TFS_NULL)
#define isint32(o)		(typeof(o) == TFS_INT32)
#define isfloat(o)		(typeof(o) == TFS_FLOAT)
#define isdouble(o)		(typeof(o) == TFS_DOUBLE)
#define isbyte(o)		(typeof(o) == TFS_BYTE)
#define isstring(o)		(typeof(o) == TFS_STRING)
#define isuserdata(o)	(typeof(o) == TFS_USERDATA)

#ifdef __cplusplus
}
#endif

#endif