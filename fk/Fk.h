#ifndef FK_H
#define FK_H

#include "fk_global.h"
#include <winsock2.h>
extern "C" {
#include <easyhook.h>
}
#include <fstream>
#include <iostream>
#include <iphlpapi.h>
#include <random>
#include <string>

char mac[128] = {0};

extern "C" void FKSHARED_EXPORT __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO *inRemoteInfo);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}

#endif // FK_H
