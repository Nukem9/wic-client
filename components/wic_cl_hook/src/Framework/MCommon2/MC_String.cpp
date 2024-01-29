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
#include <stdarg.h>
#include <string.h>
#include <Windows.h>
#include <tchar.h>
#include "MC_String.h"

//static int NullData_For_MC_String2 = 0;
//void* g_static_ref_to_null_data_mc_string2 = &NullData_For_MC_String2;
DeclareGlobalPtr(void, g_static_ref_to_null_data_mc_string2, SelectOffset(0x00DFFE0C, 0x00871310));

errno_t MC_tcsupr_s(TCHAR *_String, size_t _Count)
{
	return _tcsupr_s(_String, _Count);
}

errno_t MC_tcslwr_s(TCHAR *_String, size_t _Count)
{
	return _tcslwr_s(_String, _Count);
}

int MC_tcscmp(const TCHAR *_s1, const TCHAR *_s2)
{
	return _tcscmp(_s1, _s2);
}

int MC_tcsicmp(const TCHAR *_s1, const TCHAR *_s2)
{
	return _tcsicmp(_s1, _s2);
}

const TCHAR *MC_tcsstr(const TCHAR *_s1, const TCHAR *_s2)
{
	return _tcsstr(_s1, _s2);
}

size_t MC_tcslen(TCHAR const *_Str)
{
	return _tcslen(_Str);
}

int MC_tcsncmp(const TCHAR *_s1, const TCHAR *_s2, size_t _n)
{
	return _tcsncmp(_s1, _s2, _n);
}

int MC_tcsncicmp(const TCHAR *_s1, const TCHAR *_s2, size_t _n)
{
	return _tcsncicmp(_s1, _s2, _n);
}

const char* MC_Stristr(const char* aString, const char* aSubString)
{
	if (aString == 0)
		return 0;

	if (aSubString == 0 || *aSubString == 0)
		return aString;

	const int stringLen = (int)_tcslen(aString);
	const int subStringLen = (int)_tcslen(aSubString);

	for (int i = 0; i <= stringLen - subStringLen; i++)
	{
		if (_tcsncicmp(aString + i, aSubString, subStringLen) == 0)
			return aString + i;
	}

	return NULL;
}

static bool find_in(const char* aString, char anItem)
{
	for (const char* s = aString; *s; ++s)
		if (*s == anItem)
			return true;
	return false;
}

int MC_StringToInt(const char* const aString)
{
	unsigned int s1 = 1;
	unsigned int s2 = 0;

	const unsigned char* ptr = (const unsigned char*)aString;

	while (*ptr)
	{
		//version 0004 hash (adler32)
		s1 += *ptr++;
		s2 += s1;
	}

	s1 %= 65521;
	s2 %= 65521;

	return (s2 << 16) | s1;
}

int MC_StringToInt(const MC_LocChar* aString)
{
	unsigned int s1 = 1;
	unsigned int s2 = 0;

	while (*aString++)
	{
		const unsigned char* const charptr = (const unsigned char* const)aString;
		s1 += charptr[0];	s2 += s1;
		s1 += charptr[1];	s2 += s1;
	}

	s1 %= 65521;
	s2 %= 65521;

	return (s2 << 16) | s1;
}

bool MC_WildCardCompareNoCase(const char* aString, const char* aWildcard)
{
	while (*aString || *aWildcard)
	{
		if (*aWildcard == 0)
			return false;
		else if (*aWildcard == '*')
		{
			aWildcard++;
			if (*aWildcard == 0)
				return true;
			else
			{
				for (; *aString; aString++)
					if (MC_WildCardCompareNoCase(aString, aWildcard))
						return true;
			}

			return false;
		}
		else if (*aString == 0)
			return false;
		else if (tolower(*aWildcard) == tolower(*aString) || *aWildcard == '?')
		{
			aString++;
			aWildcard++;
		}
		else
			return false;
	}

	return true;
}

bool MC_EndsWith(const char* aString, const char* aEndString)
{
	if (aString == 0 || aEndString == 0 || *aString == 0 || *aEndString == 0)
		return false;

	const int len1 = MC_Strlen(aString);
	const int len2 = MC_Strlen(aEndString);

	if (len2 > len1)
		return false;

	return (strcmp(aString + len1 - len2, aEndString) == 0);
}

bool MC_EndsWithNoCase(const char* aString, const char* aEndString)
{
	if (aString == 0 || aEndString == 0 || *aString == 0 || *aEndString == 0)
		return false;

	const int len1 = MC_Strlen(aString);
	const int len2 = MC_Strlen(aEndString);

	if (len2 > len1)
		return false;

	return (_stricmp(aString + len1 - len2, aEndString) == 0);
}

MC_String MC_Strtok(const char** anInput, const char* aSeparator)
{
	const char* aStart = *anInput;

	// find start
	while (*aStart && find_in(aSeparator, *aStart))
		aStart++;

	const char* anEnd = aStart;

	// find end
	while (*anEnd && (find_in(aSeparator, *anEnd) == false))
		anEnd++;
	*anInput = anEnd;
	return MC_String(aStart, anEnd - aStart);
}

MC_Strfmt<64> MC_Ftoa(float f, bool aTrimFlag)
{
	MC_Strfmt<64> str("%f", f);

	if (aTrimFlag)
	{
		int len = MC_Strlen(str.myBuffer);
		for (int i = len - 1; i > 2; i--)
		{
			if (str.myBuffer[i] == '0' && str.myBuffer[i - 1] != '.')
				str.myBuffer[i] = 0;
			else
				break;
		}
	}

	return str;
}

int MC_Strlen(const char* aString)
{
	if (aString)
		return (int)_tcslen(aString);
	else
		return 0;
}

int MC_Strlen(const MC_LocChar* aString)
{
	if (aString)
		return (int)wcslen(aString);
	else
		return 0;
}

int MC_MakeUpper(MC_LocChar* aString, int aBufSize)
{
	return CharUpperBuffW(aString, aBufSize);
}

int MC_MakeLower(MC_LocChar* aString, int aBufSize)
{
	return CharLowerBuffW(aString, aBufSize);
}