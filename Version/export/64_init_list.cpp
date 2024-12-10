// dllmain.cpp : 定义 DLL 应用程序的入口点。

#if _WIN64

#include <windows.h>

extern "C"
{
FARPROC Hijack_GetFileVersionInfoA;
FARPROC Hijack_GetFileVersionInfoByHandle;
FARPROC Hijack_GetFileVersionInfoExA;
FARPROC Hijack_GetFileVersionInfoExW;
FARPROC Hijack_GetFileVersionInfoSizeA;
FARPROC Hijack_GetFileVersionInfoSizeExA;
FARPROC Hijack_GetFileVersionInfoSizeExW;
FARPROC Hijack_GetFileVersionInfoSizeW;
FARPROC Hijack_GetFileVersionInfoW;
FARPROC Hijack_VerFindFileA;
FARPROC Hijack_VerFindFileW;
FARPROC Hijack_VerInstallFileA;
FARPROC Hijack_VerInstallFileW;
FARPROC Hijack_VerLanguageNameA;
FARPROC Hijack_VerLanguageNameW;
FARPROC Hijack_VerQueryValueA;
FARPROC Hijack_VerQueryValueW;
}

HMODULE m_hModule = NULL;
FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
    FARPROC fpAddress;
    fpAddress = GetProcAddress(m_hModule, pszProcName);
    if (fpAddress == NULL)
    {
    }
    return fpAddress;
}

int _local_init_proclist()
{
    wchar_t path[MAX_PATH] = {};
    GetSystemDirectory(path, MAX_PATH);
    wcscat_s(path, L"\\");
    wcscat_s(path, L"version.dll");
    m_hModule = LoadLibrary(path);

    Hijack_GetFileVersionInfoA = GetAddress("GetFileVersionInfoA");
    Hijack_GetFileVersionInfoByHandle = GetAddress("GetFileVersionInfoByHandle");
    Hijack_GetFileVersionInfoExA = GetAddress("GetFileVersionInfoExA");
    Hijack_GetFileVersionInfoExW = GetAddress("GetFileVersionInfoExW");
    Hijack_GetFileVersionInfoSizeA = GetAddress("GetFileVersionInfoSizeA");
    Hijack_GetFileVersionInfoSizeExA = GetAddress("GetFileVersionInfoSizeExA");
    Hijack_GetFileVersionInfoSizeExW = GetAddress("GetFileVersionInfoSizeExW");
    Hijack_GetFileVersionInfoSizeW = GetAddress("GetFileVersionInfoSizeW");
    Hijack_GetFileVersionInfoW = GetAddress("GetFileVersionInfoW");
    Hijack_VerFindFileA = GetAddress("VerFindFileA");
    Hijack_VerFindFileW = GetAddress("VerFindFileW");
    Hijack_VerInstallFileA = GetAddress("VerInstallFileA");
    Hijack_VerInstallFileW = GetAddress("VerInstallFileW");
    Hijack_VerLanguageNameA = GetAddress("VerLanguageNameA");
    Hijack_VerLanguageNameW = GetAddress("VerLanguageNameW");
    Hijack_VerQueryValueA = GetAddress("VerQueryValueA");
    Hijack_VerQueryValueW = GetAddress("VerQueryValueW");

    return 1;
}

int g_init_export_64_ = _local_init_proclist();

#pragma comment(linker, "/export:GetFileVersionInfoA=HijackGetFileVersionInfoA,@1")
#pragma comment(linker, "/export:GetFileVersionInfoByHandle=HijackGetFileVersionInfoByHandle,@2")
#pragma comment(linker, "/export:GetFileVersionInfoExA=HijackGetFileVersionInfoExA,@3")
#pragma comment(linker, "/export:GetFileVersionInfoExW=HijackGetFileVersionInfoExW,@4")
#pragma comment(linker, "/export:GetFileVersionInfoSizeA=HijackGetFileVersionInfoSizeA,@5")
#pragma comment(linker, "/export:GetFileVersionInfoSizeExA=HijackGetFileVersionInfoSizeExA,@6")
#pragma comment(linker, "/export:GetFileVersionInfoSizeExW=HijackGetFileVersionInfoSizeExW,@7")
#pragma comment(linker, "/export:GetFileVersionInfoSizeW=HijackGetFileVersionInfoSizeW,@8")
#pragma comment(linker, "/export:GetFileVersionInfoW=HijackGetFileVersionInfoW,@9")
#pragma comment(linker, "/export:VerFindFileA=HijackVerFindFileA,@10")
#pragma comment(linker, "/export:VerFindFileW=HijackVerFindFileW,@11")
#pragma comment(linker, "/export:VerInstallFileA=HijackVerInstallFileA,@12")
#pragma comment(linker, "/export:VerInstallFileW=HijackVerInstallFileW,@13")
#pragma comment(linker, "/export:VerLanguageNameA=HijackVerLanguageNameA,@14")
#pragma comment(linker, "/export:VerLanguageNameW=HijackVerLanguageNameW,@15")
#pragma comment(linker, "/export:VerQueryValueA=HijackVerQueryValueA,@16")
#pragma comment(linker, "/export:VerQueryValueW=HijackVerQueryValueW,@17")

#endif
