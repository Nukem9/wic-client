#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>
#include <winsock.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")

// Shared libraries
#pragma comment(lib, "detours.lib")
#include "../../shared/detours/detours/Detours.h"
#include "../../shared/shared_utility.h"

#define CHECK_OFFSET(str, member, offset)	static_assert(offsetof(str, member) == offset, #str "." #member ": Invalid struct/class member offset");
#define CHECK_SIZE(str, size)				static_assert(sizeof(str) == size, #str ": Invalid struct/class size");

// MCommon2
#include "mcommon2/stdafx.h"

// MNetwork
#include "mnetwork/stdafx.h"

// MMassgate
#include "mmassgate/stdafx.h"

// EX_Common
#include "ex_common/stdafx.h"