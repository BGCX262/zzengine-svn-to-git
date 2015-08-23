#include "stdafx.h"
#include "tobject.h"
#include "tapi.h"
#include "atom.h"
#include "hash.h"
#include "io.h"
#include "time.h"

typedef struct HeaderEx {
	Header* ph;
	atom_t at;
}HeaderEx;

#define version_major	1
#define version_minor	0

typedef struct Buffer {
	ulong cb;
	void* buff;
	ulong count;
	ulong capacity;
}Buffer;

#define safe_free(p)	\
	if (p)				\
		free(p);		\
	p = null;

static ulong addbuffer(Buffer* pb, void* pv, ulong size) {
	ulong ab, ret;
	void* pn;
	assert(pb);
	assert(pv);
	assert(pb->cb == sizeof(Buffer));
	ab = pb->capacity - pb->count;
	if (ab < size) {
		pn = malloc((pb->capacity + size) << 1);
		assert(pn);
		if (pb->buff && pb->count) {
			memcpy(pn, pb->buff, pb->count);
			free(pb->buff);
		}
		pb->buff = pn;
		pb->capacity = (pb->capacity + size) << 1;
	}
	ret = pb->count;
	memcpy(((byte*)pb->buff) + pb->count, pv, size);
	pb->count += size;
	return ret;
}

static void bclear(Buffer* pb) {
	if (pb && pb->cb == sizeof(Buffer) && pb->buff) {
		free(pb->buff);
		memset(pb, 0, sizeof(Buffer));
	}
}

typedef struct Spair {
	c_str lhs;
	ulong rhs;
}Spair;

typedef struct Strmap {
	ulong cb;
	ulong count;
	ulong capacity;
	Spair* arr;
}Strmap;

static int sfind(Strmap* sm, c_str s) {
	ulong i;
	for (i = 0; i < sm->count; ++i)
		if (sm->arr[i].lhs == s)
			return sm->arr[i].rhs;
	return -1;
}

static void sinsert(Strmap* sm, c_str lhs, ulong rhs) {
	void* pn;
	if (sm->count == sm->capacity) {
		sm->capacity = sm->count + sm->capacity + 1;
		pn = malloc(sm->capacity * sizeof(Spair));
		assert(pn);
		if (sm->count && sm->arr) {
			memcpy(pn, sm->arr, sizeof(Spair) * sm->count);
			free(sm->arr);
		}
		sm->arr = pn;
	}
	sm->arr[sm->count].lhs = lhs;
	sm->arr[sm->count++].rhs = rhs;
}

static void sclear(Strmap* sm) {
	if (sm->arr)
		free(sm->arr);
	memset(sm, 0, sizeof(Strmap));
}

static Header* newheader(c_str name) {
	Header* ph;
	ph = (Header*)malloc(sizeof(Header));
	assert(ph);
	memset(ph, 0, sizeof(Header));
	ph->signature = TFS_SIGNATURE;
	ph->size = sizeof(Header);
	strcpy(ph->name, name);
	return ph;
}

static void deleteheader(Header* ph) {
	if (ph) {
		free(ph);
		ph = null;
	}
}

static Table* newtable(c_str name) {
	Table* pt;
	pt = (Table*)malloc(sizeof(Table));
	assert(pt);
	memset(pt, 0, sizeof(Table));
	if (name) {
		pt->name.s = name;
		pt->name.h = hash_string(name);
	}
	return pt;
}

static void deletetable(Table* pt) {
	if (pt) {
		free(pt);
		pt = null;
	}
}

static void* resizearr(void* arr, ulong s, ulong o, ulong n) {
	void* pn;
	o = s * o;
	n = s * n;
	if (arr == null) {
		pn = malloc(n);
		return pn;
	}
	if (n <= o)
		return arr;
	pn = malloc(n);
	assert(pn);
	memset(pn, 0, n);
	memcpy(pn, arr, o);
	free(arr);
	arr = null;
	return pn;
}

static Node* tfind(Table* t, c_str name) {
	ulong i;
	for (i = 0; i < t->count; ++i) {
		if (t->pn[i].k.s == name)
			return &t->pn[i];
	}
	return null;
}

static Node* tinsert(Table* t, c_str name) {
	Node* pn;
	if (t->count >= t->capacity) {
		t->pn = resizearr(t->pn, sizeof(Node), t->capacity, ((t->capacity << 1) + 1));
		t->capacity = (t->capacity << 1) + 1;
	}
	pn = &t->pn[t->count];
	pn->k.s = name;
	pn->k.h = hash_string(name);
	t->count++;
	return pn;
}

static TValue v8tov(Header* ph, TValue8 v8, SEG_TYPE st) {
	TValue v;
	if (ph->v8c >= ph->v8ca) {
		ph->pv8 = resizearr(ph->pv8, 1, ph->v8ca, (ph->v8ca << 1) + 1);
		ph->v8ca = (ph->v8ca << 1) + 1;
	}
	v = make_value(st, ph->v8c);
	ph->pv8[ph->v8c] = v8;
	ph->v8c++;
	return v;
}

