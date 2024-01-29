#include <Hooks/Hooks.h>
#include <Hooks/Memory.h>
#include "EXP_Physics.h"

void hk_sub_911CD0();
void hk_00912278();

//
// Multiple interesting things are happening here in EXP_Physics:
//
// - The particle system uses ray casts to detect collisions with the physics world. Both systems are
// parallelizable and can be run on numerous threads. Locks are required.
// 
// - The game's EXP_Physics lock causes significant performance degradation because of contention, especially
// with many CPU cores.
//
// - EXP_Physics::ParallelizableUpdate() appears to be the only writer. ParticleRayCast() is a reader and
// the main culprit of said performance issues.
// 
// ...and here's where it gets a bit weird:
//
// - ParticleRayCast() has to call EXP_Physics::Lock() in order to call MP_Engine::RayCast() safely. However,
// MP_Engine::RayCast() acquires its own separate world lock. This means that ParticleRayCast() is acquiring
// two locks.
//
// ...so why are there two locks? EXP_Physics has a slightly larger scope than MP_Engine. But here's the funny
// part: ParticleRayCast() has no use for EXP_Physics::Lock() except to access a single variable, myPhysEngine.
//
// If myPhysEngine can be safely read without a lock, then contention, and thus performance, can be improved. I
// don't have a good way to test this and rule out bugs. I can at least try to skip the EXP_Physics lock and
// see what happens. There's no effect on gameplay aside from potential crashes or performance.
//
//
// NOTE: Massive tried to implement their own crappy hack. It's a not-so-clever attempt to avoid locking by using
// an "is in use" counter variable. I'd love to read their code comments on this one.
//
// NOTE: myPhysEngine is modified during world initialization and teardown, such as map loads. A lock isn't
// necessary there.
//
DECLARE_HOOK_INITIALIZER_MP(EXP_Physics,
{
	Hooks::WriteJump(0x00911CD0, &hk_sub_911CD0);// EXP_Physics::ParallelizableUpdate() acquire
	Hooks::WriteJump(0x00912278, &hk_00912278);// EXP_Physics::ParallelizableUpdate() release

	Memory::Patch(0x00B11763, { 0xEB });// Skip acquire in ParticleRayCast()
	Memory::Patch(0x00B101E0, { 0xC3 });// Skip release in ParticleRayCastUnlock()
});

void __declspec(naked) hk_sub_911CD0()
{
	EXP_Physics *physics;

	__asm
	{
		push ebp
		mov ebp, esp
		sub esp, __LOCAL_SIZE
		mov physics, esi
	}

	physics->myParallelizableUpdateMutex.lock();

	__asm
	{
		leave
		retn
	}
}

void __declspec(naked) hk_00912278()
{
	EXP_Physics *physics;

	__asm
	{
		push ebp
		mov ebp, esp
		sub esp, __LOCAL_SIZE
		mov physics, edi
	}

	physics->myParallelizableUpdateMutex.unlock();

	__asm
	{
		leave
		push 0x000912281
		retn
	}
}