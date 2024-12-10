#ifndef _WIN64

#include <windows.h>
const wchar_t * exportDllName = L"version.dll";

size_t GetTargetAddress(const char* name)
{
	wchar_t path[MAX_PATH] = {};
	GetSystemDirectory(path, MAX_PATH);
	wcscat_s(path, L"\\");
	wcscat_s(path, exportDllName);

	auto hModule = LoadLibrary(path);
	if (hModule == NULL)
	{
		return NULL;
	}

	return (size_t)GetProcAddress(hModule, name);
}

#define EXPORT_TYPE extern "C" __declspec(naked) void __cdecl

#define DEFINE_PROC_IMPL(name)				\
EXPORT_TYPE name##__stub(void)				\
{											\
	static size_t addr = 0;					\
	if (addr == 0)							\
	{										\
		addr = GetTargetAddress(#name);		\
	}										\
											\
	__asm jmp addr							\
}											\

#define DEFINE_PROC_EXPORT(name, id)		\
__pragma( comment(linker, "/EXPORT:"## #name##"=_"## #name"__stub" ##",@" ## #id ));

#define DEFINE_EXPORT_PROC(id, name)	\
	DEFINE_PROC_IMPL(name)				\
	DEFINE_PROC_EXPORT(name, id)		\
	
	
DEFINE_EXPORT_PROC(1, GetFileVersionInfoA)
DEFINE_EXPORT_PROC(2, GetFileVersionInfoByHandle)
DEFINE_EXPORT_PROC(3, GetFileVersionInfoExA)
DEFINE_EXPORT_PROC(4, GetFileVersionInfoExW)
DEFINE_EXPORT_PROC(5, GetFileVersionInfoSizeA)
DEFINE_EXPORT_PROC(6, GetFileVersionInfoSizeExA)
DEFINE_EXPORT_PROC(7, GetFileVersionInfoSizeExW)
DEFINE_EXPORT_PROC(8, GetFileVersionInfoSizeW)
DEFINE_EXPORT_PROC(9, GetFileVersionInfoW)
DEFINE_EXPORT_PROC(10, VerFindFileA)
DEFINE_EXPORT_PROC(11, VerFindFileW)
DEFINE_EXPORT_PROC(12, VerInstallFileA)
DEFINE_EXPORT_PROC(13, VerInstallFileW)
DEFINE_EXPORT_PROC(14, VerLanguageNameA)
DEFINE_EXPORT_PROC(15, VerLanguageNameW)
DEFINE_EXPORT_PROC(16, VerQueryValueA)
DEFINE_EXPORT_PROC(17, VerQueryValueW)

#endif