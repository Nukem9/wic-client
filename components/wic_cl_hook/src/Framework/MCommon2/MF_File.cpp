#include "MF_File.h"

bool MF_File::Write(const void *someData, u32 aLength)
{
#if WIC_MP
	return ((bool(__thiscall *)(MF_File *, const void *, u32))0x009FD1C0)(this, someData, aLength);
#elif WIC_DS
	return ((bool(__thiscall *)(MF_File *, const void *, u32))0x00423BF0)(this, someData, aLength);
#else
#error Unimplemented
#endif
}

const char *MF_File::ExtractExtension(const char *aPath)
{
	MC_ASSERT(aPath);

	if (!aPath)
		return "";

	const char *lastPoint = nullptr;

	while (*aPath)
	{
		if (*aPath == '/' || *aPath == '\\' || *aPath == ' ')
			lastPoint = nullptr;
		else if (*aPath == '.')
			lastPoint = aPath;

		aPath++;
	}

	return lastPoint ? (lastPoint + 1) : aPath;
}

bool MF_File::GetFileInfo(const char *aFileName, u32& aReturnDateTime, u32& aReturnSize, char *aSDFName)
{
#if WIC_MP
	return ((bool(__cdecl *)(const char *, u32&, u32&, char *))0x009FDC80)(aFileName, aReturnDateTime, aReturnSize, aSDFName);
#elif WIC_DS
	return ((bool(__cdecl *)(const char *, u32&, u32&, char *))0x00424620)(aFileName, aReturnDateTime, aReturnSize, aSDFName);
#else
#error Unimplemented
#endif
}