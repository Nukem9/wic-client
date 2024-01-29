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
#pragma once

#include "MC_HeapImplementation.h"

// Static debug class for string output to file

#if !MC_NO_DEBUG_FILE_OUTPUT
#define MC_ASSERT(x)			do { if (!(x)) { __debugbreak(); } } while (0)
#define MC_ASSERTMSG(x, msg)	do { if (!(x)) { __debugbreak(); } } while (0)
#define MC_DEBUG				(MC_Debug::InternalPosTracer(MC_Debug::TRACE_DEBUG, __FILE__, __FUNCTION__, __LINE__))
#define MC_DEBUG_SPAM			(MC_Debug::InternalPosTracer(MC_Debug::TRACE_DEBUG_SPAM, __FILE__, __FUNCTION__, __LINE__))
#define MC_ERROR				(MC_Debug::InternalPosTracer(MC_Debug::TRACE_ERROR, __FILE__, __FUNCTION__, __LINE__))
#define MC_DEBUGPOS				(MC_Debug::OutputPos(__FILE__, __FUNCTION__, __LINE__))
#else
#define MC_ASSERT(x)			__noop
#define MC_ASSERTMSG(x, msg)	__noop
#define MC_DEBUG				__noop
#define MC_DEBUG_SPAM			__noop
#define MC_ERROR				__noop
#define MC_DEBUGPOS				__noop
#endif

// Base class for listeners. Subclasses must implement DebugMessage() and Destroy().
// 
// NOTE: The DebugMessage() function must be threadsafe!
// NOTE: Do avoid the use of heap and CRT, if possible, in the DebugMessage function.
class MC_DebugListener
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

private:
	bool myBriefErrorsFlag = false;

public:
	MC_DebugListener() = default;
	virtual ~MC_DebugListener() = default;
	virtual void DebugMessage(const char* aMessage) = 0;
	virtual void Destroy() = 0;
	virtual void Commit() = 0; // Commit to disk
};
static_assert_size(MC_DebugListener, 0x8);

// Static class used to report debug messages.
//
// Call one of the Init() methods at app start.
// Call Cleanup() at app close to delete active listeners and output any stat data.
class MC_Debug
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

public:
	enum TraceType
	{
		TRACE_DEBUG,
		TRACE_DEBUG_SPAM,
		TRACE_ERROR,
	};

	// For automatic position tracking. Don't use directly.
	class InternalPosTracer
	{
		MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

	public:
		InternalPosTracer(TraceType aType, const char* aFile, const char* aFunction, u32 aLine);
		void __cdecl operator()(const char* aDebugMessage = nullptr, ...);

		TraceType myType;
		const char* myFile;
		const char* myFunction;
		const u32 myLine;
	};
	static_assert_size(InternalPosTracer, 0x10);

	MC_Debug() = delete;
	~MC_Debug() = delete;

	static bool AddDebugListener(MC_DebugListener *aListener);
};