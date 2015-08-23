#ifndef _TINY_LOG_BASE_H
#define _TINY_LOG_BASE_H

#ifdef __GNUC__
	#define		TLAPI
	#define		C_TLAPI extern "C"
#else
	#ifdef BUILD_AS_DLL
		#define TLAPI __declspec(dllexport)
		#define C_TLAPI extern "C" __declspec(dllexport)
	#else
		#define TLAPI __declspec(dllimport)
		#define C_TLAPI extern "C" __declspec(dllimport)
	#endif
#endif

#define CONDITION_ERROR(Condition)	\
	do	\
	{	\
		if (!(Condition))	\
			goto Error;		\
	}while(false)

#define CONDITION_SUCCESS(Condition)	\
	do	\
	{	\
		if (Condition)		\
			goto Success;	\
	}while(false)

#ifndef SAFE_FREE
#define SAFE_FREE(a)	if (a) { free(a); (a) = null; }
#endif	//SAFE_FREE

#ifndef SAFE_DELETE
#define SAFE_DELETE(a)	if (a) { delete (a); (a) = null; }
#endif	//SAFE_DELETE

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(a)	if (a) { (a)->Release(); (a) = null; }
#endif	//SAFE_RELEASE

#endif	//_TINY_LOG_BASE_H