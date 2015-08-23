#include "stdafx.h"
#include "TinyLogE.h"

static char s_PriorityName[][16] = {
	"",
	"",
	"",
	"Error",
	"",
	"Warning",
	"",
	"Info",
	"",
	"Debug",
	"",
};

static bool IsEqual(ETL_LOG* pLog, ETL_LOG* pKey)
{
	bool bRet = false;
	
	if (pKey->BeginTag[0] != -1)
	{
		bRet = pLog->BeginTag[0] == pKey->BeginTag[0] &&
			pLog->BeginTag[1] == pKey->BeginTag[1] &&
			pLog->BeginTag[2] == pKey->BeginTag[2];
		if (!bRet)
			return bRet;
	}

	if ((INT_PTR)pKey->pcszClassify != -1 && pLog->pcszClassify)
	{
		bRet = strcmp(pKey->pcszClassify, pLog->pcszClassify) == 0;
		if (!bRet)
			return bRet;
	}

	if (pKey->Pri != -1)
	{
		bRet = pKey->Pri == pLog->Pri;
		if (!bRet)
			return bRet;
	}

	if ((INT_PTR)pKey->pcszCondition != -1 && pLog->pcszCondition)
	{
		bRet = strcmp(pKey->pcszCondition, pLog->pcszCondition) == 0;
		if (!bRet)
			return bRet;
	}

	if (pKey->nLine != -1)
	{
		bRet = pKey->nLine == pLog->nLine;
		if (!bRet)
			return bRet;
	}

	if ((INT_PTR)pKey->pcszFunctionName != -1 && pLog->pcszFunctionName)
	{
		bRet = strcmp(pKey->pcszFunctionName, pLog->pcszFunctionName) == 0;
		if (!bRet)
			return bRet;
	}

	if ((INT_PTR)pKey->pcszInfo != -1 && pLog->pcszInfo)
	{
		bRet = strcmp(pKey->pcszInfo, pLog->pcszInfo) == 0;
		if (!bRet)
			return bRet;
	}

	return true;
}

TinyLogE::TinyLogE()
{
	Reset();
}

TinyLogE::~TinyLogE()
{
	Reset();
}

int TinyLogE::Release()
{
	delete this;
	return true;
}

int TinyLogE::LoadFromFile(const char* pcszFilePath)
{
	int nRetCode = false;
	int nResult = false;
	int nFile = 0;

	CONDITION_ERROR(pcszFilePath);

	nRetCode = IsFileExist(pcszFilePath);
	CONDITION_ERROR(nRetCode);

	nRetCode = GetFileInfo(pcszFilePath);
	CONDITION_ERROR(nRetCode);

	while (true)
	{
		nFile = NextFile(nFile);
		CONDITION_ERROR(nFile != 0);

		if (nFile == -1)
			break;

		m_FileInfo.nFileCount++;
	}

	nResult = true;
Error:
	return nResult;
}

int TinyLogE::LoadFromAssemble(const char* pcszName)
{
	int nRetCode = false;
	int nResult = false;

	CONDITION_ERROR(pcszName);

	nResult = true;
Error:
	return nResult;
}

int TinyLogE::SelectAll(ETL_LOG** pLogs, int nCount)
{
	if (nCount == -1)
	{
		ETL_LOG LogKey;
		memset(&LogKey, -1, sizeof(LogKey));
		return GetItemCount(&LogKey);
	}

	int nRetCode = false;
	int nResult = false;
	int nIndex = 0;
	ETL_LOG** pLog = NULL;

	CONDITION_ERROR(pLogs);
	CONDITION_ERROR(nCount > 0);

	pLog = pLogs;
	for (LogVectorPtr ptr = m_Logs.begin(); ptr != m_Logs.end(); ++ptr, ++nIndex)
	{
		if (nIndex >= nCount)
			break;

		*pLog = &(*ptr);
		pLog++;
	}

	nResult = true;
Error:
	return nResult;
}

int TinyLogE::SelectItems(ETL_LOG* pKey, ETL_LOG** pLogs, int nCount)
{
	if (pKey && nCount == -1)
		return GetItemCount(pKey);

	int nRetCode = false;
	int nResult = false;
	int nIndex = 0;
	ETL_LOG** pLog = NULL;

	CONDITION_ERROR(pKey);
	CONDITION_ERROR(pLogs);
	CONDITION_ERROR(nCount > 0);

	pLog = pLogs;
	for (LogVectorPtr ptr = m_Logs.begin(); ptr != m_Logs.end(); ++ptr)
	{
		if (nIndex >= nCount)
			break;

		if (IsEqual(&(*ptr), pKey))
		{
			*pLog = &(*ptr);
			pLog++;
			nIndex++;
		}
	}

	nResult = true;
Error:
	return nResult;
}

