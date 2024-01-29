#pragma once

#define IS_PC_BUILD 1

// Multiplayer (wic.exe)
#ifndef WIC_MP
#define WIC_MP 0
#endif

// Dedicated server (wic_ds.exe)
#ifndef WIC_DS
#define WIC_DS 0
#endif

// Boradcast tool (wic_bt.exe)
#ifndef WIC_ESPORTS_PRODUCER_TOOL
#define WIC_ESPORTS_PRODUCER_TOOL 0
#endif

// Various game editors (wiced.exe)
#ifndef WIC_EDITOR
#define WIC_EDITOR 0
#endif

#define WIC_APPLICATION_NAME            "World in Conflict"
#define WIC_APPLICATION_NAME_UNICODE    L"World in Conflict"
#define WIC_DOCUMENTS_PATH_NAME         "World in Conflict"

#define WIC_NO_MASSGATE 0

#if MC_LEAN_AND_MEAN
#define MC_NO_ASSERTS 1
#define MC_NO_FATAL_ASSERTS 1
#define MC_NO_BOOM 1
#define MC_NO_PDB_APPEND 1
#define MC_NO_DEBUG_SPAM 1
#define MC_NO_DEV_CONSOLE_COMMANDS 1
#else
#define MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK 1		// Check for out of bounds indexing in MC_GrowingArray
#define MC_HEAVY_DEBUG_MC_STRING_BOUNDSCHECK 1			// Check for out of bounds indexing in MC_String
#define MC_HEAVY_DEBUG_MEMORY_SYSTEM_NAN_INIT 1			// Init memory to 0xFFFFFFFF, which triggers a float exception if we do float calcs with uninited data
#define MC_HEAVY_DEBUG_MC_VECTOR_NAN_INIT 1				// Init MC_Vector to 0xFFFFFFFF, which triggers a float exception if we do float calcs with uninited data
#define MC_HEAVY_DEBUG_TEMP_MEMORY_DEBUGGING 1			// Enable debug checks in temp mem system
#define MC_ENABLE_FLOAT_EXCEPTIONS 1					// Enable floating point exceptions
#define MC_ALLOCATION_ADD_EXTRA_DATA 1					// Store allocation extra data for memory tracking, leak finding and overwrite checks.
#endif

#include "MC_Base.h"