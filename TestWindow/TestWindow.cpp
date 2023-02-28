// TestWindow.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TestWindow.h" 
#include "resource.h"
#include <string>
#include <format>
#include <string_view>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
//#include "../MenuCommon/Defines.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_TESTWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTWINDOW));



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

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWINDOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTWINDOW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		(waw - w) / 2, (wah - h) / 2, w, h, nullptr, nullptr, hInstance, nullptr);
      //CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void AttacheMenuTools(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void DetachMenuTools(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void AttachMenuToolsToExplorer(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            case ID_FILE_ATTACHMENUTOOLS:
               AttacheMenuTools(hWnd, message, wParam, lParam);
               break;

				case ID_FILE_DETACHMENUTOOLS:
					DetachMenuTools(hWnd, message, wParam, lParam);
					break;

				case ID_FILE_ATTACH_MENUTOOLS_TO_EXPLORER:
					AttachMenuToolsToExplorer(hWnd, message, wParam, lParam);
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

HMODULE hMenuTool = NULL;

HHOOK hhkCallWndProc;
HHOOK hhkGetMessage;
HHOOK hhkCallKeyboardMsg;

HOOKPROC hkCallWndProc;
HOOKPROC hkGetMsgProc;
HOOKPROC hkCallKeyboardMsg;

#define MT_HOOK_PROC_CWP					"CallWndProc"
#define MT_HOOK_PROC_GMP					"GetMsgProc"
#define MT_HOOK_PROC_KYB					"CallKeyboardMsg"

void AttacheMenuTools(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   hMenuTool = LoadLibrary(L"MenuToolsHook64.dll");
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
	hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, hkCallWndProc, NULL, dwThreadId);
	if (!hhkCallWndProc)
	{
		return ;
	}

	// Set hook on GetMessage
	hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, hkGetMsgProc, NULL, dwThreadId);
	if (!hhkGetMessage)
	{
		return ;
	}

	// Set hook on Keyboard
	hhkCallKeyboardMsg = SetWindowsHookEx(WH_KEYBOARD, hkCallKeyboardMsg, NULL, dwThreadId);
	if (!hhkGetMessage)
	{
		return ;
	}
}

void DetachMenuTools(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   UnhookWindowsHookEx(hhkCallWndProc);
   UnhookWindowsHookEx(hhkGetMessage);
   UnhookWindowsHookEx(hhkCallKeyboardMsg);
   FreeLibrary(hMenuTool);
}


template <typename... Args>
std::wstring dyna_print(std::wstring_view rt_fmt_str, Args&&... args) {
	return std::vformat(rt_fmt_str, std::make_wformat_args(args...));
}

inline void ShowLastError(const std::wstring fmtStr)
{
	LPWSTR errorText = NULL;
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&errorText,
		0,
		NULL);
	const std::wstring errorStr = errorText;

	std::wstring msg = dyna_print(fmtStr, errorStr);
	MessageBox(NULL, msg.c_str(), L"Error", MB_ICONERROR | MB_OK);
	LocalFree(errorText);
}

void AttachMenuToolsToExplorer(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Find the window handle for the desktop shell
	HWND hShellWnd = FindWindow(_T("Progman"), NULL);
	if (hShellWnd == NULL)
		return ShowLastError(L"Failed to find desktop shell window, error {}");

	// Get the process ID of the desktop shell window
	DWORD shellPid = 0;
	GetWindowThreadProcessId(hShellWnd, &shellPid);
	if (shellPid == 0)
		return ShowLastError(L"Failed to get process ID for desktop shell, error {}");

	DWORD targetPid = shellPid;

	// Open the target process with read/write access
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetPid);
	if (hProcess == NULL)
		return ShowLastError(L"Failed to open process, error {}");

	// Get the address of the LoadLibrary function in the kernel32.dll module
	HMODULE hKernel32 = GetModuleHandle(_T("kernel32.dll"));
	if (hKernel32 == NULL)
		return ShowLastError(L"Failed to obtain kernel32.dll handle, error {}");

	FARPROC pLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryW");
	if (pLoadLibrary == nullptr)
		return ShowLastError(L"Failed to obtain LoadLibraryW adress, error {}");

	WCHAR path[MAX_PATH];
	DWORD pathLength = GetModuleFileName(NULL, path, MAX_PATH);
	PathRemoveFileSpec(path);
	PathAppend(path, L"MenuToolsHook64.dll");


	// Allocate memory in the target process to hold the path to the DLL
	SIZE_T pathSize = (_tcslen(path) * sizeof(_TCHAR));
	LPVOID remotePath = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT, PAGE_READWRITE);
	if (remotePath == NULL)
	{
		ShowLastError(L"Failed to allocate memory in process, error {}");
		CloseHandle(hProcess);
		return;
	}

	// Write the path to the DLL to the allocated memory in the target process
	if (!WriteProcessMemory(hProcess, remotePath, path, pathSize, NULL))
	{
		ShowLastError(L"Failed to write to process memory, error {}");
		VirtualFreeEx(hProcess, remotePath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return;
	}

	// Create a remote thread in the target process to call LoadLibrary with the path to the DLL
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, remotePath, 0, NULL);
	if (hThread == NULL)
	{
		ShowLastError(L"Failed to create remote thread, error {}");
		VirtualFreeEx(hProcess, remotePath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return;
	}

	// Wait for the remote thread to finish executing
	WaitForSingleObject(hThread, INFINITE);

	// Free the memory and close the handles
	VirtualFreeEx(hProcess, remotePath, 0, MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
}

