#pragma once

#ifndef __LIB_RESOLVE_H
#define __LIB_RESOLVE_H

#include <windows.h>

#include "ntdll.h"

namespace resolve {
	HANDLE  Module(_In_ ULONG dwHash);
	FARPROC Function(_In_ const PVOID hModule, _In_ ULONG dwHash);
}

#endif // __LIB_RESOLVE_H
