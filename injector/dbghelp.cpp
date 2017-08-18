#include "dbghelp.h"

BOOL WINAPI MiniDumpWriteDump(HANDLE hProcess, DWORD ProcessId, HANDLE hFile, int DumpType, void *ExceptionParam, void *UserStreamParam, void *CallbackParam)
{
	return FALSE;
}