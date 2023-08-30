// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <map>
#include <MenuCommon/ScreenToolWnd.h>
#include "MenuTools.h"

HWND GetMainWnd(DWORD currentThreadId = GetCurrentThreadId());
void InstallHooks(DWORD currentThreadId = GetCurrentThreadId());
void UninstallHooks(DWORD currentThreadId = GetCurrentThreadId());


HINSTANCE hInst;  // current instance
DWORD dwMainThreadId = 0;
//HWND hMainWnd = 0;
using MainWndMap = std::map<DWORD, HWND>;
MainWndMap mainWndMap;
std::mutex mainWndMutex;

struct Hooks
{
	HHOOK hhShell;
	HHOOK hhkCallWndProc;
	HHOOK hhkGetMessage;
	HHOOK hhkCallKeyboardMsg;
};
using HookMap = std::map<DWORD, Hooks>;
HookMap hookMap;
std::mutex hookMutex;

void Install()
{
	auto currentThreadId = GetCurrentThreadId();
	HWND hMainWnd = GetMainWnd(currentThreadId);
	if (hMainWnd)
	{
		std::lock_guard<std::mutex> lock(mainWndMutex);
		mainWndMap[currentThreadId] = hMainWnd;

		//UINT wmTaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");
		//PostMessage(hMainWnd, wmTaskbarButtonCreated, 0, 0);

		InstallHooks(currentThreadId);
	}
}

void Uninstall()
{
	auto currentThreadId = GetCurrentThreadId();
	HWND hMainWnd = 0;

	{
		std::lock_guard<std::mutex> lock(mainWndMutex);
		auto it = mainWndMap.find(currentThreadId);
		if (it == mainWndMap.end())
			return;
		hMainWnd = it->second;
	}

	UninstallHooks(currentThreadId);
	MenuTools::Uninstall(hMainWnd);
	ScreenToolWnd::pWnd.reset();
}

HHOOK g_hHook = NULL;
LRESULT CALLBACK ShellHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	DWORD currentThreadId = GetCurrentThreadId();

	if (nCode == HSHELL_WINDOWCREATED)
	{
		HWND hNewWindow = (HWND)wParam;
		// Check if the new window belongs to your process
		DWORD dwProcessId;
		GetWindowThreadProcessId(hNewWindow, &dwProcessId);
		if (dwProcessId == GetCurrentProcessId())
		{
			Install();// Handle the new window creation here
		}
	}

	std::lock_guard<std::mutex> lock(hookMutex);
	Hooks hooks = hookMap[currentThreadId];
	return CallNextHookEx(hooks.hhShell, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hInst = hModule;
		Install();
		//g_hHook = SetWindowsHookEx(WH_SHELL, ShellHookProc, NULL, 0);


		//hMainWnd = GetMainWnd();

		//InstallHooks();

		//UINT wmTaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");
		//PostMessage(hMainWnd, wmTaskbarButtonCreated, 0, 0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//UnhookWindowsHookEx(g_hHook);

		Uninstall();
		//UninstallHooks();
		//MenuTools::Uninstall(hMainWnd);
		//#include <MenuCommon/ScreenToolWnd.h>
		//ScreenToolWnd::pWnd.reset();
		hInst = NULL;
	}
	else if (dwReason == DLL_THREAD_ATTACH)
	{
		DWORD currentThreadId = GetCurrentThreadId();

		Hooks hooks = {};

		hooks.hhShell = SetWindowsHookEx(WH_SHELL, ShellHookProc, NULL, currentThreadId);

		std::lock_guard<std::mutex> lock(hookMutex);
		hookMap[currentThreadId] = hooks;
		//Install();
	}
	else if (dwReason == DLL_THREAD_DETACH)
	{
		Uninstall();
	}


	return TRUE;
}

using WndVector = std::vector<HWND>;
WndVector GetDesktopWnds();

HWND GetMainWnd(DWORD currentThreadId /*= GetCurrentThreadId()*/)
{
	auto wnds = GetDesktopWnds();

	auto currentProcessId = GetCurrentProcessId();

	for (HWND hWnd : wnds) {
		DWORD wndProcessId = NULL;
		auto wndThreadId = GetWindowThreadProcessId(hWnd, &wndProcessId);
		if (currentProcessId == wndProcessId && wndThreadId == currentThreadId)
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

//HHOOK hhkCallWndProc;
//HHOOK hhkGetMessage;
//HHOOK hhkCallKeyboardMsg;

extern HOOKPROC hkCallWndProc;
extern HOOKPROC hkGetMsgProc;
extern HOOKPROC hkCallKeyboardMsg;

#define MT_HOOK_PROC_CWP					"CallWndProc"
#define MT_HOOK_PROC_GMP					"GetMsgProc"
#define MT_HOOK_PROC_KYB					"CallKeyboardMsg"


void InstallHooks(DWORD currentThreadId /*= GetCurrentThreadId()*/)
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

	Hooks hooks = {};

	// Set hook on CallWndProc
	hooks.hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, hkCallWndProc, NULL, currentThreadId);
	if (!hooks.hhkCallWndProc)
	{
		return;
	}

	// Set hook on GetMessage
	hooks.hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, hkGetMsgProc, NULL, currentThreadId);
	if (!hooks.hhkGetMessage)
	{
		return;
	}

	// Set hook on Keyboard
	hooks.hhkCallKeyboardMsg = SetWindowsHookEx(WH_KEYBOARD, hkCallKeyboardMsg, NULL, currentThreadId);
	if (!hooks.hhkGetMessage)
	{
		return;
	}

	std::lock_guard<std::mutex> lock(hookMutex);
	hookMap[currentThreadId] = hooks;
}

void UninstallHooks(DWORD currentThreadId /*= GetCurrentThreadId()*/)
{
	Hooks hooks = {};
	{
		std::lock_guard<std::mutex> lock(hookMutex);
		auto it = hookMap.find(currentThreadId);
		if (it == hookMap.end())
			return;

		hooks = it->second;
	}

	if (hooks.hhShell)
		UnhookWindowsHookEx(hooks.hhShell);
	if (hooks.hhkCallWndProc)
		UnhookWindowsHookEx(hooks.hhkCallWndProc);
	if (hooks.hhkGetMessage)
		UnhookWindowsHookEx(hooks.hhkGetMessage);
	if (hooks.hhkCallKeyboardMsg)
		UnhookWindowsHookEx(hooks.hhkCallKeyboardMsg);
}
