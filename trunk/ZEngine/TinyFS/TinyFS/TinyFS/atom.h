/*
 *	Const String for Engine Common Support
 *	2010-11-14 by AnIcepIg
 */
#ifndef _ZENGINE_ATOM_H
#define _ZENGINE_ATOM_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct T *atom_t;

ZENGINE_API atom_t atom_create();
ZENGINE_API void atom_destroy(atom_t t);
ZENGINE_API c_str atom_new(atom_t t, c_str str, int len);
ZENGINE_API c_str atom_string(atom_t t, c_str str);
ZENGINE_API c_str atom_int(atom_t t, int n);

/* Global atom instance, atomG_init() for start and atomG_uninit() for end */
ZENGINE_API int atomG_init();
ZENGINE_API void atomG_uninit();
ZENGINE_API atom_t atomG_get();
#define atomG_new(s, l)	atom_new(atomG_get(), (s), (l))
#define atomG_string(s)	atom_string(atomG_get(), (s))
#define atomG_int(n)	atom_int(atomG_get(), (n))

#ifdef __cplusplus
}
#endif

#endif	//_ZENGINE_ATOM_H