// NetDll.h
#include <iostream>
#include <string>
#include <tchar.h>
#include <windows.h>
#pragma once

using namespace System;

namespace NetDll {

	public ref class CapsToGWBasicAscii
	{
	public:
		typedef void (*DebugOutput)(const char * const msg);
		typedef int (*InstallFunc)();
		typedef int (*UninstallFunc)();

		DebugOutput outputFunc;
		InstallFunc install;
		UninstallFunc uninstall;

		CapsToGWBasicAscii()
		{	
			HINSTANCE dll = LoadLibrary(_T("CapsToGWBasicAsciiDll.dll"));//F:\\Lp3\\General\\C++\\CapsToGWBasicAscii\\Debug\\CapsToGWBasicAsciiDll.dll")); 
			if (!dll)	
			{
				throw gcnew System::Exception(_T("Error loading DLL!"));
				//std::string blah;
				//std::cout << "Error loading DLL!" << std::endl;
				//std::cout << "Hit enter to quit." << std::endl;
				//std::cin >> blah;
				//return -1;
			}
			//this->outputFunc = LogDll;
			this->install = (InstallFunc) GetProcAddress(dll, "CapsToGWBasicAscii_Install");
			if (!install)
			{
				throw gcnew System::Exception(_T("Could not find install function in DLL."));
			}
			this->uninstall = (UninstallFunc) GetProcAddress(dll, "CapsToGWBasicAscii_Uninstall");		
			if (!uninstall)
			{
				throw gcnew System::Exception(_T("Could not find uninstall function in DLL."));
			}
		}

		void Install()
		{
			this->install();
		}
		
		void Uninstall()
		{
			this->uninstall();
		}
	};
}
