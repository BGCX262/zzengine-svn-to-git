#ifndef _TINY_LOG_EDIT_H
#define _TINY_LOG_EDIT_H

#include "Interface/IETinyLog.h"

typedef std::vector<ETL_LOG> LogVector;
typedef LogVector::iterator LogVectorPtr;

typedef std::vector<void*> BufferVector;
typedef BufferVector::iterator BufferVectorPtr;

class TinyLogE : public IETinyLog
{
public:
	TinyLogE();
	virtual ~TinyLogE();

public:
	int Release();

	int LoadFromFile(const char* pcszFilePath);
	int LoadFromAssemble(const char* pcszName);

	int SelectAll(ETL_LOG** pLogs, int nCount);
	int SelectItems(ETL_LOG* pKey, ETL_LOG** pLogs, int nCount);

	int GetFileInfo(ETL_FILE_INFO* pInfo, size_t uSize);

	const char* ToString(ETL_LOG* pLog);

private:
	ETL_FILE_INFO m_FileInfo;

	LogVector m_Logs;
	BufferVector m_Buffers;

	char m_szStr[MAX_FILE_LENGTH];

private:
	int Reset();
	
	int GetItemCount(ETL_LOG* pKey);
	int GetFileInfo(const char* pcszFile);

	int NextFile(int nFile);	/* return -1 means end. 0 for Error. file index for else. */
	int AppendLogs(char* pBuffer, int nSize);
};

#endif	//_TINY_LOG_EDIT_H