#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>

// Shared libraries
#pragma comment(lib, "detours.lib")
#include "../../shared/detours/detours/Detours.h"
#include "../../shared/shared_utility.h"