#include <windows.h>

#include "ntdll.h"
#include "define.h"
#include "resolve.h"
#include "hashstr.h"

// executable
EXTERN_C int
mainCRTStartup(void)
{
	return ERROR_SUCCESS;
}

// dll
EXTERN_C BOOL WINAPI
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return ERROR_SUCCESS;
}
