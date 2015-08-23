#ifndef _TINY_LOG_H
#define _TINY_LOG_H

#include "Interface/ITinyLog.h"
#include "Common/atom.h"

typedef std::map<c_str, short>	TLOffsetMap;
typedef TLOffsetMap::iterator	TLOffsetMapPtr;

class TinyLog : public ITinyLog
{
public:
	TinyLog();
	virtual ~TinyLog();

public:
	int Release();

	int Log(TINY_LOG_PRIORITY Priority, const char* csClassify, const char fmt[], ...);
	int Log(TINY_LOG_PRIORITY Priority 
		, const char* csClassify
		, const char* pcszCondition
		, int nLine 
		, const char* pcszFunctionName 
		, const char* pcszInfo /* = NULL */);

	TINY_LOG_PRIORITY SetPriority(TINY_LOG_PRIORITY priNew);

public:
	int Init(TINY_LOG_PARAM* pParam);

private:
	atom_t m_atLogs;
	atom_t m_atStrings;

	TLOffsetMap m_LogMap;
	TLOffsetMap m_StringMap;

	TINY_LOG_PARAM m_Param;

	int m_nIndex;
	char m_szFileName[FILE_NAME_LENGTH];
	char m_szFilePath[FILE_PATH_LENGTH];

	int m_nFileHandle;

	TINY_LOG_PRIORITY m_CurrentPriority;		//Todo: share memory

	TL_RUNTIME_PATTERN m_RTPattern;

	unsigned short m_usOffset;

	unsigned short m_usBufferOffset;
	char m_szBuffer[MAX_FILE_LENGTH];

#ifdef WIN32
	CRITICAL_SECTION m_CriticalSection;

	void Lock() { EnterCriticalSection(&m_CriticalSection); }
	void Unlock() { LeaveCriticalSection(&m_CriticalSection); }
#else
	pthread_mutex_t m_Mutex;

	void Lock() { pthread_mutex_lock(&m_Mutex); }
	void Unlock() { pthread_mutex_unlock(&m_Mutex); }
#endif

private:
	void Reset();
	int NextLogFile();
	int AddString(const char* pcszStr, unsigned short* pusRet);
};

#endif	//_TINY_LOG_H