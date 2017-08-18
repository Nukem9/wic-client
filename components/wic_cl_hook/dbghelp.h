#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern "C" __declspec(dllexport) BOOL WINAPI MiniDumpWriteDump(HANDLE hProcess, DWORD ProcessId, HANDLE hFile, int DumpType, void *ExceptionParam, void *UserStreamParam, void *CallbackParam);