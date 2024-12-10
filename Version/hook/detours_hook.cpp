
#include <windows.h>
#include "detours/detours.h"


typedef int(WINAPI* pfnMessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);

pfnMessageBoxW TrueMessageBoxW = MessageBoxW;

int WINAPI MyMessageBoxW(HWND, const wchar_t*, const wchar_t*, DWORD)
{
	return TrueMessageBoxW(NULL, L"hooked message box", NULL, NULL);
}

void InitDetoursHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach(&(PVOID&)TrueMessageBoxW, MyMessageBoxW);

	DetourTransactionCommit();

	CreateThread(NULL, NULL, [](LPVOID) {
		Sleep(1 * 1000);
		MessageBox(0, L"123", 0, 0);
		return DWORD(0);
		}, NULL, NULL, NULL);
}