static TValue8 getv8(Header* ph, TValue v, SEG_TYPE st) {
	ulong i;
	TValue8 v8;
	v8.bt = 0;
	if (value_getsegment(v) != st)
		return v8;
	i = value_getindex(v);
	if (i < ph->v8c)
		return ph->pv8[i];
	return v8;
}

static TValue v32tov(Header* ph, TValue32 v32, SEG_TYPE st) {
	TValue v;
	if (ph->v32c >= ph->v32ca) {
		ph->pv32 = resizearr(ph->pv32, sizeof(TValue32), ph->v32ca, (ph->v32ca << 1) + 1);
		ph->v32ca = (ph->v32ca << 1) + 1;
	}
	v = make_value(st, ph->v32c);
	ph->pv32[ph->v32c] = v32;
	ph->v32c++;
	return v;
}

static TValue32 getv32(Header* ph, TValue v, SEG_TYPE st) {
	ulong i;
	TValue32 v32;
	v32.n = 0;
	if (value_getsegment(v) != st)
		return v32;
	i = value_getindex(v);
	if (i < ph->v32c)
		return ph->pv32[i];
	return v32;
}

static TValue v64tov(Header* ph, TValue64 v64, SEG_TYPE st) {
	TValue v;
	if (ph->v64c >= ph->v64ca) {
		ph->pv64 = resizearr(ph->pv64, sizeof(TValue64), ph->v64ca, (ph->v64ca << 1) + 1);
		ph->v64ca = (ph->v64ca << 1) + 1;
	}
	v = make_value(st, ph->v64c);
	ph->pv64[ph->v64c] = v64;
	ph->v64c++;
	return v;
}

static TValue vtabletov(Header* ph, TValueTable vtable) {
	TValue v;
	if (ph->tablec >= ph->tableca) {
		ph->ptable = resizearr(ph->ptable, sizeof(TValueTable), ph->tableca, (ph->tableca << 1) + 1);
		ph->tableca = (ph->tableca << 1) + 1;
	}
	v = make_value(SEG_TABLE, ph->tablec);
	ph->ptable[ph->tablec] = vtable;
	ph->tablec++;
	return v;
}

static TValue vstringtov(Header* ph, TValueString vstring) {
	TValue v;
	if (ph->stringc >= ph->stringca) {
		ph->pstring = resizearr(ph->pstring, sizeof(TValueString), ph->stringca, (ph->stringca << 1) + 1);
		ph->stringca = (ph->stringca << 1) + 1;
	}
	v = make_value(SEG_STRING, ph->stringc);
	ph->pstring[ph->stringc] = vstring;
	ph->stringc++;
	return v;
}

static TValue64 getv64(Header* ph, TValue v, SEG_TYPE st) {
	ulong i;
	TValue64 v64;
	v64.d = 0.0;
	if (value_getsegment(v) != st)
		return v64;
	i = value_getindex(v);
	if (i < ph->v64c)
		return ph->pv64[i];
	return v64;
}

static int serialbuffer(Header* ph) {
	byte* pbh;
	ulong i, j;
	Table* pt;
	Node* pn;
	pbh = (byte*)ph;
	ph->pt = pbh + (int)ph->pt;
	pt = ph->pt;
	if (pt->count)
		pt->pn = pbh + (int)pt->pn;
	for (i = 0; i < pt->count; ++i) {
		pn = &pt->pn[i];
		pn->k.s = pbh + (int)pn->k.s;
	}
	if (ph->v8c)
		ph->pv8 = pbh + (int)ph->pv8;
	if (ph->v32c)
		ph->pv32 = pbh + (int)ph->pv32;
	if (ph->v64c)
		ph->pv64 = pbh + (int)ph->pv64;
	if (ph->stringc)
		ph->pstring = pbh + (int)ph->pstring;
	for (i = 0; i < ph->stringc; ++i)
		ph->pstring[i] = pbh + (int)ph->pstring[i];
	if (ph->tablec)
		ph->ptable = pbh +  (int)ph->ptable;
	for (i = 0; i < ph->tablec; ++i) {
		ph->ptable[i] = pbh + (int)ph->ptable[i];
		pt = ph->ptable[i];
		if (pt->name.s)
			pt->name.s = pbh + (int)pt->name.s;
		if (pt->count)
			pt->pn = pbh + (int)pt->pn;
		for (j = 0; j < pt->count; ++j) {
			pn = &pt->pn[j];
			pn->k.s = pbh + (int)pn->k.s;
		}
	}
	return 1;
}

TFS_API tfsE_t tfsE_Create(c_str name) {
	HeaderEx* phe;
	Header* ph;
	assert(name);
	phe = (HeaderEx*)malloc(sizeof(HeaderEx));
	assert(phe);
	phe->ph = newheader(name);
	assert(phe->ph);
	phe->at = atom_create();
	assert(phe->at);
	phe->ph->pt = newtable(null);
	assert(phe->ph->pt);
	ph = phe->ph;
	ph->v8ca = 256;
	ph->pv8 = resizearr(null, sizeof(TValue8), 0, ph->v8ca);
	assert(ph->pv8);
	ph->v32ca = 256;
	ph->pv32 = resizearr(null, sizeof(TValue32), 0, ph->v32ca);
	assert(ph->pv32);
	ph->v64ca = 256;
	ph->pv64 = resizearr(null, sizeof(TValue64), 0, ph->v64ca);
	assert(ph->pv64);
	return phe;
}

