#include <windows.h>

#include "ntdll.h"
#include "define.h"
#include "resolve.h"
#include "hashstr.h"
#include "xorstr.h"

// executable
EXTERN_C int
mainCRTStartup(void)
{
	API(KERNEL32, LoadLibraryA)(XORSTR("user32"));
	API(USER32, MessageBoxA)(NULL, XORSTR("Hello, world !"), XORSTR("Message"), NULL);
	return ERROR_SUCCESS;
}

// dll
EXTERN_C BOOL WINAPI
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
