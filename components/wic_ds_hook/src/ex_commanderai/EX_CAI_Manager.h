#pragma once

class EX_CAI_Manager
{
public:
	virtual void *__vecDelDtor(unsigned int);

	virtual void Act(const int, unsigned int);
	virtual void Update();
	virtual bool Try(const int);

	virtual float GetFitness(EX_CAI_Troop_Request *, float);
	virtual float GetValue();
	virtual float GetValue2();

	float					myValueToUse;	// this+0x4
	EX_CAI_CommanderAI_Data *myDataSet;		// this+0x8
};

CHECK_OFFSET(EX_CAI_Manager, myValueToUse, 0x4);
CHECK_OFFSET(EX_CAI_Manager, myDataSet, 0x8);