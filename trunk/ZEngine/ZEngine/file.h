/*
 *	File
 *	2010-11-18 by AnIcepIg
 */
#ifndef _ZENGINE_FILE_H
#define _ZENGINE_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FT *file_t;

ZENGINE_API bool file_exist(c_str path);

#ifdef __cplusplus
}
#endif

#endif	//_ZENGINE_FILE_H