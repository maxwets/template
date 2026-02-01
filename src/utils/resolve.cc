#include <windows.h>

#include "ntdll.h"
#include "define.h"
#include "hashstr.h"
#include "resolve.h"

static PIMAGE_NT_HEADERS LdrpImageHeader(_In_ PVOID Image);

HANDLE
resolve::
Module(_In_ ULONG dwHash) {
    PLDR_DATA_TABLE_ENTRY Data  = NULL;
    PLIST_ENTRY           Head  = NULL;
    PLIST_ENTRY           Entry = NULL;


    Head  = &(NtCurrentPeb()->Ldr->InLoadOrderModuleList);
    Entry = Head->Flink;

    for (; Head != Entry ; Entry = Entry->Flink) {
        Data = (PLDR_DATA_TABLE_ENTRY) (Entry);
        if (HASH_STR(Data->BaseDllName.Buffer) == dwHash)
            return (PLDR_DATA_TABLE_ENTRY) Data->DllBase;
    }

    return NULL;
}

FARPROC
resolve::
Function(_In_ const PVOID hModule, _In_ ULONG dwHash) {
    PVOID                   pAddress     = { 0 };
    PIMAGE_NT_HEADERS       NtHeader     = { 0 };
    PIMAGE_EXPORT_DIRECTORY ExpDir       = { 0 };
    SIZE_T                  ExpDirSize   = { 0 };
    PDWORD                  pdwAddrNames = { 0 };
    PDWORD                  pdwAddrFuncs = { 0 };
    PWORD                   pwAddrOrdns  = { 0 };
    PCHAR                   szFuncName   = { 0 };

    if (! hModule || ! dwHash)
        return NULL;

    if (! (NtHeader = LdrpImageHeader(hModule)))
        return NULL;

    ExpDir       = (PIMAGE_EXPORT_DIRECTORY) (U_PTR(hModule) + NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    ExpDirSize   = (SIZE_T) (NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);
    pdwAddrNames = (PDWORD) (U_PTR(hModule) + ExpDir->AddressOfNames);
    pdwAddrFuncs = (PDWORD) (U_PTR(hModule) + ExpDir->AddressOfFunctions);
    pwAddrOrdns  = (PWORD)  (U_PTR(hModule) + ExpDir->AddressOfNameOrdinals);

    for (DWORD i = 0; i < ExpDir->NumberOfNames; i++) {
        szFuncName = (PCHAR) C_PTR(U_PTR(hModule) + pdwAddrNames[i]);
        if (HASH_STR(szFuncName) != dwHash)
			continue;

        pAddress = C_PTR(U_PTR(hModule) + pdwAddrFuncs[pwAddrOrdns[i]]);
        if ((U_PTR(pAddress) >= U_PTR(ExpDir)) &&
             (U_PTR(pAddress) <  U_PTR(ExpDir) + ExpDirSize)
       ) { return NULL; }
        break;
    }

    return (FARPROC) pAddress;
}

static PIMAGE_NT_HEADERS
LdrpImageHeader(_In_ PVOID Image) {
    PIMAGE_DOS_HEADER DosHeader = { 0 };
    PIMAGE_NT_HEADERS NtHeader  = { 0 };

    DosHeader = (PIMAGE_DOS_HEADER) C_PTR(Image);

    if (DosHeader->e_magic != (IMAGE_DOS_SIGNATURE)) {
        return NULL;
    }

    NtHeader = (PIMAGE_NT_HEADERS) C_PTR(U_PTR(Image) + DosHeader->e_lfanew);

    if (NtHeader->Signature != (IMAGE_NT_SIGNATURE)) {
        return NULL;
    }

    return NtHeader;
}


