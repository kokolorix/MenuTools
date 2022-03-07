// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <MenuCommon/ScreenToolWnd.h>
#include "..\MenuTools\Hooks.h"

HINSTANCE hInst;  // current instance

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hInst = hModule;
		//Hooks hooks;
		//hooks.Install();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		ScreenToolWnd::pWnd.reset();
		hInst = NULL;
	}


	return TRUE;
}

