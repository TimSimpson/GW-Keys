#include <windows.h>

	extern "C"
	{

	// The following ifdef block is the standard way of creating macros which make exporting 
	// from a DLL simpler. All files within this DLL are compiled with the CAPSTOGWBASICASCIIDLL_EXPORTS
	// symbol defined on the command line. this symbol should not be defined on any project
	// that uses this DLL. This way any other project whose source files include this file see 
	// CAPSTOGWBASICASCIIDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
	// defined with this macro as being exported.
	#ifdef CAPSTOGWBASICASCIIDLL_EXPORTS
	#define CAPSTOGWBASICASCIIDLL_API __declspec(dllexport)
	#else
	#define CAPSTOGWBASICASCIIDLL_API __declspec(dllimport)
	#endif

	//typedef void (*DebugOutput)(const char * const msg);

	CAPSTOGWBASICASCIIDLL_API int GwKeys_Install();//unsigned long threadId, DebugOutput outputFunc);
	CAPSTOGWBASICASCIIDLL_API int GwKeys_Uninstall();

	//extern CAPSTOGWBASICASCIIDLL_API int nCapsToGWBasicAsciiDll;

	//CAPSTOGWBASICASCIIDLL_API int fnCapsToGWBasicAsciiDll(void);

	};