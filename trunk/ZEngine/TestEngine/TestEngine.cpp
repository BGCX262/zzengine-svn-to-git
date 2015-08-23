// TestEngine.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdio.h"
#include "..\ZEngine\atom.h"

int _tmain(int argc, _TCHAR* argv[])
{
	atomG_init();
	c_str ai = atomG_new("1234", 4);
	c_str as = atomG_new("test", 4);
	atomG_string("test string");
	atomG_string("test");
	atomG_int(-1);
	atomG_int(1234);

	char sz[128];
	while (gets(sz)) 
	{
		if (atomG_string(sz) == ai)
			printf("string is 1234\n");
		else
			printf("new string %s\n", sz);
	}
	atomG_uninit();
	return 0;
}

