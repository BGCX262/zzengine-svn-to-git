#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

#define TINYFS_DLL	"TinyFS.dll"

namespace Interop
{

public ref class TinyFS
{
public:
	TinyFS();
	virtual ~TinyFS();

	int LoadFromFile(LPCSTR pcszFile);
	int New();
	List<System::String^>^ GetTableKeys(System::String^ strName);
	int GetValue(System::String^ strTab, System::String^ strName);
	System::String^ GetName();

	bool IsTable(int nValue) { return tfs_istable(nValue); }
	bool IsBool(int nValue) { return tfs_isbool(nValue); }
	bool IsByte(int nValue) { return tfs_isbyte(nValue); }
	bool IsInt32(int nValue) { return tfs_isint32(nValue); }
	bool IsFloat(int nValue) { return tfs_isfloat(nValue); }
	bool IsDouble(int nValue) { return tfs_isdouble(nValue); }
	bool IsInt64(int nValue) { return tfs_isint64(nValue); }
	bool IsString(int nValue) { return tfs_isstring(nValue); }

	bool ToBool(int nValue) { return tfs_tobool(tfsE_GetTfs(m_t), nValue); }
	byte ToByte(int nValue) { return tfs_tobyte(tfsE_GetTfs(m_t), nValue); }
	int ToInt32(int nValue) { return tfs_toint32(tfsE_GetTfs(m_t), nValue); }
	int64 ToInt64(int nValue) { return tfs_toint64(tfsE_GetTfs(m_t), nValue); }
	float ToFloat(int nValue) { return tfs_tofloat(tfsE_GetTfs(m_t), nValue); }
	double ToDouble(int nValue) { return tfs_todouble(tfsE_GetTfs(m_t), nValue); }
	System::String^ ToString(int nValue);

	int SetNull(System::String^ strTab, System::String^ strKey);
	void SetBool(System::String^ strTab, System::String^ strKey, bool bValue);
	void SetByte(System::String^ strTab, System::String^ strKey, byte bValue);
	void SetInt32(System::String^ strTab, System::String^ strKey, int nValue);
	void SetInt64(System::String^ strTab, System::String^ strKey, int64 nValue);
	void SetFloat(System::String^ strTab, System::String^ strKey, float fValue);
	void SetDouble(System::String^ strTab, System::String^ strKey, double dValue);
	void SetString(System::String^ strTab, System::String^ strKey, System::String^ strValue);
	void SetTable(System::String^ strTab, System::String^ strKey);

	bool Save(System::String^ strFile);
public:
	bool m_bReady;

private:
	tfsE_t m_t;

private:
	tfs_Table GetTable(System::String^ strName);
};

public ref class Interoperation
{
public:
	Interoperation();
	virtual ~Interoperation();

public:
	int LoadTinyFSAssemble(System::String^ strAssemble);
	void UnloadTinyFSAssemble();

	TinyFS^ GetTinyFS(System::String^ strFile);

private:
	HMODULE m_hTinyFS;

private:
	TinyFS^ NewTinyFS();
	TinyFS^ LoadTinyFS(System::String^ strFile);
};

}