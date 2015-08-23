#include "stdafx.h"
#include "log.h"
#include "test.h"

static void FunctionStackOne()
{
	int nRandomNumber = rand();
	LOG_ERROR(nRandomNumber % 2);
	LOG_SUCCESS(nRandomNumber % 2);

Error:
Success:
Exit0:
Exit1:
	return;
}

void Run()
{
	for (int i = 0; i < 100000; ++i)
		FunctionStackOne();
}