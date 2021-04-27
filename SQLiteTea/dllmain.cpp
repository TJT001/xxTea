// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
extern "C" {
#include "sqlite3.h"
};

#define  SQLITE3_STATIC
extern "C" int sqlite3_key(sqlite3 *db, const void *pKey, int nKey);
*/


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

