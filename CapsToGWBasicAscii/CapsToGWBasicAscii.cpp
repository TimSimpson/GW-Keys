// CapsToGWBasicAscii.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <tchar.h>

#define CONSOLE_TITLE_MAX_SIZE 512

HWND findConsoleHWnd()
{	
	_TCHAR consoleTitle[CONSOLE_TITLE_MAX_SIZE];
	GetConsoleTitle(consoleTitle, CONSOLE_TITLE_MAX_SIZE);
	SetConsoleTitle(_T("BLAHHHH"));
	Sleep(40);
	HWND hWnd = FindWindow(NULL, _T("BLAHHHH"));
	SetConsoleTitle(consoleTitle);
	return hWnd;
}

void LogDll(const char * const msg)
{
	std::string m(msg);
	std::cout << m << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE dll = LoadLibrary(_T("CapsToGWBasicAsciiDll"));//"F:\\Lp3\\General\\C++\\CapsToGWBasicAscii\\Debug\\CapsToGWBasicAsciiDll.dll")); 
	if (!dll) 
	{
		std::string blah;
		std::cout << "Error loading DLL!" << std::endl;
		std::cout << "Hit enter to quit." << std::endl;
		std::cin >> blah;
		return -1;
	}

	typedef void (*DebugOutput)(const char * const msg);
	typedef int (*Install)();//unsigned long, DebugOutput);
    typedef int (*Uninstall)();
	DebugOutput outputFunc = LogDll;
	Install install = (Install) GetProcAddress(dll, "CapsToGWBasicAscii_Install");
    Uninstall uninstall = (Uninstall) GetProcAddress(dll, "CapsToGWBasicAscii_Uninstall");

	HWND hWnd = findConsoleHWnd();
	unsigned long threadId = GetWindowThreadProcessId(hWnd, NULL);
	install();//threadId, outputFunc);

	std::cout << "Hello." << std::endl;
	std::string msg;
	while (msg != "exit")
	{
		std::cout << "?";
		std::cin >> msg;
		std::cout << msg << std::endl;
	}

	uninstall();
	FreeLibrary((HMODULE)dll);

	return 0;
}

