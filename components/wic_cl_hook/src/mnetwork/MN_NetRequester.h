#pragma once

class MN_NetRequester
{
public:
	static void InitializeHook();

	bool Get(MN_NetRequester *aAltRequester, MN_INetHandler *aHandler, const char *aUrl, uint64_t aResumePosition);

private:
};