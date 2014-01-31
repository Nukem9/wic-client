#pragma once

class MC_Debug
{
public:
	enum TraceType
	{
		TRACE_DEBUG,
		TRACE_DEBUG_SPAM,
		TRACE_ERROR,
	};

	class InternalPosTracer
	{
	public:
		TraceType		myType;
		const char		*myFile;
		const char		*myFunction;
		uint			myLine;

		InternalPosTracer(TraceType aType, const char *aFile, const char *aFunction, uint aLine)
		{
			this->myType = aType;
			this->myFile = aFile;
			this->myFunction = aFunction;
			this->myLine = aLine;
		}

		void operator() (const char *aDebugMessage)
		{
			// MC_Debug::InternalPosTracer::operator()
			((void (__cdecl *)(InternalPosTracer *, const char *))0x00421180)(this, aDebugMessage);
		}
	};
};

CHECK_OFFSET(MC_Debug::InternalPosTracer, myType, 0x0);
CHECK_OFFSET(MC_Debug::InternalPosTracer, myFile, 0x4);
CHECK_OFFSET(MC_Debug::InternalPosTracer, myFunction, 0x8);
CHECK_OFFSET(MC_Debug::InternalPosTracer, myLine, 0xC);