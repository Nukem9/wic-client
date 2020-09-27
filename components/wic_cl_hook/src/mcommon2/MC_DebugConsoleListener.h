#pragma once

class MC_DebugListener
{
public:
	bool myBriefErrorsFlag; // this+0x04

	// These virtuals are in a specific order
	MC_DebugListener();
	virtual ~MC_DebugListener();
	virtual void DebugMessage(const char *Message) = 0;
	virtual void Destroy() = 0;
	virtual void Commit() = 0;
};

class MC_DebugConsoleListener : MC_DebugListener
{
public:
	MC_DebugConsoleListener();

	virtual ~MC_DebugConsoleListener();
	virtual void DebugMessage(const char *Message) override;
	virtual void Destroy() override;
	virtual void Commit() override;
};