// TestTinyLog.cpp : Defines the entry point for the console application.
//
//
#include "stdafx.h"
#include "stdio.h"
#include "log.h"
#include "test.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int nStart = GetTickCount();

	LogInit();
	Run();
	LogUninit();

	printf("%d\n", GetTickCount() - nStart);
	system("pause");
	return 0;
}

//int DoSomething(void* pvArg)
//{
//	return true;
//}
//
//int TinyLogCanonicalFunction(void* pvArg0, void* pvArg1, int* pnRet)
//{
//	int nRetCode = false;
//	int nResult = false;
//	void* pvBuffer = NULL;
//
//	LOGS_CONDITION_ERROR(pvArg0);
//	LOGS_CONDITION_ERROR(pvArg1);
//	LOGS_CONDITION_ERROR(pnRet);
//
//	nRetCode = DoSomething(pvArg0);
//	LOGS_CONDITION_ERROR(nRetCode);
//
//	pvBuffer = malloc(10);
//	LOGS_CONDITION_ERROR(pvBuffer);
//
//	*pnRet = 10;
//
//	nRetCode = DoSomething(pvArg1);
//	LOGS_CONDITION_SUCCESS(nRetCode);
//
//Success:
//	nResult = true;
//Error:
//	if (!nResult)
//	{
//		*pnRet = 0;
//	}
//	SAFE_FREE(pvBuffer);
//	return nResult;
//}

//#include "stdafx.h"
//#include <iostream>
//#include "log4cpp/Category.hh"
//#include "log4cpp/Appender.hh"
//#include "log4cpp/FileAppender.hh"
//#include "log4cpp/OstreamAppender.hh"
//#include "log4cpp/Layout.hh"
//#include "log4cpp/BasicLayout.hh"
//#include "log4cpp/Priority.hh"
//#include "log4cpp/NDC.hh"


//int main(int argc, char** argv) {
//	system("pause");
//	log4cpp::Layout* layout =   new log4cpp::BasicLayout(); 
//	// 2. ��ʼ��һ��appender ����   
//	log4cpp::Appender* appender = new log4cpp::FileAppender("FileAppender","./test_log4cpp1.log");  
//	// 3. ��layout��������appender������ 
//	appender->setLayout(layout);
//	// 4. ʵ����һ��category����
//	log4cpp::Category& warn_log = log4cpp::Category::getInstance("mywarn"); 
//	// 5. ����additivityΪfalse���滻���е�appender        
//	warn_log.setAdditivity(false);        
//	// 5. ��appender���󸽵�category��
//	warn_log.setAppender(appender);     
//	// 6. ����category�����ȼ������ڴ����ȼ�����־������¼        
//	warn_log.setPriority(log4cpp::Priority::WARN);     
//	// ��¼һЩ��־        
//	warn_log.info("Program info which cannot be wirten");  
//	warn_log.debug("This debug message will fail to write");
//	warn_log.alert("Alert info");  
//	// ������¼��־��ʽ     
//	warn_log.log(log4cpp::Priority::WARN, "This will be a logged warning");  
//	log4cpp::Priority::PriorityLevel priority;     
//	bool this_is_critical = true;      
//	if(this_is_critical)   
//		priority = log4cpp::Priority::CRIT;     
//	else              
//		priority = log4cpp::Priority::DEBUG;       
//	warn_log.log(priority,"Importance depends on context");    
//	warn_log.critStream() << "This will show up << as " << 1 << " critical message"; 
//	// clean up and flush all appenders 
//	log4cpp::Category::shutdown(); 
//
//	return 0;
//}