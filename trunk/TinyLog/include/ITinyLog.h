#ifndef _I_TINY_LOG_H
#define _I_TINY_LOG_H

#include "TLBase.h"
#include "TLDefine.h"

struct ITinyLog 
{
	virtual int Release() = 0;

	virtual int Log(TINY_LOG_PRIORITY Priority, const char* csClassify, const char fmt[], ...) = 0;
	virtual int Log(TINY_LOG_PRIORITY Priority
		, const char* csClassify			/* classify for this message */
		, const char* pcszCondition			/* condition for logging */
		, int nLine							/* code line number */
		, const char* pcszFunctionName		/* function name which for logging*/
		, const char* pcszInfo = NULL) = 0;	/* additional infomation */

	virtual TINY_LOG_PRIORITY SetPriority(TINY_LOG_PRIORITY priNew) = 0;
};

#ifndef PATH_COUNT
#define PATH_COUNT	256
#endif

struct TINY_LOG_PARAM
{
	unsigned cbSize;

	char szPath[PATH_COUNT];
	TINY_LOG_OPTION Options;

	int nVersionCreate;
};

enum eITLCode
{
	eCreateITLSuccess = 0,
	eCreateITLVersionError,
	eCreateITLError,
	eCreateITLInitError,
};

TLAPI eITLCode CreateTinyLog(TINY_LOG_PARAM* pParam, ITinyLog** ppi);

#ifdef WIN32
#define _FUNCTION __FUNCTION__
#else
#define _FUNCTION __PRETTY_FUNCTION__
#endif

#define LOG_CONDITION_ERROR(ptl, Condition)	\
	do	\
	{	\
		if (!(Condition) && (ptl))	\
		{	\
			ptl->Log(				\
				eTinyLogPriorityError,	\
				GLOBAL_CLASSIFY_NAME,	\
				#Condition,			\
				__LINE__,			\
				_FUNCTION			\
			);						\
			goto Error;				\
		}	\
	} while (false)

#define LOG_CONDITION_SUCCESS(ptl, Condition)	\
	do	\
	{	\
		if (Condition && (ptl))	\
		{	\
			ptl->Log(				\
				eTinyLogPriorityInfo,	\
				GLOBAL_CLASSIFY_NAME,	\
				#Condition,			\
				__LINE__,			\
				_FUNCTION			\
			);						\
			goto Success;			\
		}	\
	} while (false)

TLAPI int logS_Init(TINY_LOG_PARAM* pParam);
TLAPI int logS_Uninit();

TLAPI int logS_Log(TINY_LOG_PRIORITY Priority, const char* csClassify, const char fmt[], ...);
TLAPI int logS_Log(TINY_LOG_PRIORITY Priority
			 , const char* csClassify
			 , const char* pcszCondition			
			 , int nLine							
			 , const char* pcszFunctionName	
			 , const char* pcszInfo = NULL);

#define LOGS_CONDITION_ERROR(Condition)	\
	do	\
	{	\
		if (!(Condition))	\
		{	\
			logS_Log(				\
				eTinyLogPriorityError,	\
				GLOBAL_CLASSIFY_NAME,	\
				#Condition,			\
				__LINE__,			\
				_FUNCTION			\
			);						\
			goto Error;				\
		}	\
	} while (false)

#define LOGS_CONDITION_SUCCESS(Condition)	\
	do	\
	{	\
		if (Condition)	\
		{	\
			logS_Log(				\
				eTinyLogPriorityInfo,	\
				GLOBAL_CLASSIFY_NAME,	\
				#Condition,			\
				__LINE__,			\
				_FUNCTION			\
			);						\
			goto Success;			\
		}	\
	} while (false)

#endif	//_I_TINY_LOG_H