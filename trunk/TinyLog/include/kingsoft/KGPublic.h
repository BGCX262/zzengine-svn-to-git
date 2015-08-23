//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSGPublic.h
//  Version     :   1.0
//  Creater     :   Freeway Chen
//  Date        :   2004-4-19 20:44:49
//  Comment     :   Kingsoft Game Public header file
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KGPUBLIC_H
#define _KGPUBLIC_H    1

#include <assert.h>
#include "KGError.h"

#ifndef ASSERT
#define ASSERT assert
#endif

#ifdef WIN32
#define KG_FUNCTION __FUNCTION__
#else
#define KG_FUNCTION __PRETTY_FUNCTION__
#endif

#ifdef _MSC_VER
    #define TEMP_DISABLE_WARNING(warningCode, expression)   \
        __pragma(warning(push))                             \
        __pragma(warning(disable:warningCode))              \
        expression                                          \
        __pragma(warning(pop))
#else
    #define TEMP_DISABLE_WARNING(warningCode, expression)   \
        expression
#endif 

// _MSC_VER warning C4127: conditional expression is constant
// use this macro instead "WHILE_FALSE_NO_WARNING", you can enjoy the level 4 warning ^_^
#define WHILE_FALSE_NO_WARNING  \
    TEMP_DISABLE_WARNING(4127, while (false))


#define KG_USE_ARGUMENT(arg) (arg)

#define KG_PROCESS_ERROR(Condition) \
    do  \
    {   \
        if (!(Condition))   \
            goto Exit0;     \
    } WHILE_FALSE_NO_WARNING


#define KG_PROCESS_SUCCESS(Condition) \
    do  \
    {   \
        if (Condition)      \
            goto Exit1;     \
    } WHILE_FALSE_NO_WARNING



#define KG_PROCESS_ERROR_RET_CODE(Condition, Code) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
            nResult = Code;     \
            goto Exit0;         \
        }                       \
    } WHILE_FALSE_NO_WARNING

#define KG_PROCESS_ERROR_RET_COM_CODE(Condition, Code) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
            hrResult = Code;     \
            goto Exit0;         \
        }                       \
    } WHILE_FALSE_NO_WARNING


#define KG_COM_PROCESS_ERROR(Condition) \
    do  \
    {   \
        if (FAILED(Condition))  \
            goto Exit0;         \
    } WHILE_FALSE_NO_WARNING


#define KG_COM_PROCESS_SUCCESS(Condition)   \
    do  \
    {   \
        if (SUCCEEDED(Condition))   \
            goto Exit1;             \
    } WHILE_FALSE_NO_WARNING


// KG_COM_PROCESS_ERROR_RETURN_ERROR
#define KG_COM_PROC_ERR_RET_ERR(Condition)  \
    do  \
    {   \
        if (FAILED(Condition))      \
        {                           \
            hrResult = Condition;   \
            goto Exit0;             \
        }   \
    } WHILE_FALSE_NO_WARNING


#define KG_COM_PROC_ERROR_RET_CODE(Condition, Code)     \
    do  \
    {   \
        if (FAILED(Condition))  \
        {                       \
            hrResult = Code;    \
            goto Exit0;         \
        }                       \
    } WHILE_FALSE_NO_WARNING


#define KG_COM_RELEASE(pInterface) \
    do  \
    {   \
        if (pInterface)                 \
        {                               \
            (pInterface)->Release();    \
            (pInterface) = NULL;        \
        }                               \
    } WHILE_FALSE_NO_WARNING


#define KG_DELETE_ARRAY(pArray)     \
    do  \
    {   \
        if (pArray)                 \
        {                           \
            delete [](pArray);      \
            (pArray) = NULL;        \
        }                           \
    } WHILE_FALSE_NO_WARNING


#define KG_DELETE(p)    \
    do  \
    {   \
        if (p)              \
        {                   \
            delete (p);     \
            (p) = NULL;     \
        }                   \
    } WHILE_FALSE_NO_WARNING

#define KG_CHECK_ERROR(Condition) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
        }                       \
    } WHILE_FALSE_NO_WARNING


#define KG_COM_CHECK_ERROR(Condition) \
    do  \
    {   \
        if (FAILED(Condition))       \
        {                       \
        }                       \
    } WHILE_FALSE_NO_WARNING

    

#ifdef _DEBUG

#define KG_ASSERT_EXIT(Condition) \
    do  \
    {   \
        assert(Condition);  \
        if (!(Condition))   \
            goto Exit0;     \
    } WHILE_FALSE_NO_WARNING

#else

#define KG_ASSERT_EXIT(Condition)  \
    do  \
    {   \
        if (!(Condition))       \
            goto Exit0;         \
    } WHILE_FALSE_NO_WARNING


#endif

#ifndef KG_HANDLE_DEFINED
#define KG_HANDLE_DEFINED

typedef void *KGHANDLE;    

#endif // KG_HANDLE_DEFINED

#ifdef __GNUC__
#include <stdint.h>
#endif 

#if _MSC_VER >= 1200        // >= VS 6.0

typedef __int8              int8_t;
typedef __int16             int16_t;
typedef __int32             int32_t;
typedef __int64             int64_t;

typedef unsigned __int8     uint8_t;
typedef unsigned __int16    uint16_t;
typedef unsigned __int32    uint32_t;
typedef unsigned __int64    uint64_t;

#if defined(_WIN64)
typedef __int64             intptr_t;
typedef unsigned __int64    uintptr_t;
#else
typedef _W64 __int32             intptr_t;
typedef _W64 unsigned __int32    uintptr_t;
#endif

#endif //_MSC_VER


#if _MSC_VER >= 1200            // >= VS 6.0
#   define KG_LITTLE_ENDIAN	    1234
#   define KG_BIG_ENDIAN	    4321
#   define KG_PDP_ENDIAN	    3412
#   define KG_BYTE_ORDER        KG_LITTLE_ENDIAN  
#else
#   include <endian.h>
#   define KG_LITTLE_ENDIAN     __LITTLE_ENDIAN
#   define KG_BIG_ENDIAN        __BIG_ENDIAN
#   define KG_PDP_ENDIAN        __PDP_ENDIAN
#   define KG_BYTE_ORDER        __BYTE_ORDER
#endif

#endif // _KGPUBLIC_H
