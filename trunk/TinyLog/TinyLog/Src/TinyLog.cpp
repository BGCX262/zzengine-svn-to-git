#include "stdafx.h"
#include "TinyLog.h"

#define TLOG_OPENFILEFLAG   (O_CREAT | O_APPEND | O_WRONLY | O_TRUNC | O_BINARY)
#define TLOG_OPENFILEMODE   (S_IREAD | S_IWRITE)

TinyLog::TinyLog()
: m_atLogs(NULL)
, m_atStrings(NULL)
, m_nIndex(0)
, m_nFileHandle(-1)
, m_usOffset(0)
, m_usBufferOffset(0)
{
	Reset();

	memset(&m_Param, 0, sizeof(m_Param));
	
	m_szFileName[0] = '\0';
	m_szFilePath[0] = '\0';

	m_CurrentPriority = eTinyLogPriorityCount;

	m_RTPattern.cbSize = sizeof(m_RTPattern);
	m_RTPattern.fccTag = TLOG_TAG;
	m_RTPattern.fccVersion = VERSION0;
	m_RTPattern.ttTimeStamp = 0;

#ifdef WIN32
	InitializeCriticalSection(&m_CriticalSection);
#else
	pthread_mutex_init(&m_Mutex, NULL);
#endif
}

TinyLog::~TinyLog()
{
#ifdef WIN32
	DeleteCriticalSection(&m_CriticalSection);
#else
	pthread_mutex_destroy(&m_Mutex); 
#endif

	if (m_atLogs)
	{
		atom_destroy(m_atLogs);
		m_atLogs = NULL;
	}

	if (m_atStrings)
	{
		atom_destroy(m_atStrings);
		m_atStrings = NULL;
	}

	m_LogMap.clear();
	m_StringMap.clear();
}

int TinyLog::Release()
{
	delete this;
	return 0;
}

int TinyLog::Log(TINY_LOG_PRIORITY Priority, const char* csClassify, const char fmt[], ...)
{
	int nRetCode = false;
	int nResult = false;
	char szBuffer[1024];
	va_list marker;

	CONDITION_SUCCESS(Priority > m_CurrentPriority);

	va_start(marker, fmt);

	nRetCode = vsnprintf(szBuffer, sizeof(szBuffer) - 1, fmt, marker);

	va_end(marker);

	szBuffer[sizeof(szBuffer) - 1] = '\0';

	nRetCode = Log(Priority, csClassify, NULL, -1, NULL, szBuffer);
	CONDITION_ERROR(nRetCode);

Success:
	nResult = true;
Error:
	return nResult;
}

int TinyLog::Log(TINY_LOG_PRIORITY Priority 
				 , const char* csClassify
				 , const char* pcszCondition
				 , int nLine
				 , const char* pcszFunctionName 
				 , const char* pcszInfo)
{
	int nRetCode = false;
	int nResult = false;
	size_t nMaxCount = 0;
	time_t ttNow = 0;
	TL_LOG_INFO tlLog = { 0 };
	c_str cstr = NULL;
	TLOffsetMapPtr ptr;

	CONDITION_SUCCESS(Priority > m_CurrentPriority);

	if (csClassify)
		nMaxCount += strlen(csClassify) + 1;
	if (pcszCondition)
		nMaxCount += strlen(pcszCondition) + 1;
	if (pcszFunctionName)
		nMaxCount += strlen(pcszFunctionName) + 1;
	if (pcszInfo)
		nMaxCount += strlen(pcszInfo) + 1;

	nMaxCount += sizeof(TL_LOG_INFO);
	CONDITION_ERROR(nMaxCount < MAX_FILE_LENGTH - sizeof(TL_RUNTIME_PATTERN));

	if (nMaxCount > MAX_FILE_LENGTH - m_usOffset)
	{
		nRetCode = NextLogFile();
		CONDITION_ERROR(nRetCode);
	}

	ttNow = time(NULL);

	tlLog.BeginTag[0] = 0xF;
	tlLog.BeginTag[1] = 0xF;

	*(int*)tlLog.oftSecond = (int)(ttNow - m_RTPattern.ttTimeStamp);

	m_usBufferOffset = 0;

	Lock();

	nRetCode = AddString(csClassify, (unsigned short*)(tlLog.oftClassify));
	CONDITION_ERROR(nRetCode);

	nRetCode = AddString(pcszCondition, (unsigned short*)(tlLog.oftCondition));
	CONDITION_ERROR(nRetCode);

	*(int*)tlLog.nLine = nLine;
	tlLog.idxPriority = (char)Priority;

	nRetCode = AddString(pcszFunctionName, (unsigned short*)(tlLog.oftFunctionName));
	CONDITION_ERROR(nRetCode);

	nRetCode = AddString(pcszInfo, (unsigned short*)(tlLog.oftInfo));
	CONDITION_ERROR(nRetCode);

	cstr = atom_new(m_atLogs, tlLog.oftClassify
		, (uint32)sizeof(TL_LOG_INFO) - (tlLog.oftClassify - tlLog.BeginTag));
	ptr = m_LogMap.find(cstr);
	if (ptr != m_LogMap.end())
	{
		tlLog.BeginTag[2] = 0xF;
		*(unsigned short*)tlLog.oftClassify = ptr->second;

		unsigned int uBytes = &tlLog.idxPriority - tlLog.BeginTag;

		nRetCode = _write(m_nFileHandle, &tlLog, uBytes);
		CONDITION_ERROR(nRetCode == uBytes);

		m_usOffset += nRetCode;

		goto Success;
	}

	m_LogMap.insert(std::make_pair(cstr, m_usOffset));

	tlLog.BeginTag[2] = 0xE;

	nRetCode = _write(m_nFileHandle, &tlLog, sizeof(TL_LOG_INFO));
	CONDITION_ERROR(nRetCode == sizeof(TL_LOG_INFO));

	m_usOffset += nRetCode;

Success:
	nResult = true;
Error:
	Unlock();
	return nResult;
}

