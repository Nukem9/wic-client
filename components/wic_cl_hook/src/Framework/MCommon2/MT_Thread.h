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

#include "MT_Mutex.h"

class MT_Thread
{
public:
	bool StopRequested() const
	{
		return myStopRequested;
	}

protected:
	// Your thread implementation should be in Run. model like this: void Run() { while (!StopRequested()) { ... } }
	virtual void Run() = 0;

	// You can not place a thread on the stack! Threads must be allocated on the heap.
	virtual ~MT_Thread() = default;

private:
	volatile bool myStopRequested = false;
	volatile uintptr_t myThreadHandle = static_cast<uintptr_t>(-1);
	u32 myThreadId = 0;
};
static_assert_size(MT_Thread, 0x10);