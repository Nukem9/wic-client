#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <stdint.h>
#include <vector>
#include <shellapi.h>
#include <winsock.h>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "ws2_32.lib")

// Shared libraries
#pragma comment(lib, "detours.lib")
#include "../../shared/detours/detours/Detours.h"
#include "../../shared/shared_utility.h"

#include "util.h"