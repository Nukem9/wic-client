#include <Hooks/Hooks.h>
#include "MF_File.h"

const char *__fastcall hk_MF_File__ExtractExtension(void *_ECX, const char *aPath);

//
// Fix an out of bounds access when files without an extension are present in the game or mod folders
//
DECLARE_HOOK_INITIALIZER_MP(MF_File,
{
	Hooks::WriteJump(0x009FCBB0, &hk_MF_File__ExtractExtension);
});

DECLARE_HOOK_INITIALIZER_DS(MF_File,
{
	Hooks::WriteJump(0x00423EE0, &MF_File::ExtractExtension);
});

const char *__fastcall hk_MF_File__ExtractExtension(void *_ECX, const char *aPath)
{
	return MF_File::ExtractExtension(aPath);
}