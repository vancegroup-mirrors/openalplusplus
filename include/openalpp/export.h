// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OPENALPP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OPENALPP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef __openalpp_export_h__
#define __openalpp_export_h__

#ifdef _WIN32

#pragma once
#include <windows.h>

#ifdef OPENALPP_EXPORTS
#define OPENALPP_API __declspec(dllexport)
#else
#define OPENALPP_API __declspec(dllimport)
#endif
#else
#define OPENALPP_API
#endif

// This class is exported from the openalpp.dll
//class OPENALPP_API Copenalpp {
//public:
//	Copenalpp(void);
//	// TODO: add your methods here.
//};

//extern OPENALPP_API int nopenalpp;

//OPENALPP_API int fnopenalpp(void);
#endif

