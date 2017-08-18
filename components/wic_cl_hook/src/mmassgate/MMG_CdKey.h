#pragma once

class MMG_CdKey
{
public:
	static void InitializeHook();

	union KeyDefinition
	{
		struct
		{
			BYTE	gap0[8];	// this+0x0
			__int64 _bf8;		// this+0x8
		} section;

		char data[16];			// this+0x0
	};

	#pragma pack(push, 1)
	class Validator
	{
	public:
		KeyDefinition	myKey;				// this+0x0
		bool			myIsChecksumCorrect;// this+0x10

		void SetKey(const char *aKey);
		bool IsKeyValid();

		// Non-class function hook
		static void hk_SetKey();
	};
	#pragma pack(pop)
};

CHECK_SIZE(MMG_CdKey::Validator, 0x11);