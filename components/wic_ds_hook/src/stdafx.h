#pragma once

#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <intrin.h>

#pragma comment(lib, "ws2_32.lib")

// Shared libraries
#pragma comment(lib, "detours.lib")
#include "../../shared/detours/Detours.h"
#include "../../shared/shared_utility.h"
#include "../../shared/Ptr.h"

#include "types.h"

// MCommon2
#include "mcommon2/stdafx.h"

// Ice
#include "ice/stdafx.h"

// EX_Common
#include "ex_common/stdafx.h"

// EX_CommanderAI
#include "ex_commanderai/stdafx.h"

// MMassgate
#include "mmassgate/stdafx.h"

// EX_Remote
#include "ex_remote/stdafx.h"