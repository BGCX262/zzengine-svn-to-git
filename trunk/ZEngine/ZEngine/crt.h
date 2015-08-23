/*
 *	c runtime
 *	2010-11-21 by AnIcepIg
 */
#ifndef _ZENGINE_CTR_H
#define _ZENGINE_CTR_H

#if (defined(_MSC_VER) || defined(__ICL))
#define _mkdir	mkdir
#else
#define _mkdir(x) mkdir((x), 0777)
#endif

#endif	//_ZENGINE_CTR_H