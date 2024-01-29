#pragma once

void* MC_TempAlloc(u32 aSize);
void* MC_TempAllocIfOwnerOnStack(void* anOwner, u32 aSize, const char* aFile, int aLine);
void MC_TempFree(void* aPointer);
bool MC_IsProbablyOnStack(const void* aPointer);
bool __cdecl MC_Assert(const char* aFile, int aLine, const char* aString, bool* anIgnoreFlag);