int TinyLog::Init(TINY_LOG_PARAM* pParam)
{
	int nRetCode = false;
	int nResult = false;
	time_t ttNow = 0;
	struct tm* tmNow;

	CONDITION_ERROR(pParam);

	memcpy(&m_Param, pParam, sizeof(m_Param));

	if (m_Param.Options == 0)
		m_Param.Options = eTinyLogOptionFile;

	m_Param.szPath[sizeof(m_Param.szPath) - 1] = '\0';

	CONDITION_SUCCESS(m_Param.Options != eTinyLogOptionFile);

	nRetCode = (int)strlen(m_Param.szPath);
	CONDITION_ERROR(nRetCode < FILE_PATH_LENGTH);

	if (m_Param.szPath[nRetCode - 1] == '\\')
		m_Param.szPath[nRetCode - 1] = '/';
	if (m_Param.szPath[nRetCode - 1] != '/')
	{
		m_Param.szPath[nRetCode] = '/';
		m_Param.szPath[nRetCode + 1] = '\0';
	}

	ttNow = time(NULL);
	tmNow = localtime(&ttNow);

	m_RTPattern.ttTimeStamp = ttNow;

	nRetCode = _snprintf(m_szFilePath, sizeof(m_szFilePath) - 1, "%s%d-%2.2d-%2.2d/"
		, m_Param.szPath, tmNow->tm_year + 1900, tmNow->tm_mon + 1, tmNow->tm_mday);
	m_szFilePath[sizeof(m_szFilePath) - 1] = '\0';

	nRetCode = _snprintf(m_szFileName, sizeof(m_szFileName) - 1, "%d_%2.2d_%2.2d_%2.2d_%2.2d_%2.2d",  
		tmNow->tm_year + 1900, tmNow->tm_mon + 1, tmNow->tm_mday, tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec);
	m_szFileName[sizeof(m_szFileName) - 1] = '\0';

	nRetCode = NextLogFile();
	CONDITION_ERROR(nRetCode);

Success:
	nResult = true;
Error:
	if (!nResult)
	{
		m_szFileName[0] = '\0';
		m_szFilePath[0] = '\0';
		m_RTPattern.ttTimeStamp = 0;
		memset(&m_Param, 0, sizeof(m_Param));
	}
	return nResult;
}

TINY_LOG_PRIORITY TinyLog::SetPriority(TINY_LOG_PRIORITY priNew)
{
	TINY_LOG_PRIORITY priOld = m_CurrentPriority;
	m_CurrentPriority = priNew;
	return priOld;
}

void TinyLog::Reset()
{
	if (m_atLogs)
	{
		atom_destroy(m_atLogs);
		m_atLogs = NULL;
	}

	if (m_atStrings)
	{
		atom_destroy(m_atStrings);
		m_atStrings = NULL;
	}

	m_LogMap.clear();
	m_StringMap.clear();

	m_atLogs = atom_create();
	assert(m_atLogs);

	m_atStrings = atom_create();
	assert(m_atStrings);

	if (m_nFileHandle != -1)
	{
		_close(m_nFileHandle);
		m_nFileHandle = -1;
	}

	m_usOffset = 0;
}

