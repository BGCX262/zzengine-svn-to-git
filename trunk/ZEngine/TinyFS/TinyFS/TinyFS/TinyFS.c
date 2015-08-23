#include "stdafx.h"
#include "object.h"
#include "TinyFS.h"
#include "atom.h"
#include "hash.h"

typedef struct TFSCHUNK {
	struct TFSCHUNK* next;
	c_str key;
	int hashcode;
	TValue v;
}TFSCHUNK;

typedef struct TFSEX {
	tfs_t pfs;
	atom_t pat;
	tfsC_t pfc;
}TFSEX;

static tfs_t newheader(c_str name) {
	tfs_t pt;
	pt = (tfs_t)malloc(sizeof(TFS));
	assert(pt);
	memset(pt, 0, sizeof(TFS));
	pt->signature = TFS_SIGNATURE;
	strcpy(pt->name, name);
	return pt;
}

static void deleteheader(tfs_t p) {
	if (p) {
		free(p);
		p = null;
	}
}

static tfsC_t newchunk() {
	tfsC_t pt;
	pt = (tfsC_t)malloc(sizeof(TFSCHUNK));
	assert(pt);
	memset(pt, 0, sizeof(TFSCHUNK));
	return pt;
}

static void deletechunk(tfsC_t p) {
	if (p) {
		free(p);
		p = null;
	}
}

TFS_API tfs_t tfs_Load(c_str file) {
	tfs_t t;
	FILE* pf;
	assert(file);

	return t;
}

TFS_API tfsEx_t tfsEx_Create(c_str name) {
	tfsEx_t pfsex;
	assert(name);
	pfsex = (tfsEx_t)malloc(sizeof(TFSEX));
	assert(pfsex);
	pfsex->pfs = newheader(name);
	assert(pfsex->pfs);
	pfsex->pat = atom_create();
	assert(pfsex->pat);
	pfsex->pfc = newchunk();
	assert(pfsex->pfc);
	return pfsex;
}

TFS_API TValue* tfsEx_SetValue(tfsEx_t pfsex, tfsC_t pfsc, c_str key, TValue v) {
	tfsC_t next, prev;
	c_str k;
	int hc;
	assert(pfsex);
	assert(pfsc);
	k = atom_string(pfsex->pat, key);
	hc = hash_string(key);
	for (next = pfsc, prev = pfsc; next; next = next->next) {
		if (hc == next->hashcode) {
			
		}
		prev = next;
	}
}

