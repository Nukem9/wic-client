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
#pragma once 

class MT_Mutex
{
private:
	// HACK: We don't want to include windows.h, so we'll just define the size and alignment of
	// CRITICAL_SECTION here and use myMutex as a buffer.
	constexpr static size_t MUTEX_CS_SIZE = 0x18;
	constexpr static size_t MUTEX_CS_ALIGN = 0x4;

	alignas(MUTEX_CS_ALIGN) u8 myMutex[MUTEX_CS_SIZE];
	u32 myNumLocks = 0;

public:
	MT_Mutex(s32 spinLockCount = 0);
	virtual ~MT_Mutex();

	void lock();		// Lock the mutex. Will halt current thread until another thread has unlocked
	bool try_lock();	// If mutex is already locked by any other thread than me; fail instead of wait
	void unlock();		// Unlock a previously locked mutex
	u32 GetLockCount();	// Returns the number of times this mutex has been recursively locked
};
static_assert_size(MT_Mutex, 0x20);