TFS_API tfs_t tfsE_GetTfs(tfsE_t pfse) {
	if (pfse)
		return pfse->ph;
	return null;
}

tfs_Value tfsE_SetValue8(tfsE_t pfse, tfs_Table tab, c_str name, TValue8 v8, SEG_TYPE st) {
	TValue v;
	Node* pn;
	assert(pfse);
	assert(tab);
	assert(name);
	name = atom_string(pfse->at, name);
	pn = tfind(tab, name);
	if (pn == null) {
		pn = tinsert(tab, name);
		v = v8tov(pfse->ph, v8, st);
		pn->v = v;
	}
	else {
		v = v8tov(pfse->ph, v8, st);
		pn->v = v;
	}
	return v;
}

tfs_Value tfsE_SetValue32(tfsE_t pfse, tfs_Table tab, c_str name, TValue32 v32, SEG_TYPE st) {
	TValue v;
	Node* pn;
	assert(pfse);
	assert(tab);
	assert(name);
	name = atom_string(pfse->at, name);
	pn = tfind(tab, name);
	if (pn == null) {
		pn = tinsert(tab, name);
		v = v32tov(pfse->ph, v32, st);
		pn->v = v;
	}
	else {
		v = v32tov(pfse->ph, v32, st);
		pn->v = v;
	}
	return v;
}

tfs_Value tfsE_SetValue64(tfsE_t pfse, tfs_Table tab, c_str name, TValue64 v64, SEG_TYPE st) {
	TValue v;
	Node* pn;
	assert(pfse);
	assert(tab);
	assert(name);
	name = atom_string(pfse->at, name);
	pn = tfind(tab, name);
	if (pn == null) {
		pn = tinsert(tab, name);
		v = v64tov(pfse->ph, v64, st);
		pn->v = v;
	}
	else {
		v = v64tov(pfse->ph, v64, st);
		pn->v = v;
	}
	return v;
}

TFS_API tfs_Value tfsE_SetTable(tfsE_t pfse, tfs_Table tab, c_str name) {
	TValue v;
	Node* pn;
	Table* pt;
	assert(pfse);
	assert(tab);
	assert(name);
	name = atom_string(pfse->at, name);
	pn = tfind(tab, name);
	if (pn != null)
		return 0;
	pt = newtable(name);
	assert(pt);
	pn = tinsert(tab, name);
	v = vtabletov(pfse->ph, pt);
	pn->v = v;
	return v;
}

TFS_API tfs_Table tfsE_GetTable(tfsE_t pfse, c_str name) {
	Table* pt;
	ulong i;
	assert(pfse);
	if (name == null)
		return pfse->ph->pt;
	name = atom_string(pfse->at, name);
	for (i = 0; i < pfse->ph->tablec; ++i) {
		pt = (Table*)pfse->ph->ptable[i];
		if (pt->name.s == name)
			return pt;
	}
	return null;
}

TFS_API tfs_Value tfsE_SetBool(tfsE_t pfse, tfs_Table tab, c_str name, bool b) {
	TValue8 v8;
	v8.bl = b;
	return tfsE_SetValue8(pfse, tab, name, v8, SEG_VALUE8_BOOL);
}

TFS_API tfs_Value tfsE_SetByte(tfsE_t pfse, tfs_Table tab, c_str name, byte b) {
	TValue8 v8;
	v8.bt = b;
	return tfsE_SetValue8(pfse, tab, name, v8, SEG_VALUE8_BYTE);
}

TFS_API tfs_Value tfsE_SetInt32(tfsE_t pfse, tfs_Table tab, c_str name, int n) {
	TValue32 v32;
	v32.n = n;
	return tfsE_SetValue32(pfse, tab, name, v32, SEG_VALUE32_INT);
}

TFS_API tfs_Value tfsE_SetFloat(tfsE_t pfse, tfs_Table tab, c_str name, float f) {
	TValue32 v32;
	v32.f = f;
	return tfsE_SetValue32(pfse, tab, name, v32, SEG_VALUE32_FLOAT);
}

TFS_API tfs_Value tfsE_SetString(tfsE_t pfse, tfs_Table tab, c_str name, c_str s) {
	TValue v;
	Node* pn;
	assert(pfse);
	assert(tab);
	assert(name);
	name = atom_string(pfse->at, name);
	s = atom_string(pfse->at, s);
	pn = tfind(tab, name);
	if (pn == null) {
		pn = tinsert(tab, name);
		v = vstringtov(pfse->ph, s);
		pn->v = v;
	}
	else {
		v = vstringtov(pfse->ph, s);
		pn->v = v;
	}
	return v;
}

