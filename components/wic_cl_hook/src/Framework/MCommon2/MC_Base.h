#pragma once

#if IS_PC_BUILD
#define MC_FORCEINLINE __forceinline
#define MC_THREADLOCAL thread_local
#define MC_RESTRICT __restrict
#else
#error Need to define MC_FORCEINLINE, MC_THREADLOCAL and MC_RESTRICT for this platform
#endif

#define static_assert_size(type, size) static_assert(sizeof(type) == (size), "Type size doesn't match.")
#define static_assert_offset(type, member, offset) static_assert(offsetof(type, member) == (offset), "Member offset doesn't match.")

#if WIC_MP
#define SelectOffset(Game, Server) (Game)
#elif WIC_DS
#define SelectOffset(Game, Server) (Server)
#endif

#include "MC_DataTypes.h"
#include "MC_Debug.h"