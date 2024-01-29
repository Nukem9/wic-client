// Massgate
// Copyright (C) 2017 Ubisoft Entertainment
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// THIS FILE HAS BEEN MODIFIED FOR USE IN A NON-UBISOFT PROJECT. IT IS NOT
// AN ORIGINAL COPY.
//
#include <Hooks/Memory.h>
#include <ShlObj.h>
#include "MC_SystemPaths.h"

#define APPNAMESIZE 128

DeclareGlobalPtr(char, ourAppName, SelectOffset(0x00E20588, 0x008CD9B0));
DeclareGlobalPtr(char, ourDocumentsPathName, SelectOffset(0x00E20608, 0x008CDA30));

#if IS_PC_BUILD
static bool GetFolderPath(MC_String& aBasePath, const char* aPathToAppend, int aFolderFlag)
{
	char folderPath[MAX_PATH];

	if (SUCCEEDED(SHGetFolderPath(nullptr, aFolderFlag | CSIDL_FLAG_CREATE, nullptr, 0, folderPath)))
	{
		aBasePath = folderPath;

		if (strlen(ourDocumentsPathName) > 0)
		{
			if (!aBasePath.EndsWith("\\"))
				aBasePath += "\\";

			aBasePath += ourDocumentsPathName;
		}

		if (aPathToAppend)
		{
			if (!aBasePath.EndsWith("\\"))
				aBasePath += "\\";

			aBasePath += aPathToAppend;
		}

		return true;
	}

	return false;
}
#endif

void MC_SystemPaths::SetAppName(const char* anAppName, const char* aDocumentsPathName)
{
	MC_ASSERT(anAppName);
	MC_ASSERT(strlen(anAppName) < APPNAMESIZE);

	strcpy_s(ourAppName, APPNAMESIZE, anAppName);

	if (aDocumentsPathName)
	{
		MC_ASSERT(strlen(aDocumentsPathName) < APPNAMESIZE);
		strcpy_s(ourDocumentsPathName, APPNAMESIZE, aDocumentsPathName);
	}
	else
		strcpy_s(ourDocumentsPathName, APPNAMESIZE, anAppName);

}

const char* MC_SystemPaths::GetAppName()
{
	return ourAppName;
}

bool MC_SystemPaths::GetUserDocumentsPath(MC_String& aBasePath, const char* aPathToAppend)
{
	return GetFolderPath(aBasePath, aPathToAppend, CSIDL_PERSONAL);
}

bool MC_SystemPaths::GetCommonDocumentsPath(MC_String& aBasePath, const char* aPathToAppend)
{
	return GetFolderPath(aBasePath, aPathToAppend, CSIDL_COMMON_DOCUMENTS);
}

bool MC_SystemPaths::GetUserAppDataPath(MC_String& aBasePath, const char* aPathToAppend)
{
	return GetFolderPath(aBasePath, aPathToAppend, CSIDL_LOCAL_APPDATA);
}

bool MC_SystemPaths::GetCommonAppDataPath(MC_String& aBasePath, const char* aPathToAppend)
{
	return GetFolderPath(aBasePath, aPathToAppend, CSIDL_COMMON_APPDATA);
}

MC_StaticString<260> MC_SystemPaths::GetUserDocumentsFileName(const char* aFileName)
{
	const static auto cachedDocumentsPath = []() -> MC_StaticString<260>
	{
		char folderPath[MAX_PATH];

		if (FAILED(SHGetFolderPathA(nullptr, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, nullptr, 0, folderPath)))
		{
			MC_ASSERTMSG(false, "SHGetFolderPathA failed?! Permissions are either broken or an antivirus is interfering.");
			return "";
		}

		strcat_s(folderPath, "\\");
		return folderPath;
	}();

	MC_StaticString<260> path = cachedDocumentsPath;

	if (strlen(ourDocumentsPathName) > 0)
	{
		path += ourDocumentsPathName;
		path += "\\";
	}

	path += aFileName;
	return path;
}