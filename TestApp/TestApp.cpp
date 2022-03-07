// TestApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <string>
#include<iostream>
#include <stdexcept>

#include <tlhelp32.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <pathcch.h>
#pragma comment(lib, "Pathcch.lib")

#include "TestApp.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;										// current window handle
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HHOOK hhkCallWndProc;
HHOOK hhkGetMessage;
HHOOK hhkCallKeyboardMsg;
extern HOOKPROC hkCallWndProc;
extern HOOKPROC hkGetMsgProc;
extern HOOKPROC hkCallKeyboardMsg;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DoIt(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TESTAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTAPP));

	/*
	HMODULE hModDLL = LoadLibrary(BUILD(MT_DLL_NAME));
	if (!hModDLL)
	{
		return FALSE;
	}

	// CallWndProc function
	HOOKPROC hkCallWndProc = (HOOKPROC)GetProcAddress(hModDLL, MT_HOOK_PROC_CWP);
	if (!hkCallWndProc)
	{
		return FALSE;
	}

	// GetMsgProc function
	HOOKPROC hkGetMsgProc = (HOOKPROC)GetProcAddress(hModDLL, MT_HOOK_PROC_GMP);
	if (!hkGetMsgProc)
	{
		return FALSE;
	}


	// GetKeyboardProc function
	HOOKPROC hkCallKeyboardMsg = (HOOKPROC)GetProcAddress(hModDLL, MT_HOOK_PROC_KYB);
	if (!hkCallKeyboardMsg)
	{
		return FALSE;
	}

	// Set hook on CallWndProc
	hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, hkCallWndProc, hModDLL, NULL);
	if (!hhkCallWndProc)
	{
		return FALSE;
	}

	// Set hook on GetMessage
	hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, hkGetMsgProc, hModDLL, NULL);
	if (!hhkGetMessage)
	{
		return FALSE;
	}

	// Set hook on Keyboard
	hhkCallKeyboardMsg = SetWindowsHookEx(WH_KEYBOARD, hkCallKeyboardMsg, hModDLL, NULL);
	if (!hhkCallKeyboardMsg)
	{
		return FALSE;
	}
  */
	/*
	DWORD dwThreadId = ::GetCurrentThreadId();

	// Set hook on CallWndProc
	hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, hkCallWndProc, NULL, dwThreadId);
	if (!hhkCallWndProc)
	{
		return FALSE;
	}

	// Set hook on GetMessage
	hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, hkGetMsgProc, NULL, dwThreadId);
	if (!hhkGetMessage)
	{
		return FALSE;
	}

	// Set hook on Keyboard
	hhkCallKeyboardMsg = SetWindowsHookEx(WH_KEYBOARD, hkCallKeyboardMsg, NULL, dwThreadId);
	if (!hhkGetMessage)
	{
		return FALSE;
	}
	*/

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTAPP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TESTAPP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	RECT wa = { 0 };
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wa, 0);
	int waw = wa.right - wa.left, wah = wa.bottom - wa.top;
	int w = 600, h = 400;

	int x = (waw - w) / 2;
	//int x = waw;
	int y = (wah - h) / 2;

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		x, y, w, h, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

int GetProcessIdByName(std::wstring exeName)
{
		HANDLE hSnapshot;
		PROCESSENTRY32 pe = {};
		int pid = 0;
		BOOL hResult;

		// snapshot of all processes in the system
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

		// initializing size: needed for using Process32First
		pe.dwSize = sizeof(PROCESSENTRY32);

		// info about first process encountered in a system snapshot
		hResult = Process32First(hSnapshot, &pe);

		// retrieve information about the processes
		// and exit if unsuccessful
		while (hResult) {
			// if we find the process: return process ID
			if (_wcsicoll(exeName.c_str(), pe.szExeFile) == 0) {
				pid = pe.th32ProcessID;
				break;
			}
			hResult = Process32Next(hSnapshot, &pe);
		}

		// closes an open handle (CreateToolhelp32Snapshot)
		CloseHandle(hSnapshot);
		return pid;
}

#define VERIFYB( statement ) if(!(statement)) throw std::runtime_error(#statement);
#define VERIFYH( statement ) if(!SUCCEEDED(statement)) throw std::runtime_error(#statement);

std::wstring GetDLLPath()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	TCHAR* out = nullptr;
	DWORD bufSize = sizeof(buffer) / sizeof(*buffer);

	// Get the fully-qualified path of the executable
	VERIFYB(GetModuleFileName(NULL, buffer, bufSize) < bufSize);

	// now buffer = "c:\whatever\yourexecutable.exe"
	size_t pathLen = wcslen(buffer);

	// Go to the beginning of the file name
	//out = PathFindFileName(buffer);
	// 
	out = buffer;
	// now out = "yourexecutable.exe"

	// Set the dot before the extension to 0 (terminate the string there)
	VERIFYH(PathCchRemoveFileSpec(out, pathLen));
	// now out = "yourexecutable"

	size_t outLen = wcslen(out);

	std::wstring dllName = L"\\";
	dllName += BUILD(MT_DLL_NAME);


	VERIFYH(PathCchAppend(out, bufSize, dllName.c_str()));

	return std::wstring(out);
}
#include <psapi.h>

void InjectVSCode()
{
	int procId = GetProcessIdByName(L"TestWindow.exe");

	std::wstring DllPath = GetDLLPath();
	//static HMODULE hMod = NULL;
	//if (hMod)
	//{
	//	HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
	//	VERIFYB(hNtDll);

	//	//MODULEINFO baseModuleInfo;
	//	//bool success = GetModuleInfo(&baseModuleInfo);

	//	auto unloadDll = GetProcAddress(hNtDll, "LdrUnloadDll");
	//	unloadDll();
	//}
	//else
	//{
	//	hMod = LoadLibraryW(DllPath);
	//}



	// Open a handle to target process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	VERIFYB(hProcess);


	//LdrUnloadDll()

	// Allocate memory for the dllpath in the target process
	// length of the path string + null terminator
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, wcslen(DllPath.c_str()) + 1, MEM_COMMIT, PAGE_READWRITE);
	VERIFYB(pDllPath);

	// Write the path to the address of the memory we just allocated
	// in the target process
	WriteProcessMemory(hProcess, pDllPath, DllPath.c_str(), (wcslen(DllPath.c_str()) * 2) + 1, 0);

	HMODULE hKerne32 = GetModuleHandleA("Kernel32.dll");
	VERIFYB(hKerne32);

	// Create a Remote Thread in the target process which
	// calls LoadLibraryA as our dllpath as an argument -> program loads our dll
	LPTHREAD_START_ROUTINE startThread = (LPTHREAD_START_ROUTINE)GetProcAddress(hKerne32, "LoadLibraryW");
	VERIFYB(startThread);

	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0, startThread, pDllPath, 0, 0);
	VERIFYB(hLoadThread);

	// Wait for the execution of our loader thread to finish
	WaitForSingleObject(hLoadThread, INFINITE);

	// Free the memory allocated for our dll path
	VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_FILE_INJECTVISUALSTUDIOCODE:
			InjectVSCode();
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

