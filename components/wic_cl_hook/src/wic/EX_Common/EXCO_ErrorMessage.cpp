#include "EXCO_ErrorMessage.h"

void EXCO_ErrorMessage::AddError(const wchar_t *anErrorString, Error anError, const char *aDestinationGUI)
{
	const static u32 dwCall = 0x00995680;

	__asm
	{
		// aDestinationGUI appears to be discarded
		mov edi, this
		mov esi, anErrorString
		push anError
		call [dwCall]
	}
}