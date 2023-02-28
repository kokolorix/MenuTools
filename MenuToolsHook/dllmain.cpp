// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <MenuCommon/ScreenToolWnd.h>

HWND GetMainWnd();
void InstallHooks();


HINSTANCE hInst;  // current instance

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hInst = hModule;

		InstallHooks();

		HWND hWnd = GetMainWnd();

		UINT wmTaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");
		PostMessage(hWnd, wmTaskbarButtonCreated, 0, 0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
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
		GetWindowThreadProcessId(hWnd, &wndProcessId);
		if (processId == wndProcessId)
		{
			HWND hMainWnd = GetAncestor(hWnd, GA_ROOT);
			return hMainWnd;
		}
	}

	return NULL;

	//DWORD (
	//	[in]            HWND    hWnd,
	//	[out, optional] LPDWORD lpdwProcessId
	//);
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
	// CallWndProc function
	HOOKPROC hkCallWndProc = (HOOKPROC)GetProcAddress(hInst, MT_HOOK_PROC_CWP);
	if (!hkCallWndProc)
	{
		return;
	}

	// GetMsgProc function
	HOOKPROC hkGetMsgProc = (HOOKPROC)GetProcAddress(hInst, MT_HOOK_PROC_GMP);
	if (!hkGetMsgProc)
	{
		return;
	}


	HOOKPROC hkCallKeyboardMsg = (HOOKPROC)GetProcAddress(hInst, MT_HOOK_PROC_KYB);
	if (!hkCallKeyboardMsg)
	{
		return;
	}

	DWORD dwThreadId = ::GetCurrentThreadId();

	// Set hook on CallWndProc
	hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, hkCallWndProc, NULL, dwThreadId);
	if (!hhkCallWndProc)
	{
		return;
	}

	// Set hook on GetMessage
	hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, hkGetMsgProc, NULL, dwThreadId);
	if (!hhkGetMessage)
	{
		return;
	}

	// Set hook on Keyboard
	hhkCallKeyboardMsg = SetWindowsHookEx(WH_KEYBOARD, hkCallKeyboardMsg, NULL, dwThreadId);
	if (!hhkGetMessage)
	{
		return;
	}
}