#pragma once

namespace WIC_EXE
{
	void WIC_WriteConsole(const char *aString, ...);
	struct hostent *PASCAL hk_gethostbyname(const char *name);
	HINSTANCE PASCAL hk_shellexecutea(HWND hwnd, LPCTSTR lpOperation, LPCTSTR lpFile, LPCTSTR lpParameters, LPCTSTR lpDirectory, INT nShowCmd);
	
	void	InitializeHook();
}