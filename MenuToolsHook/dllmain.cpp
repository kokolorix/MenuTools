// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <MenuCommon/ScreenToolWnd.h>
#include "MenuTools.h"

HWND GetMainWnd();
void InstallHooks();
void UninstallHooks();


HINSTANCE hInst;  // current instance
DWORD dwMainThreadId = 0;
HWND hMainWnd = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hInst = hModule;


		hMainWnd = GetMainWnd();

		InstallHooks();

		UINT wmTaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");
		PostMessage(hMainWnd, wmTaskbarButtonCreated, 0, 0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		UninstallHooks();
		MenuTools::Uninstall(hMainWnd);
		#include <MenuCommon/ScreenToolWnd.h>
		ScreenToolWnd::pWnd.reset();
		hInst = NULL;
	}


	return TRUE;
}

using WndVector = std::vector<HWND>;
WndVector GetDesktopWnds();

HWND GetMainWnd()
{
	auto wnds = GetDesktopWnds();

	auto processId = GetCurrentProcessId();

	for (HWND hWnd : wnds) {
		DWORD wndProcessId = NULL;
		dwMainThreadId = GetWindowThreadProcessId(hWnd, &wndProcessId);
		if (processId == wndProcessId)
		{
			HWND hMainWnd = GetAncestor(hWnd, GA_ROOT);
			return hMainWnd;
		}
	}

	return NULL;
}

BOOL CALLBACK EnumWindowsProc(HWND   hWnd, LPARAM lParam)
{
	WndVector& hWnds = *reinterpret_cast<WndVector*>(lParam);
	hWnds.push_back(hWnd);
	return TRUE;
}

WndVector GetDesktopWnds()
{
	WndVector hWnds;
	auto threadId = GetCurrentThreadId();
	auto hDesktop = GetThreadDesktop(threadId);

	//auto glambda = [](auto a, auto&& b) { return a < b; };

	//auto enumWndProc = [&hWnds](HWND hWnd, LPARAM lParam) -> BOOL
	//{
	//	hWnds.push_back(hWnd);
	//	return TRUE;
	//};
	//static_cast<bool(__stdcall*)(HWND, LPARAM)>(&enumWndProc);
	auto res = EnumDesktopWindows(hDesktop, EnumWindowsProc, (LPARAM)&hWnds);

	return hWnds;	//EnumDesktopWindows()
}

HHOOK hhkCallWndProc;
HHOOK hhkGetMessage;
HHOOK hhkCallKeyboardMsg;

extern HOOKPROC hkCallWndProc;
extern HOOKPROC hkGetMsgProc;
extern HOOKPROC hkCallKeyboardMsg;

#define MT_HOOK_PROC_CWP					"CallWndProc"
#define MT_HOOK_PROC_GMP					"GetMsgProc"
#define MT_HOOK_PROC_KYB					"CallKeyboardMsg"


void InstallHooks() 
{
	HMODULE hMenuTool = GetModuleHandle(MT_DLL_NAME64);
	if(hMenuTool == 0)
		hMenuTool = LoadLibrary(MT_DLL_NAME64);
	if (hMenuTool == NULL)
		return;

	// CallWndProc function
	HOOKPROC hkCallWndProc = (HOOKPROC)GetProcAddress(hMenuTool, MT_HOOK_PROC_CWP);
	if (!hkCallWndProc)
	{
		return;
	}

	// GetMsgProc function
	HOOKPROC hkGetMsgProc = (HOOKPROC)GetProcAddress(hMenuTool, MT_HOOK_PROC_GMP);
	if (!hkGetMsgProc)
	{
		return;
	}


	HOOKPROC hkCallKeyboardMsg = (HOOKPROC)GetProcAddress(hMenuTool, MT_HOOK_PROC_KYB);
	if (!hkCallKeyboardMsg)
	{
		return;
	}

	DWORD dwThreadId = ::GetCurrentThreadId();

	// Set hook on CallWndProc
	hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, hkCallWndProc, NULL, dwMainThreadId);
	if (!hhkCallWndProc)
	{
		return;
	}

	// Set hook on GetMessage
	hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, hkGetMsgProc, NULL, dwMainThreadId);
	if (!hhkGetMessage)
	{
		return;
	}

	// Set hook on Keyboard
	hhkCallKeyboardMsg = SetWindowsHookEx(WH_KEYBOARD, hkCallKeyboardMsg, NULL, dwMainThreadId);
	if (!hhkGetMessage)
	{
		return;
	}
}

void UninstallHooks()
{
	UnhookWindowsHookEx(hhkCallWndProc);
	UnhookWindowsHookEx(hhkGetMessage);
	UnhookWindowsHookEx(hhkCallKeyboardMsg);
}
