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
#include "MC_CommandLine.h"

/*
 * Private constructor. Use Create()
 */
MC_CommandLine::MC_CommandLine()
{
}

/*
 * Private destructor. Use Destroy()
 */
MC_CommandLine::~MC_CommandLine()
{
	for (auto& arg : myArguments)
	{
		if (arg->myArgument)
			delete[] arg->myArgument;

		if (arg->myValue)
			delete[] arg->myValue;
	}

	myArguments.clear();
}

/*
 * Destroy
 *	Static method to delete object instance.
 */
void MC_CommandLine::Destroy()
{
	if (ourInstance)
		delete ourInstance;

	ourInstance = nullptr;
}

/*
 * Create
 *	Static method to create object instance.
 *	aCommandLine - the commandLine to parse. If no line is passed it will get the commandline using GetCommandLine()
 *  returns true if creation worked.
 */
/*
bool MC_CommandLine::Create(const char* aCommandLine)
{
	bool haveCommand;
	bool insideQuotes = false;
	const char* cmdLine;
	unsigned int i;
	int begin;

	ArgumentStruct* newArgument = NULL;

	if (ourInstance)
		return true;

	ourInstance = new MC_CommandLine;

	if (!ourInstance)
		return false;

	char* cmdLineBuf = ourInstance->myCommandLineBuf;
	cmdLineBuf[255] = 0;

	ourInstance->myArguments.Init(2, 2, false);

	if (aCommandLine == NULL)
		strncpy(cmdLineBuf, GetCommandLine(), 255);
	else
		strncpy(cmdLineBuf, aCommandLine, 255);

	strlwr(cmdLineBuf);
	cmdLine = cmdLineBuf;

	if (!cmdLine)
		return false;

	cmdLine = strstr(cmdLineBuf, " ");  // Find first space.. hopefully this is the first char after the executable.
	if (cmdLine)
		cmdLine = strstr(cmdLine, "-");  // Space found, now find the first argument (which hopefully isn't a part of the executable name/path
	else
		cmdLine = strstr(cmdLineBuf, "-"); // No space (could be that the commandline only contains the executable

	// If there's no commandline after the exe, that's ok too.
	if (!cmdLine)
		return true;

	haveCommand = false;
	const unsigned int cmdLineLength = strlen(cmdLine);
	for (i = 0; i <= cmdLineLength; i++)
	{
		if ((cmdLine[i] == ' ' && !insideQuotes) || !cmdLine[i])  // if space or end of line
		{
			if (!newArgument)
			{
				newArgument = new ArgumentStruct;
				newArgument->myArgument = NULL;
				newArgument->myValue = NULL;
				haveCommand = false;
			}

			if (newArgument && newArgument->myArgument == NULL)
			{
				haveCommand = true;
				newArgument->myArgument = new char[i];
				memset(newArgument->myArgument, 0, i);
				strncpy(newArgument->myArgument, cmdLine + begin, i - begin);
				_strlwr(newArgument->myArgument);  // Treat all commands as lowercase.
				begin = i + 1; // Begin of coming value

				if (!cmdLine[i])
				{
					ourInstance->myArguments.Add(newArgument);
					newArgument = NULL;
				}
			}
			else if (newArgument)
			{
				newArgument->myValue = new char[i - begin + 1];
				memset(newArgument->myValue, 0, i - begin + 1);
				strncpy(newArgument->myValue, cmdLine + begin, i - begin);

				ourInstance->myArguments.Add(newArgument);
				newArgument = NULL;
				haveCommand = false;
			}
		}
		else if (cmdLine[i] == '-' && !insideQuotes)  // A new command begins here.
		{
			if (haveCommand)
			{
				// We have a command but no argument. Save command without argument.
				ourInstance->myArguments.Add(newArgument);
				haveCommand = false;
				newArgument = NULL;
			}
			begin = i + 1;
		}
		else if (cmdLine[i] == '"') // Dont split things between ":s
		{
			if (insideQuotes) // second quote, store!
			{
				if (!newArgument)
				{
					newArgument = new ArgumentStruct;
					newArgument->myArgument = NULL;
					newArgument->myValue = NULL;
					haveCommand = false;
				}

				if (newArgument && newArgument->myArgument == NULL)
				{
					haveCommand = true;
					newArgument->myArgument = new char[i];
					memset(newArgument->myArgument, 0, i);
					strncpy(newArgument->myArgument, cmdLine + begin, i - begin);
					_strlwr(newArgument->myArgument);  // Treat all commands as lowercase.
					begin = i + 1; // Begin of coming value

					if (!cmdLine[i])
					{
						ourInstance->myArguments.Add(newArgument);
						newArgument = NULL;
					}
				}
				else if (newArgument)
				{
					newArgument->myValue = new char[i - begin + 1];
					memset(newArgument->myValue, 0, i - begin + 1);
					strncpy(newArgument->myValue, cmdLine + begin, i - begin);

					ourInstance->myArguments.Add(newArgument);
					newArgument = NULL;
					haveCommand = false;
				}
				insideQuotes = false;
			}
			else
			{
				begin = i + 1;
				insideQuotes = true;

			}
		}
	}

	return true;
}
*/

