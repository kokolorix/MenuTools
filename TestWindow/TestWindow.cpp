// TestWindow.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TestWindow.h" 
#include "resource.h"
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
