#pragma once

#include <MCommon2/MC_HeapImplementation.h>

class EX3D_DebugConsoleListener : public MC_DebugListener
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

private:
	EX3D_DebugConsoleListener() = default;

public:
	virtual ~EX3D_DebugConsoleListener() = default;
	virtual void DebugMessage(const char *Message) override;
	virtual void Destroy() override;
	virtual void Commit() override;

	static EX3D_DebugConsoleListener *Create();
};