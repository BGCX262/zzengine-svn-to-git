/*
 *	Common Define for Engine
 *	2010-11-14 By AnIcepIg
 */
#ifndef ZENGINE_COMMON_DEFINE_H
#define ZENGINE_COMMON_DEFINE_H

#ifdef COMMON_BUILD_AS_DLL
	#define ZENGINE_API	__declspec(dllexport)
#else
	#define ZENGINE_API	extern
#endif

#define null		0
#define max_path	256

#define NELEMS(x)	(sizeof((x))/(sizeof(((x)[0]))))

typedef const char* c_str;

#endif	//ZENGINE_COMMON_DEFINE_H