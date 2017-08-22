#pragma once

class MF_File
{
public:
	static bool GetFileInfo(const char *aFileName, uint& aReturnDateTime, uint& aReturnSize, char *aSDFName);
};