int TinyLog::AddString(const char* pcszStr, unsigned short* pusRet)
{
	int nRetCode = false;
	int nResult = false;
	c_str cstr = NULL;
	TLOffsetMapPtr ptr;

	CONDITION_ERROR(pusRet);
	*pusRet = 0;
	CONDITION_SUCCESS(pcszStr == NULL);

	cstr = atom_string(m_atStrings, pcszStr);

	ptr = m_StringMap.find(cstr);
	if (ptr != m_StringMap.end())
	{
		*pusRet = ptr->second;
		goto Success;
	}

	*pusRet = m_usOffset;
	m_StringMap.insert(std::make_pair(cstr, m_usOffset));

	nRetCode = _write(m_nFileHandle, pcszStr, (unsigned int)strlen(pcszStr) + 1);
	m_usOffset += nRetCode;

Success:
	nResult = true;
Error:
	return nResult;
}

int TinyLog::NextLogFile()
{
	int nRetCode = false;
	int nResult = false;
	char szFullPath[FILE_PATH_LENGTH];

	nRetCode = mkdir(m_Param.szPath);
	CONDITION_ERROR((nRetCode == 0) || (errno == EEXIST));

	nRetCode = mkdir(m_szFilePath);
	CONDITION_ERROR((nRetCode == 0) || (errno == EEXIST));

	Reset();

	nRetCode = _snprintf(szFullPath, sizeof(szFullPath) - 1, "%s%s(part%d).tlog"
		, m_szFilePath, m_szFileName, m_nIndex);
	szFullPath[sizeof(szFullPath) - 1] = '\0';

	m_nFileHandle = _open(szFullPath, TLOG_OPENFILEFLAG, TLOG_OPENFILEMODE);
	CONDITION_ERROR(m_nFileHandle != -1);

	nRetCode = _write(m_nFileHandle, &m_RTPattern, m_RTPattern.cbSize);
	CONDITION_ERROR(nRetCode == (int)m_RTPattern.cbSize);

	m_usOffset += (unsigned short)nRetCode;

	nResult = true;
	m_nIndex++;
Error:
	if (!nResult)
	{
		if (m_nFileHandle != -1)
		{
			_close(m_nFileHandle);
			m_nFileHandle = -1;
		}
	}
	return nResult;
}

#ifdef BUILD_AS_DLL
__declspec(dllexport)
#else
extern
#endif
eITLCode CreateTinyLog(TINY_LOG_PARAM* pParam, ITinyLog** ppi)
{
	if (!pParam || pParam->cbSize != sizeof(TINY_LOG_PARAM))
		return eCreateITLVersionError;

	if (!ppi)
		return eCreateITLError;

	TinyLog* pTinyLog = NULL;
	pTinyLog = new TinyLog();
	if (!pTinyLog)
		return eCreateITLError;

	if (!pTinyLog->Init(pParam))
		return eCreateITLInitError;

	*ppi = static_cast<ITinyLog*>(pTinyLog);

	return eCreateITLSuccess;
}

static ITinyLog* gs_TinyLogS = NULL;

TLAPI int logS_Init(TINY_LOG_PARAM* pParam)
{
	int nRetCode = false;
	int nResult = false;

	CONDITION_ERROR(gs_TinyLogS == NULL);

	nRetCode = CreateTinyLog(pParam, &gs_TinyLogS);
	CONDITION_ERROR(nRetCode == eCreateITLSuccess);

	nResult = true;
Error:
	return nResult;
}

TLAPI int logS_Uninit()
{
	int nResult = false;

	CONDITION_ERROR(gs_TinyLogS);

	gs_TinyLogS->Release();

	nResult = true;
Error:
	return nResult;
}

TLAPI int logS_Log(TINY_LOG_PRIORITY Priority, const char* csClassify, const char fmt[], ...)
{
	int nResult = false;
	char szBuffer[1024];
	va_list marker;

	CONDITION_ERROR(gs_TinyLogS);

	va_start(marker, fmt);
	vsnprintf(szBuffer, sizeof(szBuffer) - 1, fmt, marker);
	va_end(marker);

	szBuffer[sizeof(szBuffer) - 1] = '\0';

	nResult = gs_TinyLogS->Log(Priority, csClassify, NULL, -1, NULL, szBuffer);
Error:
	return nResult;
}

TLAPI int logS_Log(TINY_LOG_PRIORITY Priority
				   , const char* csClassify
				   , const char* pcszCondition			
				   , int nLine							
				   , const char* pcszFunctionName	
				   , const char* pcszInfo /* = NULL */)
{
	int nResult = false;

	CONDITION_ERROR(gs_TinyLogS);

	nResult = gs_TinyLogS->Log(Priority, csClassify, pcszCondition, nLine, pcszFunctionName, pcszInfo);
Error:
	return nResult;
}