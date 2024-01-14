#include "Fk.h"

using namespace std;

ULONG WINAPI Mine_GetAdaptersInfo(PIP_ADAPTER_INFO AdapterInfo, PULONG SizePointer) {
    ULONG            ret   = GetAdaptersInfo(AdapterInfo, SizePointer);
    PIP_ADAPTER_INFO first = AdapterInfo;

    for (int i = 0; first; ++i) {
        memcpy(first->Address, mac+i*8, 8);
        first = first->Next;
    }
    return ret;
}
ULONG WINAPI Mine_GetAdaptersAddresses(ULONG Family, ULONG Flags, PVOID Reserved,
                                       PIP_ADAPTER_ADDRESSES AdapterAddresses, PULONG SizePointer) {
    ULONG ret = GetAdaptersAddresses(Family, Flags, Reserved, AdapterAddresses, SizePointer);
    PIP_ADAPTER_ADDRESSES first = AdapterAddresses;

    for (int i = 0; first; ++i) {
        memcpy(first->PhysicalAddress, mac+i*8, 8);
        first = first->Next;
    }
    return ret;
}
void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO *inRemoteInfo) {
    if (inRemoteInfo->UserDataSize >= 128) {
        memcpy(mac, inRemoteInfo->UserData, 128);
    }

    HOOK_TRACE_INFO hHook = {0};
    NTSTATUS result = LhInstallHook(GetProcAddress(GetModuleHandle(L"IPHLPAPI"), "GetAdaptersInfo"),
                                    Mine_GetAdaptersInfo, nullptr, &hHook);
    if (FAILED(result)) {
        OutputDebugString(RtlGetLastErrorString());
    }
    ULONG ACLEntries[ 1 ] = {0};
    LhSetExclusiveACL(ACLEntries, 1, &hHook);

    HOOK_TRACE_INFO hHook1 = {0};
    result = LhInstallHook(GetProcAddress(GetModuleHandle(L"IPHLPAPI"), "GetAdaptersAddresses"),
                           Mine_GetAdaptersAddresses, nullptr, &hHook1);
    if (FAILED(result)) {
        OutputDebugString(RtlGetLastErrorString());
    }
    ULONG ACLEntries1[ 1 ] = {0};
    LhSetExclusiveACL(ACLEntries1, 1, &hHook1);
}
