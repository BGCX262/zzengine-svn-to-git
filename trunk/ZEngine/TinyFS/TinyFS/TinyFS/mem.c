#include "stdafx.h"
#include "mem.h"

typedef struct memS_atom {
	struct memS_atom* link;
	int capacity;
	int offset;
	byte* p;
}memS_atom;

typedef struct ST {
	int cb;
	struct memS_atom* pl;
}ST;

#define INIT_CAPACITY_S		128

ZENGINE_API memS_t memS_create() {
	memS_t p = (memS_t)malloc(sizeof(ST));
	assert(p);
	p->cb = sizeof(ST);
	p->pl = (memS_atom*)malloc(sizeof(memS_atom) + INIT_CAPACITY_S);
	assert(p->pl);
	p->pl->link = null;
	p->pl->capacity = INIT_CAPACITY_S;
	p->pl->offset = 0;
	p->pl->p = (byte*)(p->pl + 1);
	return p;
}

ZENGINE_API void memS_destroy(memS_t t) {
	struct memS_atom* pa;
	struct memS_atom* pt;
	assert(t);
	assert(t->cb == sizeof(ST));
	pa = t->pl;
	while (pa) {
		pt = pa->link;
		free(pa);
		pa = pt;
	}
	free(t);
	t = null;
}

ZENGINE_API void* memS_alloc(memS_t t, int size) {
	struct memS_atom* pa;
	struct memS_atom* pn;
	byte* pbr;
	int newc;
	assert(t);
	assert(t->cb == sizeof(ST));
	assert(size > 0);
	pa = t->pl;
	if (size <= pa->capacity - pa->offset) {
		pbr = pa->p + pa->offset;
		pa->offset += size;
		return pbr;
	}
	else {
		newc = (pa->capacity + size) * 2;
		pn = (struct memS_atom*)malloc(newc + sizeof(memS_atom));
		assert(pn);
		pn->capacity = newc;
		pn->offset = 0;
		pn->p = (byte*)(pn + 1);
		pn->link = pa;
		t->pl = pn;
		return memS_alloc(t, size);
	}
}