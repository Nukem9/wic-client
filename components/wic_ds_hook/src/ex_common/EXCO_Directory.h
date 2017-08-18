#pragma once

class EXCO_Directory
{
public:
	static char ourModDirectory[];

	const Ice *myDirectoryFile;				// this+0x0
	const Ice *myGeneralPlayerFeedbackFile;	// this+0x4
	const Ice *myGenericUnitFeedbackFile;	// this+0x8
	const Ice *mySpawnerDefinitionFile;		// this+0xC
	const Ice *myTeamDefinitionFile;		// this+0x10
	const Ice *myGameModesFile;				// this+0x14
	const Ice *myGlobalScriptEventsFile;	// this+0x18
	const Ice *myDirectoryRoot;				// this+0x1C
	const Ice *myGeneralPlayerFeedback;		// this+0x20
	const Ice *myGenericUnitFeedback;		// this+0x24
	const Ice *mySpawnerDefinitions;		// this+0x28
	const Ice *myTeamDefinitions;			// this+0x2C
	const Ice *myTeamDefintions;			// this+0x30
	const Ice *myGameModes;					// this+0x34
	const Ice *myGlobalScriptEvents;		// this+0x38

	static void InitializeHook();

	const char *GetModDirectory();
};