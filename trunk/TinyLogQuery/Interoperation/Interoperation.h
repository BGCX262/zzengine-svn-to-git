#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

#define TINYLOG_ASSEMBLY	"TinyLog.dll"

namespace Interop
{

public ref class CLogInfo
{
public:
	System::String^ m_strTag;
	System::String^ m_strClassify;
	int m_nPriority;
	System::String^ m_strCondition;
	int m_nLine;
	System::String^ m_strFunction;
	System::String^ m_strInfo;
};

public ref class CTinyLog
{
public:
	CTinyLog(IETinyLog* piTinyLog);
	virtual ~CTinyLog();

	System::String^ GetName();
	void Release();

	System::String^ GetAllText();
	System::String^ GetText(CLogInfo^ pInfo);

private:
	IETinyLog* m_piTinyLog;
};

public ref class Interoperation
{
public:
	Interoperation();
	virtual ~Interoperation();

	CTinyLog^ LoadFromFile(System::String^ strFileName);
};

}