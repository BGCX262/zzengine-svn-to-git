#include "stdafx.h"
#include "hash.h"

static int hashstring(const char* s, int seed) {
	const char* str = s;
	int hash = 0;
	while (*str) {
		hash = (*str++) + hash * seed;
	}
	return hash & 0x7FFFFFFF;
}

ZENGINE_API int hash_string(c_str str) {
	assert(str);
	return hashstring(str, 1);
}