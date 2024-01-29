#include <Hooks/Hooks.h>
#include <string>
#include <regex>
#include "MN_NetRequester.h"

#if WIC_MP
bool __fastcall MN_NetRequester__Get(MN_NetRequester *thisptr, void *_EDX, MN_NetRequester *aAltRequester, MN_INetHandler *aHandler, const char *aUrl, uint64_t aResumePosition);
decltype(&MN_NetRequester__Get) OriginalGet;
#elif WIC_ESPORTS_PRODUCER_TOOL
bool hk_MN_NetRequester__Get(MN_NetRequester *aThis, MN_INetHandler *aHandler, uint64_t aResumePosition);
decltype(&hk_MN_NetRequester__Get) OriginalGet;
#endif

//
// Rewrite old URL hostnames so HTTP request headers are correct
//
DECLARE_HOOK_INITIALIZER_MP(MN_NetRequester,
{
	Hooks::WriteJump(0x00B92EB0, &MN_NetRequester__Get, reinterpret_cast<void **>(&OriginalGet));

//#ifdef WIC_ESPORTS_PRODUCER_TOOL
//		Hooks::WriteJump(0x00BA16D0, &hk_MN_NetRequester__Get, reinterpret_cast<void **>(&OriginalGet));
//#endif
});

#if WIC_MP
bool __fastcall MN_NetRequester__Get(MN_NetRequester *thisptr, void *_EDX, MN_NetRequester *aAltRequester, MN_INetHandler *aHandler, const char *aUrl, uint64_t aResumePosition)
{
	std::string rewriteUrl(aUrl);
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("massive\\.se"), "massgate.org");
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("massgate\\.net"), "massgate.org");
	rewriteUrl = std::regex_replace(rewriteUrl, std::regex("ubisoft\\.com"), "massgate.org");

	return OriginalGet(thisptr, _EDX, aAltRequester, aHandler, rewriteUrl.c_str(), aResumePosition);
}
#elif WIC_ESPORTS_PRODUCER_TOOL
bool MN_NetRequester__Get(MN_NetRequester *aThis, MN_INetHandler *aHandler, const char *aUrl, uint64_t aResumePosition)
{
	__asm
	{
		push dword ptr ds:[aResumePosition + 4]
		push dword ptr ds:[aResumePosition + 0]
		mov eax, aUrl
		push aHandler
		push aThis
		call [OriginalGet]
	}
}

bool __declspec(naked) hk_MN_NetRequester__Get(MN_NetRequester *aThis, MN_INetHandler *aHandler, uint64_t aResumePosition)
{
	const char *url;
	bool returnValue;

	__asm
	{
		push ebp
		mov ebp, esp
		sub esp, __LOCAL_SIZE
		mov url, eax
	}

	returnValue = [](MN_NetRequester *aThis, MN_INetHandler *aHandler, const char *aUrl, uint64_t aResumePosition)
	{
		std::string rewriteUrl(aUrl);
		rewriteUrl = std::regex_replace(rewriteUrl, std::regex("massive\\.se"), "massgate.org");
		rewriteUrl = std::regex_replace(rewriteUrl, std::regex("massgate\\.net"), "massgate.org");
		rewriteUrl = std::regex_replace(rewriteUrl, std::regex("ubisoft\\.com"), "massgate.org");

		return MN_NetRequester__Get(aThis, aHandler, rewriteUrl.c_str(), aResumePosition);
	}(aThis, aHandler, url, aResumePosition);

	__asm
	{
		mov al, returnValue
		leave
		retn 0x10
	}
}
#endif