// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <windows.h>
#include "log/static_log.h"

extern void InitVehHook(size_t hookAddress);

extern void InitDetoursHook();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LOG_INFO(L"dll main.");
        //InitVehHook((size_t)MessageBox);
        //InitDetoursHook();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

