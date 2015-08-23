#include "stdafx.h"
#include "log.h"

int KLogInit()
{
	int nRetCode = false;
	int nResult = false;
	KGLOG_PARAM param = { 0 };

	nRetCode = sprintf_s(param.szPath, _countof(param.szPath), "%s", "Logs");
	KG_PROCESS_ERROR(nRetCode > 0);

	nRetCode = sprintf_s(param.szIdent, _countof(param.szIdent), "%s", "Kingsoft");
	KG_PROCESS_ERROR(nRetCode > 0);

	param.nMaxLineEachFile = 1 << 16;
	param.Options = (KGLOG_OPTIONS)KGLOG_OPTION_FILE;

	nRetCode = KGLogInit(param, NULL);
	KG_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	return nResult;
}

int KLogUninit()
{
	int nRetCode = false;
	int nResult = false;

	nRetCode = KGLogUnInit(NULL);
	KG_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	return nResult;
}

int TLogInit()
{
	int nRetCode = false;
	int nResult = false;
	TINY_LOG_PARAM param = { sizeof(TINY_LOG_PARAM) };

	nRetCode = sprintf_s(param.szPath, _countof(param.szPath), "%s/%s", "Logs", "TinyLog");
	CONDITION_ERROR(nRetCode > 0);

	param.Options = eTinyLogOptionFile;

	nRetCode = logS_Init(&param);
	CONDITION_ERROR(nRetCode);

	nResult = true;
Error:
	return nResult;
}

int TLogUninit()
{
	int nRetCode = false;
	int nResult = false;

	nRetCode = logS_Uninit();
	CONDITION_ERROR(nRetCode);

	nResult = true;
Error:
	return nResult;
}

int LLogInit()
{
	log4cpp::Layout* layout = new log4cpp::BasicLayout();
	log4cpp::Appender* pFile = new log4cpp::FileAppender("FileAppender", "./testlog.llog");
	log4cpp::Category& dLog = log4cpp::Category::getRoot().getInstance("Error");
	dLog.addAppender(pFile);
	log4cpp::Category& iLog = log4cpp::Category::getRoot().getInstance("Info");
	iLog.addAppender(pFile);
	return true;
}

int LLogUninit()
{
	//log4cpp::Category::shutdown();
	return true;
}

int LLogPrint(const char* pcszPri, const char* pcszCate, const char* pcszCondition, int nLine, const char* pcszFuncName)
{
	char szInfo[256];
	sprintf_s(szInfo, _countof(szInfo), "%s at Line %d in %s.", pcszCondition, nLine, pcszFuncName);
	log4cpp::Category& log = log4cpp::Category::getRoot().getInstance(pcszCate);
	if (pcszPri == "Error")
		log.setPriority(log4cpp::Priority::ERROR);
	else
		log.setPriority(log4cpp::Priority::INFO);
	log.error(szInfo);
	return true;
}