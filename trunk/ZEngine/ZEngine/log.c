#include "stdafx.h"
#include "log.h"
#include "atom.h"
#include "hash.h"
#include "file.h"
#include "crt.h"

typedef struct log {
	int cb;
	atom_t ap;
}log_t;

static struct LOG_PARAM _log_param;
static char _prefix[max_path];

static int create_log() {
	int ret;
	time_t tmt;
	ret = _mkdir(_log_param.path);
	if (ret == -1 && errno != EEXIST) {
		fprintf(stderr, "[Error] _mkdir(%s) failed\n", _log_param.path);
		return 0;
	}

	
	return 0;
}

ZENGINE_API int log_init(LOG_PARAM *param) {
	int ret;
	assert(param && "Invalid log params.");
	if (param->cb != sizeof(LOG_PARAM))
		return 0;
	memcpy(&_log_param, param, param->cb);
	if (param->options == LOG_OPTIONS_NONE && param->path[0] != '\0')
		_log_param.options = LOG_OPTIONS_FILE;

	if (_log_param.options & LOG_OPTIONS_FILE) {
		strcpy_s(_prefix, sizeof(_prefix), _log_param.path);
		ret = (int)strlen(_prefix);
		if (_prefix[ret - 1] == '/')
			_prefix[ret - 1] = '\\';
		if (_prefix[ret - 1] != '\\') {
			_prefix[ret] = '\\';
			_prefix[ret + 1] = '\0';
		}
		strcat_s(_prefix, sizeof(_prefix), _log_param.ident);
		return create_log();
	}
	return 0;
}