//TFS_API tfs_Value tfsE_SetObject(tfsE_t pfse, tfs_Table tab, c_str name, void* pv) {
//	TValue32 v32;
//	v32.pv = pv;
//	return tfsE_SetValue32(pfse, tab, name, v32);
//}

TFS_API tfs_Value tfsE_SetDouble(tfsE_t pfse, tfs_Table tab, c_str name, double d) {
	TValue64 v64;
	v64.d = d;
	return tfsE_SetValue64(pfse, tab, name, v64, SEG_VALUE64_DOUBLE);
}

TFS_API tfs_Value tfsE_SetInt64(tfsE_t pfse, tfs_Table tab, c_str name, int64 i64) {
	TValue64 v64;
	v64.i64 = i64;
	return tfsE_SetValue64(pfse, tab, name, v64, SEG_VALUE64_INT);
}

TFS_API int tfsE_SetNull(tfsE_t pfse, tfs_Table tab, c_str name) {
	Node* pn;
	assert(pfse);
	assert(tab);
	assert(name);
	name = atom_string(pfse->at, name);
	pn = tfind(tab, name);
	if (pn != null) {
		pn->v = 0;
		return 1;
	}
	return 0;
}

static int pathvalid(c_str file) {
	size_t i, len;
	char path[256];
	int flag;
	if (file == null)
		return 0;
	len = strlen(file);
	if (file[len - 1] == '\\' || file[len - 1] == '/')
		return 0;
	strcpy(path, file);
	flag = 0;
	for (i = len - 1; i >= 0; --i) {
		if (flag && !(path[i] == '\\' || path[i] == '/')) {
			path[i + 1] = '\0';
			break;
		}
		if (path[i] == '\\' || path[i] == '/')
			flag++;
	}
	if (_access(path, _A_NORMAL) == -1)
		return 0;
	return 1;
}

static void replacevalue(Header* ph, Table* pt, TValue vo, TValue vn) {
	ulong i, index;
	Table* next;
	for (i = 0; i < pt->count; ++i) {
		if (pt->pn[i].v == vo)
			pt->pn[i].v = vn;
		else if (istable(pt->pn[i].v)) {
			index = value_getindex(pt->pn[i].v);
			next = (Table*)(ph->ptable[index]);
			replacevalue(ph, next, vo, vn);
		}
	}
}

static int compressv8(tfsE_t pfse) {
	ulong i, j, c, flag;
	TValue8* pv8;
	TValue vo, vn;
	Header* ph;
	ph = pfse->ph;
	if (ph->v8c == 0) {
		ph->v8c = ph->v8ca = 0;	
		return 1;
	}
	c = 0;
	pv8 = (TValue8*)malloc(sizeof(TValue8) * ph->v8c);
	assert(pv8);
	for (i = 0; i < ph->v8c; ++i) {
		flag = 0;
		for (j = 0; j < c; ++j) {
			if (ph->pv8[i].bt == pv8[j].bt) {
				flag = 1;
				vo = make_value(SEG_VALUE8_BOOL, i);
				vn = make_value(SEG_VALUE8_BOOL, j);
				replacevalue(ph, ph->pt, vo, vn);
				vo = make_value(SEG_VALUE8_BYTE, i);
				vn = make_value(SEG_VALUE8_BYTE, j);
				replacevalue(ph, ph->pt, vo, vn);
				break;
			}
		}
		if (!flag) {
			pv8[c] = ph->pv8[i];
			vo = make_value(SEG_VALUE8_BOOL, i);
			vn = make_value(SEG_VALUE8_BOOL, c);
			if (vo != vn)
				replacevalue(ph, ph->pt, vo, vn);
			vo = make_value(SEG_VALUE8_BYTE, i);
			vn = make_value(SEG_VALUE8_BYTE, c);
			if (vo != vn)
				replacevalue(ph, ph->pt, vo, vn);
			c++;
		}
	}
	ph->v8c = c;
	ph->v8ca = c;
	free(ph->pv8);
	ph->pv8 = pv8;
	return 1;
}

static int compressv32(tfsE_t pfse) {
	ulong i, j, c, flag;
	TValue32* pv32;
	TValue vo, vn;
	Header* ph;
	ph = pfse->ph;
	if (ph->v32c == 0) {
		ph->v32c = ph->v32ca = 0;	
		return 1;
	}
	c = 0;
	pv32 = (TValue32*)malloc(sizeof(TValue32) * ph->v32c);
	assert(pv32);
	for (i = 0; i < ph->v32c; ++i) {
		flag = 0;
		for (j = 0; j < c; ++j) {
			if (ph->pv32[i].n == pv32[j].n) {
				flag = 1;
				vo = make_value(SEG_VALUE32_INT, i);
				vn = make_value(SEG_VALUE32_INT, j);
				replacevalue(ph, ph->pt, vo, vn);
				vo = make_value(SEG_VALUE32_FLOAT, i);
				vn = make_value(SEG_VALUE32_FLOAT, j);
				replacevalue(ph, ph->pt, vo, vn);
				break;
			}
		}
		if (!flag) {
			pv32[c] = ph->pv32[i];
			vo = make_value(SEG_VALUE32_INT, i);
			vn = make_value(SEG_VALUE32_INT, c);
			if (vo != vn)
				replacevalue(ph, ph->pt, vo, vn);
			vo = make_value(SEG_VALUE32_FLOAT, i);
			vn = make_value(SEG_VALUE32_FLOAT, c);
			if (vo != vn)
				replacevalue(ph, ph->pt, vo, vn);
			c++;
		}
	}
	ph->v32c = c;
	ph->v32ca = c;
	free(ph->pv32);
	ph->pv32 = pv32;
	return 1;
}

