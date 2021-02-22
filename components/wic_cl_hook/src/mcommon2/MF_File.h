#pragma once

class MF_File
{
public:
	static void InitializeHook();

	const char *ExtractExtension(const char *aPath);
	static bool GetFileInfo(const char *aFileName, uint& aReturnDateTime, uint& aReturnSize, char *aSDFName);
};