// TestApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestApp.h"
#include <objbase.h>

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
	 HRESULT hrInit = CoInitialize(NULL);    // Initialize COM so we can call CoCreateInstance
	 if (FAILED(hrInit))
		 return FALSE;

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
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
    DWORD dwThreadId = ::GetCurrentThreadId();

	 // Set hook on CallWndProc
    hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, hkCallWndProc, NULL, dwThreadId);
    if (!hhkCallWndProc)
    {
		 CoUninitialize();
       return FALSE;
    }

    // Set hook on GetMessage
    hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, hkGetMsgProc, NULL, dwThreadId);
    if (!hhkGetMessage)
    {
		 CoUninitialize();
       return FALSE;
    }

	 // Set hook on Keyboard
    hhkCallKeyboardMsg = SetWindowsHookEx(WH_KEYBOARD, hkCallKeyboardMsg, NULL, dwThreadId);
    if (!hhkGetMessage)
    {
		 CoUninitialize();
		 return FALSE;
    }

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

	 CoUninitialize();

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTAPP);
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

HRESULT CreateThumbnailToolbar(HWND hWnd);

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
	static UINT wmTaskbarButtonCreated = WM_NULL;

	if (wmTaskbarButtonCreated == WM_NULL)
	{
		// Compute the value for the TaskbarButtonCreated message
		wmTaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");

		// In case the application is run elevated, allow the
		// TaskbarButtonCreated and WM_COMMAND messages through.
		ChangeWindowMessageFilter(wmTaskbarButtonCreated, MSGFLT_ADD);
		ChangeWindowMessageFilter(WM_COMMAND, MSGFLT_ADD);
	}

	if (message == wmTaskbarButtonCreated)
	{
		// Once we get the TaskbarButtonCreated message, we can create
		// our window's thumbnail toolbar.
		HRESULT hThumbnailToolbar = CreateThumbnailToolbar(hWnd);
	}

	// Handle menu messages
	else switch (message)
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

