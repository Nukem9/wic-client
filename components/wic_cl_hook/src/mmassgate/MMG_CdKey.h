#pragma once

class MMG_CdKey
{
public:
	union KeyDefinition
	{
		struct
		{
			unsigned __int64 productId : 3;				// 0:2
			unsigned __int64 batchId : 7;				// 3:9
			unsigned __int64 sequenceNumber : 25;		// 10:34
			unsigned __int64 checksum : 10;				// 35:44
			unsigned __int64 randomDataPart1 : 19;		// 45:63
			unsigned __int64 randomDataPart2 : 32;		// 64:95
			unsigned __int64 randomDataPart3 : 4;		// 96:99
			unsigned __int64 zero : 28;					// 100:127
		} section;

		char data[16];
	};

	#pragma pack(push, 1)
	class Validator
	{
	public:
		enum KeyType
		{
			INVALID_TYPE = 0x0,
			PERMANENT_TEA_ENCRYPTION = 0x1,
			PERMANENT_AES_ENCRYPTION = 0x2,
			PERMANENT_BLOWFISH_ENCRYPTION = 0x3,
			SUBSCRIPTION_TEA_ENCRYPTION = 0x4,
			SUBSCRIPTION_AES_ENCRYPTION = 0x5,
			SUBSCRIPTION_BLOWFISH_ENCRYPTION = 0x6,
			CAFE_TEA_ENCRYPTION = 0x7,
			CAFE_AES_ENCRYPTION = 0x8,
			CAFE_BLOWFISH_ENCRYPTION = 0x9,
			GUEST_KEY_TEA_ENCRYPTION = 0xA,
		};

		KeyDefinition	m_Key;				// this+0x0
		bool			m_IsChecksumCorrect;// this+0x10

	private:
		static const char *LocCdKeyAlphabet;

	public:
		void SetKey(const char *aKey);
		uint32_t GetSequenceNumber();

		// Non-class function hook
		static void hk_SetKey();

	private:
	};
	#pragma pack(pop)

	static void InitializeHook();
};

//CHECK_OFFSET(MMG_CdKey::Validator, m_Key, 0x0);
//CHECK_OFFSET(MMG_CdKey::Validator, m_IsChecksumCorrect, 0x10);
CHECK_SIZE(MMG_CdKey::Validator, 0x11);