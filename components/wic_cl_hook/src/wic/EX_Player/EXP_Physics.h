#pragma once

#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_String.h>
#include <MCommon2/MC_Vector.h>
#include <MCommon2/MT_Mutex.h>

class EXP_PhysGetYObject;
class MP_AabbPhantom;
class MP_Engine;
class MP_RigidBody;
class WICP_PropContainer;

class EXP_Physics
{
public:
	struct Explosion
	{
		MC_Vector3f myPosition;
		float myRadius;
		float myForce;
		float myDelay;
		float myLife;
	};
	static_assert_size(Explosion, 0x1C);

	MC_String myFrameOut;
	MT_Mutex myParallelizableUpdateMutex;
    MC_GrowingArray<Explosion> myExplosions;
    MC_GrowingArray<EXP_PhysGetYObject *> myGetYObjects;
    MC_GrowingArray<MP_RigidBody *> myActiveEntities;
    WICP_PropContainer *myPropContainer;
    MP_Engine *myPhysEngine;
    MP_RigidBody *myHeightfield;
    MP_AabbPhantom *myWaterPhantom;
    const char *myWaterSplashFile;
    s32 myThreadMemoryId;
    s32 myLastProcessedProp;
    u32 myEngineMaxNumSteps;
    float myEngineStepSize;
    float myEngineMaxStep;
    float myEngineStepScale;
    float myEngineUpdateTime;
};
static_assert_size(EXP_Physics, 0x84);