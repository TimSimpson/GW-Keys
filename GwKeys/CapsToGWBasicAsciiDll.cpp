// CapsToGWBasicAsciiDll.cpp : Defines the exported functions for the DLL application.
//
#include <windows.h>
#include "CapsToGWBasicAsciiDll.h"
#include <sstream>

HINSTANCE hInstance;
#pragma data_seg(".shared")

HHOOK hook = NULL;
static int fakeCapsLockInComing = 0;
static bool capsLockTrueState = false;
//static DebugOutput log;

#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

static int GwKeys[256];

static void initializeGwKeys()
{
	// Useful references:
	// http://www.beginningtoseethelight.org/ascii/
	// http://www.mikezilla.com/exp0012.html
	// Funny symbols that don't actually work :( http://t-a-w.blogspot.com/2008/12/funny-characters-in-unicode.html
	// Virtual key codes: http://msdn.microsoft.com/en-us/library/dd375731%28v=VS.85%29.aspx

	for (int i = 0 ; i < 256; i ++) 
	{
		GwKeys[i] = 0;
	}
	/*
	GwKeys['Q'] = 9668;
	GwKeys['W'] = 8616;
	GwKeys['T'] = 182;
	GwKeys['Y'] = 8595;
	GwKeys['U'] = 167;
	GwKeys['O'] = 9788;
	GwKeys['P'] = 9658;
	GwKeys[VK_BACK] = 8962; //127
	GwKeys['A'] = 9786;//1;
	GwKeys['S'] = 8252;
	GwKeys['D'] = 9830;	
	GwKeys['Z'] = 8594;
	GwKeys['X'] = 8593;
	GwKeys['V'] = 9644;
	GwKeys[VK_F6] = 9608;
	GwKeys[VK_F7] = 9604;
	GwKeys[VK_F8] = 9612;
	GwKeys[VK_F9] = 9616;
	GwKeys[VK_F5] = 9600;
	
	GwKeys['1'] = 9787;
	GwKeys['2'] = 9829;
	GwKeys['3'] = 9827;
	GwKeys['4'] = 9824;
	GwKeys['5'] = 8226;
	GwKeys['6'] = 9688;
	GwKeys['9'] = 9675;
	GwKeys['0'] = 9689;
	GwKeys['E'] = 9794; // 11
	GwKeys['R'] = 9792; // 12
	GwKeys['I'] = 9834; // 13
	GwKeys['['] = 9835;
	GwKeys[']'] = 8595;
	GwKeys['F'] = 8592; // 27
	GwKeys['G'] = 8735;
	GwKeys['H'] = 9650;
	GwKeys['J'] = 9660;*/

	GwKeys['1'] = 9787;
	GwKeys['2'] = 9829;
	GwKeys['3'] = 9827;
	GwKeys['4'] = 9824;
	GwKeys['5'] = 8226;
	GwKeys['6'] = 9688;
	GwKeys['9'] = 9675;
	GwKeys['0'] = 9689;
	GwKeys[VK_BACK] = 8962; //127  // GW
	
	GwKeys['Q'] = 9668; // GW
	GwKeys['W'] = 8616; // GW
	GwKeys['E'] = 9794; // 11
	GwKeys['R'] = 9792; // 12
	GwKeys['T'] = 182; // GW
	GwKeys['Y'] = 8595; // GW
	GwKeys['U'] = 167; // GW
	GwKeys['I'] = 9834; // 13
	GwKeys['O'] = 9788; // GW
	GwKeys['P'] = 9658; // GW
	GwKeys['['] = 9835;
	GwKeys[']'] = 8595;

	GwKeys['A'] = 9786;//1;  // GW
	GwKeys['S'] = 8252; // GW
	GwKeys['D'] = 9830;	 // GW
	GwKeys['F'] = 8592; // 27
	GwKeys['G'] = 8735;
	GwKeys['H'] = 9650;
	GwKeys['J'] = 9660; // 31
	GwKeys['K'] = 199;
	GwKeys['L'] = 252;
	GwKeys[';'] = 233;
	GwKeys['\''] = 226;

	GwKeys['Z'] = 8594; // GW
	GwKeys['X'] = 8593; // GW
	GwKeys['V'] = 9644; // GW
	GwKeys[VK_F6] = 9608; // GW
	GwKeys[VK_F7] = 9604; // GW
	GwKeys[VK_F8] = 9612; // GW
	GwKeys[VK_F9] = 9616; // GW
	GwKeys[VK_F5] = 9600; // GW
	
	
	
	
	

	
}

