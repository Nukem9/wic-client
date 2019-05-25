#pragma once

class EX_CAI_AI
{
private:
	char _pad[0xFC];
	class EX_CAI_CommanderAI *myCommanderAI;
	char _pad1[0x44];
	int myApAmount;
	char _pad2[0x24];
	int myRoleId;

public:
	void hk_SetAP(const int anAmount);
	void hk_SetMaxAP(const int anAmount);
	void hk_SetCurrentMax(const int anAmount);
};