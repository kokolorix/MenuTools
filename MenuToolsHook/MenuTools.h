#include <tchar.h>
#include <ShellAPI.h>
#include <strsafe.h>

namespace MenuTools
{
	// Functions
	BOOL Install(HWND hWnd);
	BOOL Uninstall(HWND hWnd);
	VOID Status(HWND hWnd);

	// Callback
	BOOL TrayProc(HWND hWnd, WPARAM wParam, LPARAM lParam);
	BOOL WndProc(HWND hWnd, WPARAM wParam);
};

extern NOTIFYICONDATA nid;

// Helpers
HICON GetWindowIcon(HWND hWnd);
BOOL InsertSubMenu(HMENU hMenu, HMENU hSubMenu, UINT uPosition, UINT uFlags, UINT uIDNewItem, LPCWSTR lpNewItem);
BOOL IsMenuItem(HMENU hMenu, UINT item);