#include "../stdafx.h"
#include <regex>
#include <string>

decltype(&MN_NetRequester::Get) origGet;

void MN_NetRequester::InitializeHook()
{
	*(uint8_t **)&origGet = Detours::X86::DetourFunctionClass((uint8_t *)0x00B92EB0, &MN_NetRequester::Get);
}

bool MN_NetRequester::Get(MN_NetRequester *aAltRequester, MN_INetHandler *aHandler, const char *aUrl, uint64 aResumePosition)
{
	// Rewrite old URL hostnames so HTTP request headers are correct
	std::string rewriteUrl(aUrl);
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("massive\\.se"), "massgate.org");
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("massgate\\.net"), "massgate.org");
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("ubisoft\\.com"), "massgate.org");

	return (this->*origGet)(aAltRequester, aHandler, rewriteUrl.c_str(), aResumePosition);
}