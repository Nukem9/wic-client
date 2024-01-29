#pragma once

void *MC_Allocate(size_t aSize, const char *aFileName, int aLine, void *aThreadHandle, const void* aContext, bool anArrayFlag);
void MC_Free(void* aPointer, bool anArrayFlag);

#define MC_HEAPIMPLEMENTATION_REDEFINE_NEW \
 \
static void *operator new(std::size_t count) \
{ \
	return MC_Allocate(count, nullptr, 0, nullptr, nullptr, false); \
} \
 \
static void *operator new[](std::size_t count) \
{ \
	return MC_Allocate(count, nullptr, 0, nullptr, nullptr, true); \
} \
 \
static void *operator new(std::size_t count, std::align_val_t al) \
{ \
	MC_ASSERT(false); \
	return nullptr; \
} \
 \
static void *operator new[](std::size_t count, std::align_val_t al) \
{ \
	MC_ASSERT(false); \
	return nullptr; \
} \
 \
static void *operator new(std::size_t count, const std::nothrow_t&) noexcept \
{ \
	return MC_Allocate(count, nullptr, 0, nullptr, nullptr, false); \
} \
 \
static void *operator new[](std::size_t count, const std::nothrow_t&) noexcept \
{ \
	return MC_Allocate(count, nullptr, 0, nullptr, nullptr, true); \
} \
 \
static void *operator new(std::size_t count, std::align_val_t al, const std::nothrow_t&) noexcept \
{ \
	MC_ASSERT(false); \
	return nullptr; \
} \
 \
static void *operator new[](std::size_t count, std::align_val_t al, const std::nothrow_t&) noexcept \
{ \
	MC_ASSERT(false); \
	return nullptr; \
} \
 \
static void operator delete(void *ptr) noexcept \
{ \
	MC_Free(ptr, false); \
} \
 \
static void operator delete[](void *ptr) noexcept \
{ \
	MC_Free(ptr, true); \
} \
 \
static void operator delete(void *ptr, std::align_val_t al) noexcept \
{ \
	MC_ASSERT(false); \
} \
 \
static void operator delete[](void *ptr, std::align_val_t al) noexcept \
{ \
	MC_ASSERT(false); \
} \
 \
static void operator delete(void *ptr, std::size_t sz) noexcept \
{ \
	MC_Free(ptr, false); \
} \
 \
static void operator delete[](void *ptr, std::size_t sz) noexcept \
{ \
	MC_Free(ptr, true); \
} \
 \
static void operator delete(void *ptr, std::size_t sz, std::align_val_t al) noexcept \
{ \
	MC_ASSERT(false); \
} \
 \
static void operator delete[](void *ptr, std::size_t sz, std::align_val_t al) noexcept \
{ \
	MC_ASSERT(false); \
}