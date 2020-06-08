#include "stdafx.h"

#define WIC_USE_TBB			0
#define WIC_USE_PAGE_HEAP	0

#if WIC_USE_TBB
#include <tbb/scalable_allocator.h>

void *MemAlloc(size_t Size, size_t Alignment = 0, bool Aligned = false)
{
	// If the caller doesn't care, force 4 byte aligns as a minimum
	if (!Aligned)
		Alignment = 4;

	if (Size <= 0)
	{
		Size = 1;
		Alignment = 2;
	}

	//AssertMsg(Alignment != 0 && Alignment % 2 == 0, "Alignment is fucked");

	// Must be a power of 2, round it up if needed
	if ((Alignment & (Alignment - 1)) != 0)
	{
		Alignment--;
		Alignment |= Alignment >> 1;
		Alignment |= Alignment >> 2;
		Alignment |= Alignment >> 4;
		Alignment |= Alignment >> 8;
		Alignment |= Alignment >> 16;
		Alignment++;
	}

	// Size must be a multiple of alignment, round up to nearest
	if ((Size % Alignment) != 0)
		Size = ((Size + Alignment - 1) / Alignment) * Alignment;

#if WIC_USE_PAGE_HEAP
	void *ptr = VirtualAlloc(nullptr, Size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	void *ptr = scalable_aligned_malloc(Size, Alignment);

	if (ptr)
		memset(ptr, 0, Size);
#endif

	return ptr;
}

void MemFree(void *Memory, bool Aligned = false)
{
	if (!Memory)
		return;

#if WIC_USE_PAGE_HEAP
	VirtualFree(Memory, 0, MEM_RELEASE);
#else
	scalable_aligned_free(Memory);
#endif
}

size_t MemSize(void *Memory)
{
#if WIC_USE_PAGE_HEAP
	MEMORY_BASIC_INFORMATION info;
	VirtualQuery(Memory, &info, sizeof(MEMORY_BASIC_INFORMATION));

	size_t result = info.RegionSize;
#else
	size_t result = scalable_msize(Memory);
#endif

	return result;
}

//
// CRT hijacked functions
//
void *hk_calloc(size_t Count, size_t Size)
{
	// The allocated memory is always zeroed
	return MemAlloc(Count * Size, 0, false);
}

void *hk_malloc(size_t Size)
{
	return MemAlloc(Size);
}

void *hk_aligned_malloc(size_t Size, size_t Alignment)
{
	return MemAlloc(Size, Alignment, true);
}

void *hk_aligned_realloc(void *Memory, size_t Size, size_t Alignment)
{
	void *newMemory = nullptr;

	if (Size > 0)
	{
		// Recalloc behaves like calloc if there's no existing allocation. Realloc doesn't. Zero it anyway.
		newMemory = MemAlloc(Size, Alignment, Alignment != 0);

		if (Memory)
			memcpy(newMemory, Memory, min(Size, MemSize(Memory)));
	}

	MemFree(Memory);
	return newMemory;
}

void *hk_realloc(void *Memory, size_t Size)
{
	return hk_aligned_realloc(Memory, Size, 0);
}

void *hk_recalloc(void *Memory, size_t Count, size_t Size)
{
	return hk_aligned_realloc(Memory, Count * Size, 0);
}

void hk_free(void *Block)
{
	MemFree(Block);
}

void hk_aligned_free(void *Block)
{
	MemFree(Block, true);
}

size_t hk_msize(void *Block)
{
	return MemSize(Block);
}

char *hk_strdup(const char *str1)
{
	size_t len = (strlen(str1) + 1) * sizeof(char);
	return (char *)memcpy(hk_malloc(len), str1, len);
}

void __declspec(naked) MC_FastFree()
{
	__asm
	{
		push esi
		call hk_free
		add esp, 0x4
		retn
	}
}

void PatchMemoryAllocators()
{
	Detours::X86::DetourFunction((PBYTE)0x00557C81, (PBYTE)&hk_malloc);			// malloc
	Detours::X86::DetourFunction((PBYTE)0x00558D04, (PBYTE)&hk_realloc);		// realloc
	Detours::X86::DetourFunction((PBYTE)0x00558F1F, (PBYTE)&hk_recalloc);		// _recalloc
	Detours::X86::DetourFunction((PBYTE)0x00558277, (PBYTE)&hk_calloc);			// calloc
	Detours::X86::DetourFunction((PBYTE)0x00561CC1, (PBYTE)&hk_calloc);			// __calloc_crt
	Detours::X86::DetourFunction((PBYTE)0x005563D9, (PBYTE)&hk_aligned_malloc);	// _aligned_malloc
	Detours::X86::DetourFunction((PBYTE)0x0055655A, (PBYTE)&hk_aligned_realloc);// _aligned_realloc
	Detours::X86::DetourFunction((PBYTE)0x00558BCA, (PBYTE)&hk_strdup);			// _strdup
	Detours::X86::DetourFunction((PBYTE)0x00557D44, (PBYTE)&hk_free);			// free
	Detours::X86::DetourFunction((PBYTE)0x005563C4, (PBYTE)&hk_aligned_free);	// _aligned_free
	Detours::X86::DetourFunction((PBYTE)0x00565742, (PBYTE)&hk_msize);			// _msize

	Detours::X86::DetourFunction((PBYTE)0x00B2DDE0, (PBYTE)&hk_malloc);			// operator new
	Detours::X86::DetourFunction((PBYTE)0x00B2DDA0, (PBYTE)&hk_free);			// operator delete
	Detours::X86::DetourFunction((PBYTE)0x00B2DD60, (PBYTE)&hk_free);			// operator delete[]

	PatchMemory(0x00B2F01F, (PBYTE)"\xEB", 1);									// Forcefully disable TempMemEnableFlag
	Detours::X86::DetourFunction((PBYTE)0x009F3BC0, (PBYTE)&MC_FastFree);		// MC_FastFree
	Detours::X86::DetourFunction((PBYTE)0x009FEA00, (PBYTE)&hk_malloc);			// MC_TempAlloc
	Detours::X86::DetourFunction((PBYTE)0x009FE8F0, (PBYTE)&hk_free);			// MC_TempFree
}
#else
void PatchMemoryAllocators()
{
}
#endif