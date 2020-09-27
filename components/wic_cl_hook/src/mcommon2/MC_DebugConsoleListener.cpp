#include "../stdafx.h"

MC_DebugListener::MC_DebugListener()
{
	myBriefErrorsFlag = false;
}

MC_DebugListener::~MC_DebugListener()
{
}

MC_DebugConsoleListener::MC_DebugConsoleListener()
{
}

MC_DebugConsoleListener::~MC_DebugConsoleListener()
{
}

void MC_DebugConsoleListener::DebugMessage(const char *Message)
{
	if (EX3D_Console::ourInstance)
		EX3D_Console::ourInstance->StuffText(Message);

	OutputDebugStringA(Message);
}

void MC_DebugConsoleListener::Destroy()
{
}

void MC_DebugConsoleListener::Commit()
{
}