// Interoperation.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Interoperation.h"
#include <msclr/marshal.h>

using namespace msclr::interop;

namespace Interop
{
Interoperation::Interoperation():m_hTinyFS(NULL)
{
}

Interoperation::~Interoperation()
{

}

int Interoperation::LoadTinyFSAssemble(System::String^ strAssemble)
{
	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strAssemble);
	LPCSTR pcszAssemble = static_cast<LPCSTR>(intPtr.ToPointer());
	m_hTinyFS = LoadLibraryA(pcszAssemble);
	if (m_hTinyFS == NULL)
		return 0;
	return 1;
}

void Interoperation::UnloadTinyFSAssemble()
{
	if (m_hTinyFS)
	{
		FreeLibrary(m_hTinyFS);
		m_hTinyFS = NULL;
	}
}

TinyFS^ Interoperation::GetTinyFS(System::String^ strFile)
{
	if (strFile)
		return LoadTinyFS(strFile);
	else
		return NewTinyFS();
}

TinyFS^ Interoperation::LoadTinyFS(System::String^ strFile)
{
	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strFile);
	LPCSTR pcszFile = static_cast<LPCSTR>(intPtr.ToPointer());
	TinyFS^ pTinyFS = gcnew TinyFS();
	pTinyFS->LoadFromFile(pcszFile);
	return pTinyFS;
}

TinyFS^ Interoperation::NewTinyFS()
{
	TinyFS^ pTinyFS = gcnew TinyFS();
	return pTinyFS;
}


TinyFS::TinyFS():m_t(NULL)
	, m_bReady(false)
{
}

TinyFS::~TinyFS()
{}

int TinyFS::New()
{
	m_t = tfsE_Create("RLTools-tfs");
	if (m_t)
	{
		m_bReady = true;
		return 1;
	}
	return 0;
}

int TinyFS::LoadFromFile(LPCSTR pcszFile)
{
	if (!pcszFile)
		return 0;
	m_t = tfsE_Load(pcszFile);
	if (m_t)
	{
		m_bReady = true;
		return 1;
	}
	return 0;
}

List<System::String^>^ TinyFS::GetTableKeys(System::String^ strName)
{
	List<System::String^>^ items = gcnew List<System::String^ >();
	tfs_Table tab = NULL;
	tfs_t tfs = tfsE_GetTfs(m_t);
	if (strName)
	{
		IntPtr intPtr = Marshal::StringToHGlobalAnsi(strName);
		LPCSTR pcszName = static_cast<LPCSTR>(intPtr.ToPointer());
		tab = tfs_GetTable(tfs, pcszName);
	}
	else
		tab = tfs_GetTable(tfs, NULL);

	if (tab)
	{
		for (ulong i = 0; i < tab->count; ++i)
		{
			items->Add(marshal_as<System::String^>(tab->pn[i].k.s));
		}
	}
	return items;
}

System::String^ TinyFS::GetName()
{
	tfs_t tfs = tfsE_GetTfs(m_t);
	return marshal_as<System::String^>(tfs->name);
}

int TinyFS::GetValue(System::String^ strTab, System::String^ strName)
{
	tfs_Table tab = NULL;
	tfs_t tfs = tfsE_GetTfs(m_t);
	IntPtr intPtr;
	LPCSTR pcszName = NULL;

	if (strTab)
	{
		intPtr = Marshal::StringToHGlobalAnsi(strTab);
		pcszName = static_cast<LPCSTR>(intPtr.ToPointer());
		tab = tfs_GetTable(tfs, pcszName);
	}
	else
		tab = tfs_GetTable(tfs, NULL);
	if (!tab || !strName)
		return false;

	intPtr = Marshal::StringToHGlobalAnsi(strName);
	pcszName = static_cast<LPCSTR>(intPtr.ToPointer());
	tfs_Value tv = tfs_GetValue(tfs, tab, pcszName);
	return (int)tv;
}

System::String^ TinyFS::ToString(int nValue)
{
	return marshal_as<System::String^>(tfs_tostring(tfsE_GetTfs(m_t), nValue));
}

int TinyFS::SetNull(System::String^ strTab, System::String^ strKey)
{
	if (!strKey)
		return 0;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszName = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	return tfsE_SetNull(m_t, tab, pcszName);
}

void TinyFS::SetBool(System::String^ strTab, System::String^ strKey, bool bValue)
{
	if (!strKey)
		return;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszKey = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	tfsE_SetBool(m_t, tab, pcszKey, bValue);
}

void TinyFS::SetByte(System::String^ strTab, System::String^ strKey, byte bValue)
{
	if (!strKey)
		return;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszKey = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	tfsE_SetByte(m_t, tab, pcszKey, bValue);
}

void TinyFS::SetInt32(System::String^ strTab, System::String^ strKey, int nValue)
{
	if (!strKey)
		return;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszKey = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	tfsE_SetInt32(m_t, tab, pcszKey, nValue);
}

void TinyFS::SetInt64(System::String^ strTab, System::String^ strKey, int64 nValue)
{
	if (!strKey)
		return;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszKey = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	tfsE_SetInt64(m_t, tab, pcszKey, nValue);
}

void TinyFS::SetFloat(System::String^ strTab, System::String^ strKey, float fValue)
{
	if (!strKey)
		return;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszKey = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	tfsE_SetFloat(m_t, tab, pcszKey, fValue);
}

void TinyFS::SetDouble(System::String^ strTab, System::String^ strKey, double dValue)
{
	if (!strKey)
		return;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszKey = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	tfsE_SetDouble(m_t, tab, pcszKey, dValue);
}

void TinyFS::SetTable(System::String^ strTab, System::String^ strKey)
{
	if (!strKey)
		return;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszKey = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	tfsE_SetTable(m_t, tab, pcszKey);
}

void TinyFS::SetString(System::String^ strTab, System::String^ strKey, System::String^ strValue)
{
	if (!strKey || !strValue)
		return;

	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strKey);
	LPCSTR pcszKey = static_cast<LPCSTR>(intPtr.ToPointer());
	intPtr = Marshal::StringToHGlobalAnsi(strValue);
	LPCSTR pcszValue = static_cast<LPCSTR>(intPtr.ToPointer());

	tfs_Table tab = GetTable(strTab);
	tfsE_SetString(m_t, tab, pcszKey, pcszValue);
}

tfs_Table TinyFS::GetTable(System::String^ strName)
{
	tfs_Table tab = NULL;
	tfs_t tfs = tfsE_GetTfs(m_t);
	if (strName)
	{
		IntPtr intPtr = Marshal::StringToHGlobalAnsi(strName);
		LPCSTR pcszName = static_cast<LPCSTR>(intPtr.ToPointer());
		tab = tfs_GetTable(tfs, pcszName);
	}
	else
		tab = tfs_GetTable(tfs, NULL);
	return tab;
}

bool TinyFS::Save(System::String^ strFile)
{
	if (!strFile)
		return false;
	IntPtr intPtr = Marshal::StringToHGlobalAnsi(strFile);
	LPCSTR pcszFile = static_cast<LPCSTR>(intPtr.ToPointer());
	LPCSTR pszRet = tfsE_Save(pcszFile, m_t);
	return pszRet != NULL;
}

}