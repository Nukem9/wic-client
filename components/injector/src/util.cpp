#include "stdafx.h"

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