#pragma once

class MC_DebugConsoleListener
{
public:
	MC_DebugConsoleListener();

	// These virtuals are in a specific order
	virtual ~MC_DebugConsoleListener();
	virtual void DebugMessage(const char *Message);
	virtual void Destroy();
	virtual void Commit();
};