class Keyboard
{
private:
	BYTE keyState[256];
public: 
	Keyboard()
	{
		GetKeyboardState((LPBYTE)&keyState);
	}

	inline bool keyModifierOn(int index)
	{
		return ( keyState[index] & 1 );
	}

	inline bool capsLockIsOn() 
	{
		return keyModifierOn(VK_CAPITAL);
	}

	inline bool numLockIsOn() 
	{
		return keyModifierOn(VK_NUMLOCK); 
	}

	inline void simpleKeyPress(BYTE keyCode)
	{
		keybd_event(keyCode, 0, 0, 0);
		keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
	}

	void SetCapsLock(bool desiredState)
	{
		bool futureState = capsLockIsOn();// || keyWillBeDown;
		if ((desiredState && !futureState)
			|| (!desiredState && futureState))
		{
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}

	void SetNumLock(bool desiredState)
	{
		if( (desiredState && !numLockIsOn()) ||
			  (!desiredState && (numLockIsOn())) )
		  {
		  // Simulate a key press
			 keybd_event( VK_NUMLOCK,
						  0x45,
						  KEYEVENTF_EXTENDEDKEY | 0,
						  0 );

		  // Simulate a key release
			 keybd_event( VK_NUMLOCK,
						  0x45,
						  KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
						  0);
		  }
	}

	void printUnicode(int unicodeChar)
	{
		bool altAlreadyOn = keyModifierOn(VK_MENU);
		
		/*keybd_event('L', 0, 0, 0);
		keybd_event('L', 0, KEYEVENTF_KEYUP, 0);*/

		//if (!altAlreadyOn) 
		//{
			// Simulate alt key going down...
			//keybd_event(VK_MENU, 0, 0, 0);
		//}
		
		//Beep(100, 50);

		//keybd_event(VK_NUMPAD2, 0, 0, 0);
		//keybd_event(VK_NUMPAD2, 0, KEYEVENTF_KEYUP, 0);
		//keybd_event(VK_NUMPAD1, 0, 0, 0);
		//keybd_event(VK_NUMPAD1, 0, KEYEVENTF_KEYUP, 0);
		//keybd_event(VK_NUMPAD9, 0, 0, 0);
		//keybd_event(VK_NUMPAD9, 0, KEYEVENTF_KEYUP, 0);

		// Simulate alt key going up
		//keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);	
		//if (altAlreadyOn)
		//{
		//	keybd_event(VK_MENU, 0, 0, 0);
		//}

		// 219 == 9608
		// 128 
		// 01  == 9786

		KEYBDINPUT ki;
		INPUT input;
		int character = 0;

		ki.wVk = 0;
		///*if (asciiIndex < 128) 
		//{
		//	ki.wScan = 9785 + asciiIndex;
		//} 
		//else
		//{
		//	ki.wScan = 9517 + asciiIndex;
		//}*/
		ki.wScan = unicodeChar;
		ki.dwFlags = KEYEVENTF_UNICODE;
		ki.time = 0;
		ki.dwExtraInfo = 0;   
		input.type = INPUT_KEYBOARD;
		input.ki = ki;
		SendInput(1, &input, sizeof(INPUT));

	}

};


//void SetNumLock( BOOL bState )
//{
//  BYTE keyState[256];
//
//  GetKeyboardState((LPBYTE)&keyState);
//  if( (bState && !(keyState[VK_NUMLOCK] & 1)) ||
//      (!bState && (keyState[VK_NUMLOCK] & 1)) )
//  {
//  // Simulate a key press
//     keybd_event( VK_NUMLOCK,
//                  0x45,
//                  KEYEVENTF_EXTENDEDKEY | 0,
//                  0 );
//
//  // Simulate a key release
//     keybd_event( VK_NUMLOCK,
//                  0x45,
//                  KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
//                  0);
//  }
//}

//
//static LRESULT CALLBACK LowLevelKeyboardProc(
//  __in  int nCode,
//  __in  WPARAM wParam,
//  __in  LPARAM lParam
//)
//{
//	if (code < 0)
//	{
//		return CallNextHookEx(hook, code, wParam, lParam);
//	}
//	/* The identifier of the keyboard message. 
//		This parameter can be one of the following messages: 
//		WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP. 
//	*/
//
//	KBDLLHOOKSTRUCT keyInfo = (KBDLLHOOKSTRUCT *) lParam;
//	Keyboard kb;
//
//	return CallNextHookEx(hook, code, wParam, lParam);
//}
static void moveMouse(int x, int y)
{
	// This code doesn't work right now (OR EVER?!!)
	MOUSEINPUT mi;
	INPUT input;
	int character = 0;

	mi.dx = x;
	///*if (asciiIndex < 128) 
	//{
	//	ki.wScan = 9785 + asciiIndex;
	//} 
	//else
	//{
	//	ki.wScan = 9517 + asciiIndex;
	//}*/
	mi.dy = y;
	mi.mouseData = 0;
	mi.dwFlags = 0;
	mi.time = 0;
	mi.dwExtraInfo = 0;
	input.type = INPUT_MOUSE;
	input.mi = mi;
	SendInput(1, &input, sizeof(INPUT));
}

static LRESULT CALLBACK KeyboardProc(
	__in  int code, 
	__in  WPARAM wParam, 
	__in  LPARAM lParam)
{
	if (code < 0)
	{
		return CallNextHookEx(hook, code, wParam, lParam);
	}

	Keyboard kb;		
	
	//1000000000000000000000000000000
	//0100000000000000000000000000000
	//0x80000000
	//0x40000000
	//0x020000000
	bool keyWasDown = (lParam & (0x40000000)) == 0;
	bool keyDown = (lParam & (0x80000000)) == 0;
	if (wParam == VK_CAPITAL)	
	{
		if (fakeCapsLockInComing > 0)
		{
			fakeCapsLockInComing --;
		}
		else
		{	
			wParam = 'A';
			if (keyDown && !capsLockTrueState) // !keyWasDown)
			{
				fakeCapsLockInComing = 2;
				capsLockTrueState = true;	
				
				// Simulate key release
				keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				// Simulate key down (this turns it off)
				keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
				//Beep(1200, 20);
			}			
			else if (!keyDown && capsLockTrueState)
			{
				capsLockTrueState = false;
				if (kb.capsLockIsOn())
				{
					// SHOULD BE OFF, but I've found sometimes it can somehow turn
					// on, in which case this is needed
					fakeCapsLockInComing = 2;
					// Simulate key release
					keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
					// Simulate key down (this turns it off)
					keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
				}				
			}
			return 10;			
		}		
	}
	
	if (capsLockTrueState)
	{			
		if (keyDown)
		{		
			if (wParam == VK_ESCAPE)
			{
				// This is the "Get me the hell out of here" key.
				// Very important for when things ultimately break.
				kb.SetCapsLock(false);
				capsLockTrueState = false;
			}

			if (wParam == VK_LEFT)
			{
				//moveMouse(-100, 0);
				kb.simpleKeyPress(VK_MEDIA_PREV_TRACK);
			}
			else if (wParam == VK_RIGHT)
			{
				//moveMouse(100, 0);
				kb.simpleKeyPress('K');
			}
			if (wParam == VK_UP)
			{
				//moveMouse(-100, 0);
				kb.simpleKeyPress(VK_VOLUME_UP);
			}
			else if (wParam == VK_DOWN)
			{
				//moveMouse(100, 0);
				kb.simpleKeyPress(VK_VOLUME_DOWN);
			}
			
			
			int unicode = GwKeys[wParam];
			if (unicode > 0)
			{
				kb.printUnicode(unicode);
				return 10;
			}
		}				
	}
	
	return CallNextHookEx(hook, code, wParam, lParam);
}

extern "C"
{
	CAPSTOGWBASICASCIIDLL_API int GwKeys_Install()//HWND hWnd)
	{
		//unsigned long threadId = GetWindowThreadProcessId(hWnd, NULL);

		hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hInstance, 0);//threadId);
		//log = output;
		return 0;
	}

	CAPSTOGWBASICASCIIDLL_API int GwKeys_Uninstall()
	{
		UnhookWindowsHookEx(hook);
		return 0;
	}

}

BOOL WINAPI DllMain(__in HINSTANCE hinstDLL, __in  DWORD fdwReason, __in  LPVOID lpvReserved) 
{
    switch (fdwReason)
	{
    case DLL_PROCESS_ATTACH :
	  initializeGwKeys();
      hInstance = (HINSTANCE) hinstDLL;
      break;

    case DLL_THREAD_ATTACH :
      break;

    case DLL_THREAD_DETACH :
      break;

    case DLL_PROCESS_DETACH :
      break;
    }
    return TRUE;
}
