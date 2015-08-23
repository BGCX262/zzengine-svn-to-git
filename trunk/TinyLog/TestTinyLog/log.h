#pragma once

#include "../include/kingsoft/KGLog.h"
#include "../include/ITinyLog.h"

#include "log4cpp/Category.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/BasicLayout.hh"

int KLogInit();
int KLogUninit();

int TLogInit();
int TLogUninit();

int LLogInit();
int LLogUninit();
int LLogPrint(const char* pcszPri, const char* pcszCate, const char* pcszCondition, int nLine, const char* pcszFuncName);

#define LogInit		TLogInit
#define LogUninit	TLogUninit

#ifdef KINGSOFT
#define LogInit		KLogInit
#define LogUninit	KLogUninit
#endif

#ifdef LOG4CPP
#define LogInit LLogInit
#define LogUninit LLogUninit
#endif

#define LOG_ERROR	LOGS_CONDITION_ERROR
#define LOG_SUCCESS	LOGS_CONDITION_SUCCESS

#ifdef KINGSOFT
#define LOG_ERROR	KGLOG_PROCESS_ERROR
#define LOG_SUCCESS	KGLOG_PROCESS_SUCCESS
#endif

#define LLOG_CONDITION_ERROR(Condition)	\
	do	\
	{	\
		if (!(Condition))	\
		{	\
			LLogPrint(				\
				"Error",			\
				"Error",			\
				#Condition,			\
				__LINE__,			\
				_FUNCTION			\
			);						\
		goto Error;				\
		}	\
	} while (false)

#define LLOG_CONDITION_SUCCESS(Condition)	\
	do	\
	{	\
		if (Condition)	\
		{	\
			LLogPrint(				\
				"Info",	\
				"Info",	\
				#Condition,			\
				__LINE__,			\
				_FUNCTION			\
			);						\
			goto Success;			\
		}	\
	} while (false)

#ifdef LOG4CPP
#define LOG_ERROR	LLOG_CONDITION_ERROR
#define LOG_SUCCESS	LLOG_CONDITION_SUCCESS
#endif