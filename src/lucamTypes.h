
#ifndef LUMENERA_LUCAMTYPES_H
#define LUMENERA_LUCAMTYPES_H


/***************************************************************************
* Some Windows types, for compatibility with the Lumenera API for Windows
***************************************************************************/

#if (_MSC_VER >= 1300)
#define LUCAM_DEPRECATED   __declspec(deprecated)
#else
#define LUCAM_DEPRECATED
#endif

#if defined(_WIN32)
#include <windows.h>
#ifdef LUCAMAPI_EXPORTS	// Only to be defined by Lumenera

#ifdef _WIN64
#ifdef __cplusplus
#define LUCAM_API extern "C" __declspec(dllexport)
#else
#define LUCAM_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define LUCAM_API extern "C" /*__declspec(dllexport)*/
#else
#define LUCAM_API /*__declspec(dllexport)*/
#endif
#endif

#else
#ifdef __cplusplus
#define LUCAM_API extern "C" __declspec(dllimport)
#else
#define LUCAM_API __declspec(dllimport)
#endif
#endif

#define LUCAM_EXPORT __stdcall


#elif defined(__linux__)

#ifdef __cplusplus
#define LUCAM_API extern "C" 
#else
#define LUCAM_API 
#endif

#define LUCAM_EXPORT

#define HWND void*


#if defined __x86_64__
typedef int                LONG;
typedef unsigned int       ULONG;
typedef unsigned int       DWORD;
#else
typedef long               LONG;
typedef unsigned long      ULONG;
typedef unsigned long      DWORD;
#endif
typedef short              SHORT;
typedef unsigned short     USHORT;
typedef unsigned short     WORD;
typedef int                BOOL;
typedef char               CHAR;
typedef unsigned char      UCHAR;
typedef unsigned char      BYTE;
typedef short              WCHAR;
typedef float              FLOAT;
typedef void               VOID, *PVOID;
typedef void *             HANDLE;
typedef const char *       LPCSTR;

#ifndef FALSE
#define FALSE                 0
#endif
#ifndef TRUE
#define TRUE                  1
#endif

#else
#error "Unsupported platform"
#endif


#endif
