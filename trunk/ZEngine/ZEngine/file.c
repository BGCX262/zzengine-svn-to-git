#include "stdafx.h"
#include "file.h"

typedef struct FT {
	int cb;
}FT;

ZENGINE_API bool file_exist(c_str path) {
	assert(path);
	return !(GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY);
}