#ifndef _TINY_LOG_ATOM_H
#define _TINY_LOG_ATOM_H

#include "ntype.h"

typedef struct TL_ATOM *atom_t;

atom_t atom_create();
void atom_destroy(atom_t t);
c_str atom_new(atom_t t, c_str cstr, uint32 uLen);
c_str atom_string(atom_t t, c_str cstr);

#endif	//_TINY_LOG_ATOM_H