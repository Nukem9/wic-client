#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <vector>
#include <shellapi.h>

#pragma comment(lib, "version.lib")

// Shared libraries
#pragma comment(lib, "detours.lib")
#include "../../shared/detours/Detours.h"
#include "../../shared/shared_utility.h"

#include "util.h"