#pragma once

class MF_File
{
public:
	static void InitializeHook();

	const char *__fastcall ExtractExtension(const char *aPath);
	static bool GetFileInfo(const char *aFileName, uint32_t& aReturnDateTime, uint32_t& aReturnSize, char *aSDFName);
};