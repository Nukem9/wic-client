#pragma once

class MF_File
{
public:
	bool Write(const void* someData, u32 aLength);

	static const char *ExtractExtension(const char *aPath);
	static bool GetFileInfo(const char *aFileName, u32& aReturnDateTime, u32& aReturnSize, char *aSDFName);
};