#include "../stdafx.h"

void MF_File::InitializeHook()
{
	Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x009FCBB0), &ExtractExtension);
}

const char *MF_File::ExtractExtension(const char *aPath)
{
	if (!aPath)
		return "";

	auto ext = PathFindExtensionA(aPath);

	if (ext[0] == '.')
		return &ext[1];

	return ext;
}

bool MF_File::GetFileInfo(const char *aFileName, uint32_t& aReturnDateTime, uint32_t& aReturnSize, char *aSDFName)
{
	return ((bool(__cdecl *)(const char *, uint32_t&, uint32_t&, char *))0x009FDC80)(aFileName, aReturnDateTime, aReturnSize, aSDFName);
}