#include "stdafx.h"
#include "..\TinyFS\tapi.h"


int _tmain(int argc, _TCHAR* argv[])
{
	tfsE_t tfs = tfsE_Create("Person");
	/*tfs_Table tab = tfsE_GetTable(tfs, NULL);
	tfs_t fs;
	tfs_Value v;
	v = tfsE_SetString(tfs, tab, "Name", "Lilei");
	v = tfsE_SetFloat(tfs, tab, "Height", 175.5);
	v = tfsE_SetFloat(tfs, tab, "Weight", 65.2);
	v = tfsE_SetString(tfs, tab, "Nation", "Chinese");
	v = tfsE_SetBool(tfs, tab, "Sex", true);
	v = tfsE_SetTable(tfs, tab, "Father");
	fs = tfsE_GetTfs(tfs);
	tfs_Table father = tfs_totable(fs, v);
	v = tfsE_SetString(tfs, father, "Name", "LiGang");
	v = tfsE_SetString(tfs, father, "Sex", "Man");
	int n = tfsE_SetNull(tfs, father, "Sex");
	v = tfsE_SetInt32(tfs, father, "Age", 48);
	n = tfsE_SetNull(tfs, father, "Age");
	v = tfsE_SetTable(tfs, tab, "Mother");
	tfs_Table mother = tfs_totable(fs, v);
	v = tfsE_SetString(tfs, mother, "Name", "HanMeimei");*/
	tfsE_Save(".\\new000.tfs", tfs);
	/*tfs_t tfs = NULL;
	tfs_Value tv = 0;
	tfs_Table tab = NULL;
	tfs = tfs_Load(".\\test.tfs");
	tv = tfs_GetValue(tfs, NULL, "Name");
	if (tfs_isstring(tv))
		printf("Name=%s\n", tfs_tostring(tfs, tv));
	tv = tfs_GetValue(tfs, NULL, "Sex");
	if (tfs_isbool(tv))
		printf("Sex=%s\n", tfs_tobool(tfs, tv)?"man":"women");
	tv = tfs_GetValue(tfs, NULL, "Height");
	if (tfs_isfloat(tv))
		printf("Height=%.3f\n", tfs_tofloat(tfs, tv));
	tv = tfs_GetValue(tfs, NULL, "Weight");
	if (tfs_isfloat(tv))
		printf("Weight=%.3lf\n", tfs_tofloat(tfs, tv));
	tv = tfs_GetValue(tfs, NULL, "Father");
	if (tfs_istable(tv)) {
		tab = tfs_totable(tfs, tv);
		tv = tfs_GetValue(tfs, tab, "Name");
		if (tfs_isstring(tv))
			printf("Father=%s\n", tfs_tostring(tfs, tv));
		tv = tfs_GetValue(tfs, tab, "Age");
		if (tfs_isint32(tv))
			printf("Age=%d\n", tfs_toint32(tfs, tv));
	}

	tfsE_t pfse = tfsE_From(tfs);

	tfs_Unload(tfs);*/
	return 0;
}

