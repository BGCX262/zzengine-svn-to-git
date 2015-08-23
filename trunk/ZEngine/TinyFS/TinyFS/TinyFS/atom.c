#include "stdafx.h"
#include "atom.h"
#include "hash.h"
#include "mem.h"

#define BUCKET_COUNT	2048

typedef struct atom {
	struct atom* link;
	int len;
	char* str;
}atom;

typedef struct T {
	int cb;
	memS_t pm;
	struct atom* buckets[BUCKET_COUNT];
}T;

ZENGINE_API atom_t atom_create() {
	atom_t a = (atom_t)malloc(sizeof(T));
	assert(a);
	a->pm = memS_create();
	assert(a->pm);
	a->cb = sizeof(T);
	memset(a->buckets, 0, sizeof(char*) * BUCKET_COUNT);
	return a;
}

ZENGINE_API void atom_destroy(atom_t t) {
	assert(t);
	assert(t->cb == sizeof(T));
	memS_destroy(t->pm);
	free(t);
}

ZENGINE_API c_str atom_new(atom_t t, c_str str, int len) {
	int i, pos;
	struct atom* pa;
	assert(t);
	assert(t->cb == sizeof(T));
	assert(str);
	assert(len > 0);
	pos = hash_string(str);
	pos &= NELEMS(t->buckets) - 1;
	for (pa = t->buckets[pos]; pa; pa = pa->link) {
		if (len == pa->len) {
			for (i = 0; i < len && pa->str[i] == str[i]; ++i);
			if (i == len)
				return pa->str;
		}
	}
	/* string is not existed in buckets */
	pa = (struct atom*)memS_alloc(t->pm, sizeof(atom) + len + 1);
	assert(pa);
	pa->len = len;
	pa->str = (char*)(pa + 1);
	strncpy(pa->str, str, len + 1);
	pa->link = t->buckets[pos];
	t->buckets[pos] = pa;
	return pa->str;
}

ZENGINE_API c_str atom_string(atom_t t, c_str str) {
	assert(t);
	assert(t->cb == sizeof(T));
	assert(str);
	return atom_new(t, str, (int)strlen(str));
}

ZENGINE_API c_str atom_int(atom_t t, int n) {
	char nstr[64];
	assert(t);
	assert(t->cb == sizeof(T));
	_itoa(n, nstr, 10);
	return atom_new(t, nstr, (int)strlen(nstr));
}

static atom_t sg_single = null;

ZENGINE_API int atomG_init() {
	assert(sg_single == null);
	sg_single = atom_create();
	return 1;
}

ZENGINE_API void atomG_uninit() {
	assert(sg_single);
	atom_destroy(sg_single);
	sg_single = null;
}

ZENGINE_API atom_t atomG_get() {
	return sg_single;
}