/*
 * IsPresent
 *	Checks to see if a command existed on the commandline whether the command had an argument or not.
 *  anArgument - argument to check for
 *	returns true if found.
 */
bool MC_CommandLine::IsPresent(const char* anArgument)
{
	for (const auto& arg : myArguments)
	{
		if (arg && !strcmp(arg->myArgument, anArgument))
			return true;
	}

	return false;
}

/*
 * GetStringValue
 *	Sets aReturnValue to the anArguments argument value.
 *  anArgument - argument to check for
 *	aReturnValue - a reference that will contain the value
 *	returns true if found and the command had an argument.
 */
bool MC_CommandLine::GetStringValue(const char* anArgument, const char*& aReturnValue)
{
	for (const auto& arg : myArguments)
	{
		if (arg && !strcmp(arg->myArgument, anArgument))
		{
			if (!arg->myValue)
				return false;

			aReturnValue = arg->myValue;
			return true;
		}
	}

	return false;
}

/*
 * GetIntValue
 *	Sets aReturnValue to the anArguments argument value.
 *  anArgument - argument to check for
 *	aReturnValue - a reference that will contain the value
 *	returns true if found and the command had an argument.
 */
bool MC_CommandLine::GetIntValue(const char* anArgument, int& aReturnValue)
{
	if (const char *retVal; GetStringValue(anArgument, retVal))
	{
		aReturnValue = atoi(retVal);
		return true;
	}

	return false;
}

/*
 * GetFloatValue
 *	Sets aReturnValue to the anArguments argument value.
 *  anArgument - argument to check for
 *	aReturnValue - a reference that will contain the value
 *	returns true if found and the command had an argument.
 */
bool MC_CommandLine::GetFloatValue(const char* anArgument, float& aReturnValue)
{
	if (const char *retVal; GetStringValue(anArgument, retVal))
	{
		aReturnValue = std::strtof(retVal, nullptr);
		return true;
	}

	return false;
}

/*
 * Add
 *	Adds a commandline argument programatically. Mostly used in debugging.
 *	No parsing of the argument or value is done so make sure they are correct when adding.
 *  anArgument - argument to add
 *	aValue - a value of the argument. May be NULL.
 *	returns true if add succeeded
 */
/*
bool MC_CommandLine::Add(const char* anArgument, const char* aValue)
{
	if (!anArgument)
		false;

	if (IsPresent(anArgument))
		return false;

	// The argument was new. Now create it.
	auto newArgument = new ArgumentStruct;

	if (!newArgument)
		return false;

	// Copy argument
	newArgument->myArgument = new char[strlen(anArgument) + 1];
	strcpy(newArgument->myArgument, anArgument);

	if (aValue)
	{
		// copy value
		newArgument->myValue = new char[strlen(aValue) + 1];
		strcpy(newArgument->myValue, aValue);
	}

	myArguments.push_back(newArgument);
	return true;
}
*/