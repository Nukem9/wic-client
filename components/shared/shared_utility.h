#pragma once

#include <Windows.h>
#include <stdint.h>

static void PatchMemory(uintptr_t Address, const uint8_t *Data, size_t Size)
{
	DWORD d = 0;
	VirtualProtect(reinterpret_cast<LPVOID>(Address), Size, PAGE_EXECUTE_READWRITE, &d);

	for (uintptr_t i = Address; i < (Address + Size); i++)
		*reinterpret_cast<volatile uint8_t *>(i) = *Data++;

	VirtualProtect(reinterpret_cast<LPVOID>(Address), Size, d, &d);
	FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<LPVOID>(Address), Size);
}

static void PatchMemory(uintptr_t Address, std::initializer_list<uint8_t> Data)
{
	PatchMemory(Address, Data.begin(), Data.size());
}