int TinyLogE::GetFileInfo(ETL_FILE_INFO* pInfo, size_t uSize)
{
	int nResult = false;

	CONDITION_ERROR(pInfo);
	CONDITION_ERROR(uSize == sizeof(ETL_FILE_INFO));

	memcpy(pInfo, &m_FileInfo, uSize);

	nResult = true;
Error:
	return nResult;
}

const char* TinyLogE::ToString(ETL_LOG* pLog)
{
	int nRetCode = false;
	int nResult = false;

	CONDITION_ERROR(pLog);

	m_szStr[0] = '\0';

	_snprintf(m_szStr, sizeof(m_szStr) - 1,
		"[%d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d][%s] %s: Condition(%s) at line(%d) in function(%s). Info: %s.\r\n",
		pLog->Time.nYear, pLog->Time.nMonth, pLog->Time.nDay,
		pLog->Time.nHour, pLog->Time.nMinuter, pLog->Time.nSecond,
		pLog->pcszClassify, s_PriorityName[pLog->Pri],
		pLog->pcszCondition, pLog->nLine,
		pLog->pcszFunctionName, pLog->pcszInfo);

	nResult = true;
Error:
	return m_szStr;
}

int TinyLogE::Reset()
{
	int nRetCode = false;
	int nResult = false;

	m_Logs.clear();

	for (BufferVectorPtr ptr = m_Buffers.begin(); ptr != m_Buffers.end(); ++ptr)
	{
		SAFE_FREE(*ptr);
	}
	m_Buffers.clear();

	nResult = true;
//Error:
	return nResult;
}

int TinyLogE::GetItemCount(ETL_LOG* pKey)
{
	int nRetCode = false;
	int nResult = false;
	int nCount = 0;

	CONDITION_ERROR(pKey);

	for (LogVectorPtr ptr = m_Logs.begin(); ptr != m_Logs.end(); ++ptr)
	{
		ETL_LOG* pLog = &(*ptr);
		if (IsEqual(pLog, pKey))
			nCount++;
	}

	nResult = true;
Error:
	return nCount;
}

int TinyLogE::GetFileInfo(const char* pcszFile)
{
	int nRetCode = false;
	int nResult = false;
	int nIndex = 0;
	char szFile[FILE_PATH_LENGTH];

	m_FileInfo.szFileName[0] = '\0';
	m_FileInfo.szFilePath[0] = '\0';
	m_FileInfo.nFileCount = 0;
	m_FileInfo.szTag[0] = '\0';
	m_FileInfo.szVersion[0] = '\0';
	m_FileInfo.ttTime = 0;

	nIndex = (int)strlen(pcszFile) - 1;
	CONDITION_ERROR(nIndex > (int)strlen(".tlog") + (int)strlen("1900_01_01_01_01_01(part)"));

	strcpy_s(szFile, sizeof(szFile), pcszFile);

	for (; nIndex > 0; --nIndex)
	{
		if (szFile[nIndex] == '(')
			szFile[nIndex] = '\0';
		if (szFile[nIndex] == '\\' || szFile[nIndex] == '/')
		{
			szFile[nIndex] = '\0';
			++nIndex;
			break;
		}
	}

	strcpy_s(m_FileInfo.szFileName, sizeof(m_FileInfo.szFileName), &szFile[nIndex]);
	strcpy_s(m_FileInfo.szFilePath, sizeof(m_FileInfo.szFilePath), szFile);

	nResult = true;
Error:
	return nResult;
}