static int compressv64(tfsE_t pfse) {
	ulong i, j, c, flag;
	TValue64* pv64;
	TValue vo, vn;
	Header* ph;
	ph = pfse->ph;
	if (ph->v64c == 0) {
		ph->v64c = ph->v64ca = 0;
		return 1;
	}
	c = 0;
	pv64 = (TValue64*)malloc(sizeof(TValue64) * ph->v64c);
	assert(pv64);
	for (i = 0; i < ph->v64c; ++i) {
		flag = 0;
		for (j = 0; j < c; ++j) {
			if (ph->pv64[i].d == pv64[j].d) {
				flag = 1;
				vo = make_value(SEG_VALUE64_INT, i);
				vn = make_value(SEG_VALUE64_INT, j);
				replacevalue(ph, ph->pt, vo, vn);
				vo = make_value(SEG_VALUE64_DOUBLE, i);
				vn = make_value(SEG_VALUE64_DOUBLE, j);
				replacevalue(ph, ph->pt, vo, vn);
				break;
			}
		}
		if (!flag) {
			pv64[c] = ph->pv64[i];
			vo = make_value(SEG_VALUE64_INT, i);
			vn = make_value(SEG_VALUE64_INT, c);
			if (vo != vn)
				replacevalue(ph, ph->pt, vo, vn);
			vo = make_value(SEG_VALUE64_DOUBLE, i);
			vn = make_value(SEG_VALUE64_DOUBLE, c);
			if (vo != vn)
				replacevalue(ph, ph->pt, vo, vn);
			c++;
		}
	}
	ph->v64c = c;
	ph->v64ca = c;
	free(ph->pv64);
	ph->pv64 = pv64;
	return 1;
}

static int compresstable(Header* ph, Table* pt) {
	int i, index, ct;
	Table* next;
	for (i = 0; i < pt->count; ++i) {
		if (istable(pt->pn[i].v)) {
			index = value_getindex(pt->pn[i].v);
			next = (Table*)ph->ptable[index];
			compresstable(ph, next);
		}
	}
	ct = 0;
	for (i = pt->count - 1; i >= 0; --i) {
		if (isnull(pt->pn[i].v)) {
			pt->pn[i] = pt->pn[pt->count - ct - 1];
			ct++;
		}
	}
	pt->count -= ct;
	pt->capacity = pt->count;
	return 1;
}

static int compressstring(HeaderEx* pfse) {
	ulong i, j, c, flag;
	TValueString* pvstr;
	TValue vo, vn;
	Header* ph;
	ph = pfse->ph;
	if (ph->stringc == 0) {
		ph->stringc = ph->stringca = 0;	
		return 1;
	}
	c = 0;
	pvstr = (TValueString*)malloc(sizeof(TValueString) * ph->stringc);
	assert(pvstr);
	for (i = 0; i < ph->stringc; ++i) {
		flag = 0;
		vo = make_value(SEG_STRING, i);
		for (j = 0; j < c; ++j) {
			if (ph->pstring[i] == pvstr[j]) {
				flag = 1;
				vn = make_value(SEG_STRING, j);
				replacevalue(ph, ph->pt, vo, vn);
				break;
			}
		}
		if (!flag) {
			pvstr[c] = ph->pstring[i];
			vn = make_value(SEG_STRING, c);
			if (vo != vn)
				replacevalue(ph, ph->pt, vo, vn);
			c++;
		}
	}
	ph->stringc = c;
	ph->stringca = c;
	free(ph->pstring);
	ph->pstring = pvstr;
	return 1;
}

static int compress(tfsE_t pfse) {
	if (!compressv8(pfse))
		return 0;
	if (!compressv32(pfse))
		return 0;
	if (!compressv64(pfse))
		return 0;
	if (!compressstring(pfse))
		return 0;
	if (!compresstable(pfse->ph, pfse->ph->pt))
		return 0;
	pfse->ph->pt->capacity = pfse->ph->pt->count;
	pfse->ph->tableca = pfse->ph->tablec;
	return 1;
}

static int tablecmp(const void* a, const void* b) {
	return ((Node*)a)->k.h - ((Node*)b)->k.h;
}

static int sorttable(Header* ph, Table* pt) {
	ulong i, index;
	Table* next;
	for (i = 0; i < pt->count; ++i) {
		if (istable(pt->pn[i].v)) {
			index = value_getindex(pt->pn[i].v);
			next = (Table*)ph->ptable[index];
			sorttable(ph, next);
		}
	}
	qsort(pt->pn, pt->count, sizeof(Node), tablecmp);
	return 1;
}

