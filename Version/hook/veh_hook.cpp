#include <windows.h>
#include <TlHelp32.h>

static void SetThreadHardBreakPoint(HANDLE hThread, size_t addr)
{
	CONTEXT dst_context = {};
	memset(&dst_context, 0, sizeof(dst_context));
	dst_context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	//SuspendThread(hThread);
	GetThreadContext(hThread, &dst_context);

	dst_context.Dr1 = addr;
	dst_context.Dr7 |= (1 << 2 * 1);	// 0=Drx*2

	dst_context.Dr7 |= (DWORD)0x0 << (12 + 4 * 1);  //exe
	//dst_context.Dr7 |= (DWORD)0x1 << (12 + 4 * 0);  //w
	//dst_context.Dr7 |= (DWORD)0x3 << (12 + 4 * 0);  //rw

	// Bit位范围	名称	描述
	// 0	L0	本地断点0使能
	// 1	G0	全局断点0使能
	// 2	L1	本地断点1使能
	// 3	G1	全局断点1使能
	// 4	L2	本地断点2使能
	// 5	G2	全局断点2使能
	// 6	L3	本地断点3使能
	// 7	G3	全局断点3使能
	// 8	LE	本地DR使能扩展位
	// 9	GE	全局DR使能扩展位
	// 10-11	保留	保留位（未使用）
	// 12-13	RW0	断点0的读/写控制
	// 14-15	LEN0	断点0的长度控制
	// 16-17	RW1	断点1的读/写控制
	// 18-19	LEN1	断点1的长度控制
	// 20-21	RW2	断点2的读/写控制
	// 22-23	LEN2	断点2的长度控制
	// 24-25	RW3	断点3的读/写控制
	// 26-27	LEN3	断点3的长度控制
	// 28-31	保留	保留位（未使用）

	SetThreadContext(hThread, &dst_context);
	//ResumeThread(hThread);
}

static LONG __stdcall VEH_ExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	//if (ExceptionInfo->ExceptionRecord->ExceptionAddress == (PVOID)MessageBox)
	//{
	//	size_t* p = (size_t*)ExceptionInfo->ContextRecord->Esp;

	//	const wchar_t* msg = L"hook veh test.";
	//	p[2] = (size_t)msg;

	//	ExceptionInfo->ContextRecord->Eip = (size_t)ExceptionInfo->ContextRecord->Eip + 2;

	//	return EXCEPTION_CONTINUE_EXECUTION;
	//}

	return EXCEPTION_CONTINUE_SEARCH;
}

void AddNewThreadHook(size_t hookAddress)
{
	SetThreadHardBreakPoint(GetCurrentThread(), hookAddress);
}

void InitVehHook(size_t hookAddress)
{
	{
		typedef PVOID(WINAPI* FNAddVectoredExceptionHandler)(ULONG First, PVECTORED_EXCEPTION_HANDLER Handler);
		FNAddVectoredExceptionHandler pfnAddVectoredExceptionHandler = (FNAddVectoredExceptionHandler)GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "AddVectoredExceptionHandler");
		if (pfnAddVectoredExceptionHandler == NULL)
		{
			return;
		}
		pfnAddVectoredExceptionHandler(FALSE, VEH_ExceptionFilter);
	}

	{
		THREADENTRY32 te = {};
		te.dwSize = sizeof(THREADENTRY32);

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, GetCurrentProcessId());
		if (hSnapshot == INVALID_HANDLE_VALUE) 
		{
			return;
		}

		for (BOOL next = Thread32First(hSnapshot, &te); next; next = Thread32Next(hSnapshot, &te)) 
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
			if (hThread != NULL)
			{
				SetThreadHardBreakPoint(hThread, hookAddress);
				CloseHandle(hThread);
			}
		}

		CloseHandle(hSnapshot);
	}
}
