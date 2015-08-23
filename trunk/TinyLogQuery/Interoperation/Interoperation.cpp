// Interoperation.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Interoperation.h"
#include <msclr/marshal.h>

using namespace msclr::interop;

namespace Interop
{

CTinyLog::CTinyLog(IETinyLog* piTinyLog)
	:m_piTinyLog(NULL)
{
	m_piTinyLog = piTinyLog;
}

CTinyLog::~CTinyLog()
{
	if (m_piTinyLog)
	{
		m_piTinyLog->Release();
		m_piTinyLog = NULL;
	}
}

System::String^ CTinyLog::GetName()
{
	ETL_FILE_INFO eInfo;
	m_piTinyLog->GetFileInfo(&eInfo, sizeof(ETL_FILE_INFO));
	return marshal_as<System::String^>(eInfo.szFileName);
}

void CTinyLog::Release()
{
	if (m_piTinyLog)
	{
		m_piTinyLog->Release();
		m_piTinyLog = NULL;
	}
}

System::String^ CTinyLog::GetAllText()
{
	LPCSTR pcszText = NULL;
	System::String^ str = gcnew System::String("");

	ETL_LOG** pLogs = NULL;
	int nCount = 0;

	nCount = m_piTinyLog->SelectAll(NULL, -1);
	if (nCount == 0)
		return str;

	pLogs = (ETL_LOG**)malloc(sizeof(ETL_LOG*) * nCount);
	m_piTinyLog->SelectAll(pLogs, nCount);

	for (int i = 0; i < nCount; ++i)
	{
		ETL_LOG* pLog = pLogs[i];
		pcszText = m_piTinyLog->ToString(pLog);
		str += marshal_as<System::String^>(pcszText);
	}

	return str;
}

System::String^ CTinyLog::GetText(CLogInfo^ pInfo)
{
	LPCSTR pcszText = NULL;
	return marshal_as<System::String^>("");
}

Interoperation::Interoperation()
{

}

Interoperation::~Interoperation()
{

}

CTinyLog^ Interoperation::LoadFromFile(System::String^ strFileName)
{
	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strFileName);
	LPCSTR pcszFileName = static_cast<LPCSTR>(intPtr.ToPointer());

	IETinyLog* piTinyLog = NULL;
	ETINY_LOG_PARAM param = { sizeof(ETINY_LOG_PARAM) };
	
	CreateETinyLog(&param, &piTinyLog);

	CTinyLog^ cTinyLog = gcnew CTinyLog(piTinyLog);
	piTinyLog->LoadFromFile(pcszFileName);

	return cTinyLog;
}

}


