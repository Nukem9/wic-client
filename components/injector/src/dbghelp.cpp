#include "dbghelp.h"

#pragma comment(linker, "/export:SymInitialize=dbghelp_old.SymInitialize")
#pragma comment(linker, "/export:SymCleanup=dbghelp_old.SymCleanup")
#pragma comment(linker, "/export:StackWalk64=dbghelp_old.StackWalk64")
#pragma comment(linker, "/export:SymGetOptions=dbghelp_old.SymGetOptions")
#pragma comment(linker, "/export:SymSetOptions=dbghelp_old.SymSetOptions")
#pragma comment(linker, "/export:SymFunctionTableAccess64=dbghelp_old.SymFunctionTableAccess64")
#pragma comment(linker, "/export:SymGetLineFromAddr64=dbghelp_old.SymGetLineFromAddr64")
#pragma comment(linker, "/export:SymGetModuleBase64=dbghelp_old.SymGetModuleBase64")
#pragma comment(linker, "/export:SymGetModuleInfo64=dbghelp_old.SymGetModuleInfo64")
#pragma comment(linker, "/export:SymGetSymFromAddr64=dbghelp_old.SymGetSymFromAddr64")
#pragma comment(linker, "/export:UnDecorateSymbolName=dbghelp_old.UnDecorateSymbolName")
#pragma comment(linker, "/export:SymLoadModule64=dbghelp_old.SymLoadModule64")
#pragma comment(linker, "/export:SymGetSearchPath=dbghelp_old.SymGetSearchPath")

BOOL WINAPI MiniDumpWriteDump(HANDLE hProcess, DWORD ProcessId, HANDLE hFile, int DumpType, void *ExceptionParam, void *UserStreamParam, void *CallbackParam)
{
	return FALSE;
}