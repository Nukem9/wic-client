#include "stdafx.h"

bool RelaunchWicExecutable(wchar_t *NewExecutable, wchar_t *OldExecutable)
{
	int argCount;
	wchar_t **args = CommandLineToArgvW(GetCommandLineW(), &argCount);

	if (!args || argCount <= 0)
		return false;

	wchar_t newCommandLine[32768];
	memset(newCommandLine, 0, sizeof(newCommandLine));

	// First copy and replace the exe name
	if (wcschr(args[0], ' ')) wcscpy_s(newCommandLine, L"\"");
	wcscat_s(newCommandLine, args[0]);

	wchar_t *oldExe = wcsstr(newCommandLine, OldExecutable);

	if (!oldExe)
	{
		LocalFree(args);
		return false;
	}

	wcscpy_s(oldExe, ARRAYSIZE(newCommandLine) - (oldExe - newCommandLine), NewExecutable);
	if (wcschr(args[0], ' ')) wcscat_s(newCommandLine, L"\"");

	// Now append the rest of the arguments
	for (int i = 1; i < argCount; i++)
	{
		wcscat_s(newCommandLine, L" ");

		if (wcschr(args[i], ' '))
		{
			// Spaces inside parameters must be contained with quotes
			wcscat_s(newCommandLine, L"\"");
			wcscat_s(newCommandLine, args[i]);
			wcscat_s(newCommandLine, L"\"");
		}
		else
		{
			// Parameter has no spaces inside it
			wcscat_s(newCommandLine, args[i]);
		}
	}

	// Launch the new process
	PROCESS_INFORMATION processInfo;
	memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));

	STARTUPINFO startupInfo;
	memset(&startupInfo, 0, sizeof(STARTUPINFOA));
	startupInfo.cb = sizeof(startupInfo);

	if (!CreateProcess(nullptr, newCommandLine, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startupInfo, &processInfo))
	{
		LocalFree(args);
		return false;
	}

	LocalFree(args);
	return true;
}

bool GetModuleVersion(wchar_t *Path, int *Major, int *Minor, int *Build, int *Revision)
{
	DWORD verHandle;
	DWORD verSize = GetFileVersionInfoSize(Path, &verHandle);

	if (verSize == 0)
		return false;

	// Allocate & read the version structures
	std::vector<BYTE> verData(verSize);
	{
		UINT size;
		LPVOID lpBuffer;

		if (!GetFileVersionInfo(Path, verHandle, verSize, verData.data()) || !VerQueryValue(verData.data(), L"\\", (LPVOID *)&lpBuffer, &size))
			return false;

		if (size < sizeof(VS_FIXEDFILEINFO))
			return false;

		// Check magic constant given by MSDN
		VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;

		if (verInfo->dwSignature != 0xFEEF04BD)
			return false;

		if (Major)
			*Major = (verInfo->dwFileVersionMS >> 16) & 0xffff;

		if (Minor)
			*Minor = (verInfo->dwFileVersionMS >> 0) & 0xffff;

		if (Build)
			*Build = (verInfo->dwFileVersionLS >> 16) & 0xffff;

		if (Revision)
			*Revision = (verInfo->dwFileVersionLS >> 0) & 0xffff;
	}

	return true;
}