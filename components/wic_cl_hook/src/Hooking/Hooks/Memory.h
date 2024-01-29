#pragma once

#include <initializer_list>

#define DeclareGlobalRef(Type, Name, Offset) Type& Name = *reinterpret_cast<Type *>(Offset)
#define DeclareGlobalPtr(Type, Name, Offset) Type *Name = reinterpret_cast<Type *>(Offset)

namespace Memory
{
	void Patch(std::uintptr_t Address, const std::uint8_t *Data, std::size_t Size);
	void Patch(std::uintptr_t Address, std::initializer_list<std::uint8_t> Data);
	void Fill(std::uintptr_t Address, std::uint8_t Value, std::size_t Size);
}