static void mark(Header* ph, Table* tab, bool* pbv8, bool* pbv32, bool* pbv64, bool* pbvtab, bool* pbvstr) {
	ulong i, seg, index;
	Table* next;
	for (i = 0; i < tab->count; ++i) {
		seg = value_getsegment(tab->pn[i].v);
		index = value_getindex(tab->pn[i].v);
		switch (seg) {
			case SEG_VALUE8_BOOL:
			case SEG_VALUE8_BYTE:
				pbv8[index] = 1;
				break;
			case SEG_VALUE32_INT:
			case SEG_VALUE32_FLOAT:
				pbv32[index] = 1;
				break;
			case SEG_VALUE64_INT:
			case SEG_VALUE64_DOUBLE:
				pbv64[index] = 1;
				break;
			case SEG_TABLE:
				pbvtab[index] = 1;
				next = (Table*)ph->ptable[index];
				mark(ph, next, pbv8, pbv32, pbv64, pbvtab, pbvstr);
				break;
			case  SEG_STRING:
				pbvstr[index] = 1;
				break;
		}
	}
}

static int filter(Header* ph) {
	bool* pbv8, *pbv32, *pbv64, *pbvtab, *pbvstr;
	TValue8 v8;
	TValue32 v32;
	TValue64 v64;
	ulong i;
	pbv8 = null;
	if (ph->v8c) {
		pbv8 = (bool*)malloc(ph->v8c);
		assert(pbv8);
		memset(pbv8, 0, ph->v8c);
	}
	pbv32 = null;
	if (ph->v32c) {
		pbv32 = (bool*)malloc(ph->v32c);
		assert(pbv32);
		memset(pbv32, 0, ph->v32c);
	}
	pbv64 = null;
	if (ph->v64c) {
		pbv64 = (bool*)malloc(ph->v64c);
		assert(pbv64);
		memset(pbv64, 0, ph->v64c);
	}
	pbvtab = null;
	if (ph->tablec) {
		pbvtab = (bool*)malloc(ph->tablec);
		assert(pbvtab);
		memset(pbvtab, 0, ph->tablec);
	}
	pbvstr = null;
	if (ph->stringc) {
		pbvstr = (bool*)malloc(ph->stringc);
		assert(pbvstr);
		memset(pbvstr, 0, ph->stringc);
	}
	mark(ph, ph->pt, pbv8, pbv32, pbv64, pbvtab, pbvstr);
	v8.bt = 0;
	v32.n = 0;
	v64.i64 = 0;
	for (i = 0; i < ph->v8c; ++i)
		if (!pbv8[i])
			ph->pv8[i] = v8;
	for (i = 0; i < ph->v32c; ++i)
		if (!pbv32[i])
			ph->pv32[i] = v32;
	for (i = 0; i < ph->v64c; ++i)
		if (!pbv64[i])
			ph->pv64[i] = v64;
	for (i = 0; i < ph->tablec; ++i)
		if (!pbvtab[i])
			ph->ptable[i] = 0;
	for (i = 0; i < ph->stringc; ++i)
		if (!pbvstr[i])
			ph->pstring[i] = 0;
	safe_free(pbv8);
	safe_free(pbv32);
	safe_free(pbv64);
	safe_free(pbvtab);
	safe_free(pbvstr);
	return 1;
}

static int prepare(tfsE_t pfse) {
	Header* ph;
	ph = pfse->ph;
	ph->major = version_major;
	ph->minor = version_minor;
	time((time_t*)(&ph->timestamp));
	ph->size = 0;
	if (!filter(ph))
		return 0;
	if (!compress(pfse))
		return 0;
	if (!sorttable(ph, ph->pt))
		return 0;
	return 1;
}

