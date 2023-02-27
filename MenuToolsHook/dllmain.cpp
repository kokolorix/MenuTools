// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <MenuCommon/ScreenToolWnd.h>

HWND GetMainWnd();


HINSTANCE hInst;  // current instance

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hInst = hModule;
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


