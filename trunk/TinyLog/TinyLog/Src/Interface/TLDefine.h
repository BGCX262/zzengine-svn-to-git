#ifndef _TINY_LOG_DEFINE_H
#define _TINY_LOG_DEFINE_H

enum TINY_LOG_PRIORITY
{
	eTinyLogPriorityReserve0 = 0,
	eTinyLogPriorityReserve1 = 1,
	eTinyLogPriorityReserve2 = 2,
	eTinyLogPriorityError = 3,
	eTinyLogPriorityReserve4 = 4,
	eTinyLogPriorityWarning = 5,
	eTinyLogPriorityReserve6 = 6,
	eTinyLogPriorityInfo = 7,
	eTinyLogPriorityReserve8 = 8,
	eTinyLogPriorityDebug = 9,
	eTinyLogPriorityReserve10 = 10,

	eTinyLogPriorityCount
};

enum TINY_LOG_OPTION
{
	eTinyLogOptionFile = 0x01,
	eTinyLogOptionConsole = 0x01 << 1,
	eTinyLogOptionStderr = 0x01 << 2,
};

#define GLOBAL_CLASSIFY_NAME	"_Global"

struct TL_LOG_INFO
{
	char BeginTag[3];		/* 0xFF; beginning of a log. 0xFFE for normal; 0xFFF for offset. */
	char oftSecond[4];		/* offset; seconds passed from log begin */
	char oftClassify[2];	/* offset; const char* pcszClassity */
	char idxPriority;		/* enum of TINY_LOG_PRIORITY; */
	char oftCondition[2];	/* offset; const char* pcszCondition */
	char nLine[4];			/* int; code line number */
	char oftFunctionName[2];/* offset; const char* pcszFunctionName */
	char oftInfo[2];		/* offset; const char* pcszAdditionInfo */
};	/* 20 bytes total */

typedef unsigned long FOURCC;

#define TLOG_TAG		'TLOG'
#define VERSION0		'V000'

struct TL_RUNTIME_PATTERN
{
	unsigned cbSize;

	FOURCC fccTag;				/* 'TLOG' */
	FOURCC fccVersion;			/* 'V000' newest */
	time_t ttTimeStamp;
};

#define FILE_NAME_LENGTH	32
#define FILE_PATH_LENGTH	256

#define MAX_FILE_LENGTH		(1 << 16)

#endif	//_TINY_LOG_DEFINE_H