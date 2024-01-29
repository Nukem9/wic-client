#include <Windows.h>
#include "EX3D_DebugConsoleListener.h"
#include "EX3D_Console.h"

void EX3D_DebugConsoleListener::DebugMessage(const char *Message)
{
	if (EX3D_Console::ourInstance)
		EX3D_Console::ourInstance->StuffText(Message);

	OutputDebugStringA(Message);
	OutputDebugStringA("\n");
}

void EX3D_DebugConsoleListener::Destroy()
{
}

void EX3D_DebugConsoleListener::Commit()
{
}

EX3D_DebugConsoleListener *EX3D_DebugConsoleListener::Create()
{
	return new EX3D_DebugConsoleListener;
}
