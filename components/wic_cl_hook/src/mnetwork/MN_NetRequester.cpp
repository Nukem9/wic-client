#include "../stdafx.h"
#include <string>
#include <regex>

decltype(&MN_NetRequester::Get) OriginalGet;

void MN_NetRequester::InitializeHook()
{
	*reinterpret_cast<uint8_t **>(&OriginalGet) = Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x00B92EB0), &MN_NetRequester::Get);
}

bool MN_NetRequester::Get(MN_NetRequester *aAltRequester, MN_INetHandler *aHandler, const char *aUrl, uint64 aResumePosition)
{
	std::string rewriteUrl(aUrl);
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("massive\\.se"), "massgate.org");
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("massgate\\.net"), "massgate.org");
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("ubisoft\\.com"), "massgate.org");

	return (this->*OriginalGet)(aAltRequester, aHandler, rewriteUrl.c_str(), aResumePosition);
}