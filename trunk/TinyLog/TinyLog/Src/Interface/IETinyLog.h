#ifndef _IE_TINY_LOG_H
#define _IE_TINY_LOG_H

#include "TLBase.h"
#include "TLDefine.h"

struct ETL_LOG_TIME		/* value -1 is wildcard */
{
	int nYear;
	int nMonth;
	int nDay;
	int nHour;
	int nMinuter;
	int nSecond;
};

struct ETL_LOG
{
	char BeginTag[3];	/* 0xFFF or 0xFFE */
	ETL_LOG_TIME Time;
	const char* pcszClassify;
	TINY_LOG_PRIORITY Pri;
	const char* pcszCondition;
	int nLine;
	const char* pcszFunctionName;
	const char* pcszInfo;
};

struct ETL_FILE_INFO
{
	char szFileName[FILE_NAME_LENGTH];
	char szFilePath[FILE_PATH_LENGTH];

	int nFileCount;

	char szTag[8];
	char szVersion[8];

	time_t ttTime;
};

struct IETinyLog
{
	virtual int Release() = 0;

	virtual int LoadFromFile(const char* pcszFilePath) = 0;
	virtual int LoadFromAssemble(const char* pcszName) = 0;

	/* 
		if nCount == -1, return item counts 
		you can use this number to malloc memory to receive Logs
	*/
	virtual int SelectAll(ETL_LOG** pLogs, int nCount) = 0;
	virtual int SelectItems(ETL_LOG* pKey, ETL_LOG** pLogs, int nCount) = 0;

	virtual int GetFileInfo(ETL_FILE_INFO* pInfo, size_t uSize) = 0;

	virtual const char* ToString(ETL_LOG* pLog) = 0;
};

enum eIETLCode
{
	eCreateIETLSuccess = 0,
	eCreateIETLError,
};

struct ETINY_LOG_PARAM
{
	unsigned cbSize;
	int nVersionCreate;
};

TLAPI eIETLCode CreateETinyLog(ETINY_LOG_PARAM* pParam, IETinyLog** ppi);

#endif