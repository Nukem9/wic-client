// Massgate
// Copyright (C) 2017 Ubisoft Entertainment
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// THIS FILE HAS BEEN MODIFIED FOR USE IN A NON-UBISOFT PROJECT. IT IS NOT
// AN ORIGINAL COPY.
//
#include <Windows.h>
#include "MT_Mutex.h"

#define GET_CRITICAL_SECTION() (std::launder(reinterpret_cast<CRITICAL_SECTION *>(&myMutex)))

MT_Mutex::MT_Mutex(s32 spinLockCount)
{
	static_assert(sizeof(CRITICAL_SECTION) == MUTEX_CS_SIZE);
	static_assert(alignof(CRITICAL_SECTION) == MUTEX_CS_ALIGN);

	if (spinLockCount > 0)
		InitializeCriticalSectionAndSpinCount(GET_CRITICAL_SECTION(), spinLockCount);
	else
		InitializeCriticalSection(GET_CRITICAL_SECTION());
}

MT_Mutex::~MT_Mutex()
{
	DeleteCriticalSection(GET_CRITICAL_SECTION());
}

void
MT_Mutex::lock()
{
	EnterCriticalSection(GET_CRITICAL_SECTION());
	myNumLocks++;
}

bool
MT_Mutex::try_lock()
{
	if (TryEnterCriticalSection(GET_CRITICAL_SECTION()))
	{
		myNumLocks++;
		return true;
	}

	return false;
}

void
MT_Mutex::unlock()
{
	MC_ASSERT(myNumLocks > 0);

	myNumLocks--;
	LeaveCriticalSection(GET_CRITICAL_SECTION());
}

u32
MT_Mutex::GetLockCount()
{
	return myNumLocks;
}