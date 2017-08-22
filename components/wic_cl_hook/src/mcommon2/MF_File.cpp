#include "../stdafx.h"

bool MF_File::GetFileInfo(const char *aFileName, uint& aReturnDateTime, uint& aReturnSize, char *aSDFName)
{
	return ((bool(__cdecl *)(const char *, uint&, uint&, char *))0x009FDC80)(aFileName, aReturnDateTime, aReturnSize, aSDFName);
}