static int writefile(c_str file, tfsE_t pfse) {
	Buffer buffer;
	ulong count, i, j;
	int index, ret;
	Header* ph;
	c_str str;
	Table* pt;
	FILE* pf;
	Node* pn;
	Strmap sm;
	ret = 0;
	pf = null;
	memset(&buffer, 0, sizeof(Buffer));
	memset(&sm, 0, sizeof(Strmap));
	buffer.cb = sizeof(Buffer);
	ph = pfse->ph;
	count = addbuffer(&buffer, ph, sizeof(Header));
	count = addbuffer(&buffer, ph->pv8, sizeof(TValue8) * ph->v8c);
	((Header*)(buffer.buff))->pv8 = count;
	count = addbuffer(&buffer, ph->pv32, sizeof(TValue32) * ph->v32c);
	((Header*)(buffer.buff))->pv32 = count;
	count = addbuffer(&buffer, ph->pv64, sizeof(TValue64) * ph->v64c);
	((Header*)(buffer.buff))->pv64 = count;
	pt = ph->pt;
	if (pt->count && pt->pn) {
		for (i = 0; i < pt->count; ++i) {
			pn = &pt->pn[i];
			str = pn->k.s;
			if (str) {
				index = sfind(&sm, str);
				if (index == -1) {
					count = addbuffer(&buffer, str, strlen(str) + 1);
					pn->k.s = count;
					sinsert(&sm, str, count);
				}
				else
					pn->k.s = index;
			}
		}
		count = addbuffer(&buffer, pt->pn, sizeof(Node) * pt->count);
		pt->pn = count;
	}
	count = addbuffer(&buffer, pt, sizeof(Table));
	((Header*)(buffer.buff))->pt = count;
	for (i = 0; i < ph->stringc; ++i) {
		str = ph->pstring[i];
		if (str) {
			index = sfind(&sm, str);
			if (index == -1) {
				count = addbuffer(&buffer, str, strlen(str) + 1);
				ph->pstring[i] = count;
				sinsert(&sm, str, count);
			}
			else
				ph->pstring[i] = index;
		}
	}
	if (ph->pstring) {
		count = addbuffer(&buffer, ph->pstring, sizeof(TValueString) * ph->stringc);
		((Header*)(buffer.buff))->pstring = count;
	}
	for (i = 0; i < ph->tablec; ++i) {
		pt = ph->ptable[i];
		if (pt) {
			str = pt->name.s;
			if (str) {
				index = sfind(&sm, str);
				if (index == -1) {
					count = addbuffer(&buffer, str, strlen(str) + 1);
					pt->name.s = count;
					sinsert(&sm, str, count);
				}
				else
					pt->name.s = index;
			}
			if (pt->count && pt->pn) {
				for (j = 0; j < pt->count; ++j) {
					pn = &pt->pn[j];
					str = pn->k.s;
					if (str) {
						index = sfind(&sm, str);
						if (index == -1) {
							count = addbuffer(&buffer, str, strlen(str) + 1);
							pn->k.s = count;
							sinsert(&sm, str, count);
						}
						else
							pn->k.s = index;
					}
				}
				count = addbuffer(&buffer, pt->pn, sizeof(Node) * pt->count);
				pt->pn = count;
			}
			count = addbuffer(&buffer, pt, sizeof(Table));
			ph->ptable[i] = count;
		}
	}
	if (ph->ptable) {
		count = addbuffer(&buffer, ph->ptable, sizeof(TValueTable) * ph->tablec);
		((Header*)(buffer.buff))->ptable = count;
	}
	((Header*)(buffer.buff))->size = buffer.count;
	pf = fopen(file, "wb");
	if (!pf)
		goto failed;
	count = fwrite(buffer.buff, 1, buffer.count, pf);
	if (count != buffer.count)
		goto failed;
	ret = 1;
failed:
	if (pf)
		fclose(pf);
	sclear(&sm);
	bclear(&buffer);
	return ret;
}

TFS_API c_str tfsE_Save(c_str file, tfsE_t pfse) {
	assert(pfse);
	if (!pathvalid(file))
		return null;
	if (!prepare(pfse))
		return null;
	if (!writefile(file, pfse))
		return null;
	return file;
}

static void copynodes(HeaderEx* phe, Header* ph, Table* dst, Table* src) {
	ulong i;
	Node* pn;
	c_str str;
	Table* ptd, *pts;
	TValue8 v8;
	TValue32 v32;
	TValue64 v64;
	for (i = 0; i < src->count; ++i) {
		pn = &src->pn[i];
		if (isbyte(pn->v)) {
			v8.bt = tfs_tobyte(ph, pn->v);
			tfsE_SetValue8(phe, dst, pn->k.s, v8, SEG_VALUE8_BYTE);
		}
		else if (isbool(pn->v)) {
			v8.bl = tfs_tobool(ph, pn->v);
			tfsE_SetValue8(phe, dst, pn->k.s, v8, SEG_VALUE8_BOOL);
		}
		else if (isint32(pn->v)) {
			v32.n = tfs_toint32(ph, pn->v);	
			tfsE_SetValue32(phe, dst, pn->k.s, v32, SEG_VALUE32_INT);
		}
		else if (isfloat(pn->v)) {
			v32.f = tfs_tofloat(ph, pn->v);	
			tfsE_SetValue32(phe, dst, pn->k.s, v32, SEG_VALUE32_FLOAT);
		}
		else if (isint64(pn->v)) {
			v64.i64 = tfs_toint64(ph, pn->v);		
			tfsE_SetValue64(phe, dst, pn->k.s, v64, SEG_VALUE64_INT);
		}
		else if (isdouble(pn->v)) {
			v64.d = tfs_todouble(ph, pn->v);		
			tfsE_SetValue64(phe, dst, pn->k.s, v64, SEG_VALUE64_DOUBLE);
		}
		else if (isstring(pn->v)) {
			str = tfs_tostring(ph, pn->v);
			tfsE_SetString(phe, dst, pn->k.s, str);
		}
		else if (istable(pn->v)) {
			pts = tfs_totable(ph, pn->v);
			ptd = tfs_totable(phe->ph, tfsE_SetTable(phe, dst, pn->k.s));
			copynodes(phe, ph, ptd, pts);
		}
	}
}