int TinyLogE::NextFile(int nFile)
{
	int nRetCode = false;
	int nResult = false;
	char szFile[FILE_PATH_LENGTH];
	FILE* pFile = NULL;
	size_t uFileSize = 0;
	TL_RUNTIME_PATTERN* pTL = NULL;
	void* pBuffer = NULL;

	_snprintf(szFile, sizeof(szFile) - 1, "%s/%s(part%d).tlog"
		, m_FileInfo.szFilePath, m_FileInfo.szFileName, nFile);
	szFile[sizeof(szFile) - 1] = '\0';

	nRetCode = IsFileExist(szFile);
	if (!nRetCode)
	{
		nResult = -1;
		goto Error;
	}

	pFile = fopen(szFile, "rb");
	CONDITION_ERROR(pFile);

	uFileSize = GetFileSize(pFile);
	CONDITION_ERROR(uFileSize > sizeof(TL_RUNTIME_PATTERN));

	pBuffer = malloc(uFileSize);
	CONDITION_ERROR(pBuffer);
	m_Buffers.push_back(pBuffer);

	nRetCode = (int)fread(pBuffer, 1, uFileSize, pFile);
	CONDITION_ERROR(nRetCode == uFileSize);

	pTL = (TL_RUNTIME_PATTERN*)pBuffer;
	CONDITION_ERROR(pTL->fccTag == TLOG_TAG);

	*(FOURCC*)(LONG_PTR)m_FileInfo.szTag = pTL->fccTag;
	m_FileInfo.szTag[sizeof(FOURCC)] = '\0';

	*(FOURCC*)(LONG_PTR)m_FileInfo.szVersion = pTL->fccVersion;
	m_FileInfo.szVersion[sizeof(FOURCC)] = '\0';

	m_FileInfo.ttTime = pTL->ttTimeStamp;

	nRetCode = AppendLogs((char*)pBuffer, (int)uFileSize);
	CONDITION_ERROR(nRetCode);

	nResult = nFile + 1;
Error:
	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	return nResult;
}

int TinyLogE::AppendLogs(char* pBuffer, int nSize)
{
	int nRetCode = false;
	int nResult = false;
	int nIndex = 0;

	CONDITION_ERROR(pBuffer);

	while (nIndex < nSize - 3)
	{
		if (pBuffer[nIndex] == 0xF && pBuffer[nIndex + 1] == 0xF)
		{
			ETL_LOG eLog;
			TL_LOG_INFO* pLog = (TL_LOG_INFO*)(&pBuffer[nIndex]);
			time_t tt = m_FileInfo.ttTime;
			struct tm* ptm;
			unsigned short usOffset;

			memset(&eLog, 0, sizeof(eLog));

			eLog.BeginTag[0] = pLog->BeginTag[0];
			eLog.BeginTag[1] = pLog->BeginTag[1];
			eLog.BeginTag[2] = pLog->BeginTag[2];

			tt += *(int*)(pLog->oftSecond);
			ptm = localtime(&tt);
			eLog.Time.nYear = ptm->tm_year + 1900;
			eLog.Time.nMonth = ptm->tm_mon + 1;
			eLog.Time.nDay = ptm->tm_mday;
			eLog.Time.nHour = ptm->tm_hour;
			eLog.Time.nMinuter = ptm->tm_min;
			eLog.Time.nSecond = ptm->tm_sec;

			if (pLog->BeginTag[2] == 0xF)
			{
				usOffset = *(unsigned short*)pLog->oftClassify;
				CONDITION_ERROR(usOffset < nSize);

				pLog = (TL_LOG_INFO*)(pBuffer + usOffset);
			}

			usOffset = *(unsigned short*)pLog->oftClassify;
			if (usOffset)
				eLog.pcszClassify = (const char*)(pBuffer + usOffset);
			
			eLog.Pri = (TINY_LOG_PRIORITY)pLog->idxPriority;
			
			usOffset = *(unsigned short*)pLog->oftCondition;
			if (usOffset)
				eLog.pcszCondition = (const char*)(pBuffer + usOffset);

			eLog.nLine = *(int*)(pLog->nLine);

			usOffset = *(unsigned short*)pLog->oftFunctionName;
			if (usOffset)
				eLog.pcszFunctionName = (const char*)(pBuffer + usOffset);

			usOffset = *(unsigned short*)pLog->oftInfo;
			if (usOffset)
				eLog.pcszInfo = (const char*)(pBuffer + usOffset);

			m_Logs.push_back(eLog);

			nIndex += 2;
		}

		++nIndex;
	}

	nResult = true;
Error:
	return nResult;
}

TLAPI eIETLCode CreateETinyLog(ETINY_LOG_PARAM* pParam, IETinyLog** ppi)
{
	eIETLCode eRet = eCreateIETLError;
	TinyLogE* pTinyLogE = NULL;

	CONDITION_ERROR(pParam);
	CONDITION_ERROR(ppi);
	CONDITION_ERROR(pParam->cbSize == sizeof(ETINY_LOG_PARAM));

	pTinyLogE = new TinyLogE();
	CONDITION_ERROR(pTinyLogE);

	*ppi = static_cast<IETinyLog*>(pTinyLogE);

Error:
	return eRet;
}