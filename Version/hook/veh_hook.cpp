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

	// Bitλ��Χ	����	����
	// 0	L0	���ضϵ�0ʹ��
	// 1	G0	ȫ�ֶϵ�0ʹ��
	// 2	L1	���ضϵ�1ʹ��
	// 3	G1	ȫ�ֶϵ�1ʹ��
	// 4	L2	���ضϵ�2ʹ��
	// 5	G2	ȫ�ֶϵ�2ʹ��
	// 6	L3	���ضϵ�3ʹ��
	// 7	G3	ȫ�ֶϵ�3ʹ��
	// 8	LE	����DRʹ����չλ
	// 9	GE	ȫ��DRʹ����չλ
	// 10-11	����	����λ��δʹ�ã�
	// 12-13	RW0	�ϵ�0�Ķ�/д����
	// 14-15	LEN0	�ϵ�0�ĳ��ȿ���
	// 16-17	RW1	�ϵ�1�Ķ�/д����
	// 18-19	LEN1	�ϵ�1�ĳ��ȿ���
	// 20-21	RW2	�ϵ�2�Ķ�/д����
	// 22-23	LEN2	�ϵ�2�ĳ��ȿ���
	// 24-25	RW3	�ϵ�3�Ķ�/д����
	// 26-27	LEN3	�ϵ�3�ĳ��ȿ���
	// 28-31	����	����λ��δʹ�ã�

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