TFS_API tfsE_t tfsE_From(tfs_t tfs) {
	HeaderEx* phe;
	Header* ph;
	Table* pt;
	phe = tfsE_Create(tfs->name);
	assert(phe);
	ph = phe->ph;
	ph->major = tfs->major;
	ph->minor = tfs->minor;
	ph->timestamp = tfs->timestamp;
	ph->size = tfs->size;
	pt = ph->pt;
	copynodes(phe, tfs, pt, tfs->pt);
	return phe;
}

TFS_API tfsE_t tfsE_Load(c_str file) {
	tfs_t t;
	tfsE_t te;
	t = te = null;
	t = tfs_Load(file);
	if (t) {
		te = tfsE_From(t);
		tfs_Unload(t);
	}
	return te;
}

TFS_API c_str tfsE_GetTableName(tfs_Table tab) {
	assert(tab);
	return tab->name.s;
}

TFS_API uint tfsE_GetTableSize(tfs_Table tab) {
	assert(tab);
	return tab->count;
}

TFS_API c_str tfsE_GetKeyAt(tfs_Table tab, uint index) {
	assert(tab);
	assert(index < tab->count);
	return tab->pn[index].k.s;
}

TFS_API tfs_Value tfsE_GetValueAt(tfs_Table tab, uint index) {
	assert(tab);
	assert(index < tab->count);
	return tab->pn[index].v;
}

TFS_API tfs_Table tfs_totable(tfs_t pfs, tfs_Value v) {
	ulong index;
	if (!pfs || !istable(v))
		return null;
	index = value_getindex(v);
	if (index >= pfs->tablec)
		return null;
	return pfs->ptable[index];
}

TFS_API tfs_t tfs_Load(c_str file) {
	Header* ph;
	FILE* pf;
	int size, res, fn;
	size_t count;
	assert(file);
	res = 0;
	ph = pf = null;
	pf = fopen(file, "rb");
	if (!pf)
		goto failed;
	fn = fileno(pf);
	size = filelength(fn);
	if (size < sizeof(Header))
		goto failed;
	ph = (Header*)malloc(size);
	assert(ph);
	count = fread(ph, 1, size, pf);
	if (count != size || ph->signature != TFS_SIGNATURE || count != ph->size)
		goto failed;
	if (!serialbuffer(ph))
		goto failed;
	res = 1;
failed:
	if (!res) {
		if (pf)
			fclose(pf);
		if (ph) {
			free(ph);
			ph = null;
		}
	}
	return ph;
}

TFS_API void tfs_Unload(tfs_t pfs) {
	if (pfs) {
		free(pfs);
		pfs = null;
	}
}

TFS_API tfs_Table tfs_GetTable(tfs_t pfs, c_str name) {
	ulong i;
	int h;
	Table* pt;
	assert(pfs);
	if (!name) 
		return pfs->pt;
	h = hash_string(name);
	for (i = 0; i < pfs->tablec; ++i) {
		pt = pfs->ptable[i];
		if (pt->name.h == h)
			return pt;
	}
	return null;
}

TFS_API tfs_Value tfs_GetValue(tfs_t pfs, tfs_Table tab, c_str name) {
	int h, l, u, m;
	assert(pfs);
	assert(name);
	if (!tab)
		tab = pfs->pt;
	if (tab->count == 0)
		return 0;
	l = 0;
	u = tab->count - 1;
	h = hash_string(name);
	while (l <= u) {
		m = (l + u) / 2;
		if (tab->pn[m].k.h == h)
			return tab->pn[m].v;
		else if (tab->pn[m].k.h > h)
			u = m - 1;
		else
			l = m + 1;
	} 
	return 0;
}

TFS_API c_str tfs_tostring(tfs_t pfs, tfs_Value v) {
	ulong index;
	assert(pfs);
	if (!isstring(v))
		return null;
	index = value_getindex(v);
	if (index < pfs->stringc)
		return pfs->pstring[index];
	return null;
}

TFS_API TValue8 tfs_tovalue8(tfs_t pfs, tfs_Value v) {
	ulong index;
	TValue8 v8;
	assert(pfs);
	v8.bt = 0;
	if (!isbool(v) && !isbyte(v))
		return v8;
	index = value_getindex(v);
	if (index < pfs->v8c)
		return pfs->pv8[index];
	return v8;
}

TFS_API TValue32 tfs_tovalue32(tfs_t pfs, tfs_Value v) {
	ulong index;
	TValue32 v32;
	assert(pfs);
	v32.n = 0;
	if (!isint32(v) && !isfloat(v))
		return v32;
	index = value_getindex(v);
	if (index < pfs->v32c)
		return pfs->pv32[index];
	return v32;
}

TFS_API TValue64 tfs_tovalue64(tfs_t pfs, tfs_Value v) {
	ulong index;
	TValue64 v64;
	assert(pfs);
	v64.i64 = 0;
	if (!isint64(v) && !isdouble(v))
		return v64;
	index = value_getindex(v);
	if (index < pfs->v64c)
		return pfs->pv64[index];
	return v64;
}
