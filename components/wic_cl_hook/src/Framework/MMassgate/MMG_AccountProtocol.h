#pragma once

class MMG_ICipher;

class MMG_AccountProtocol
{
public:
#pragma pack(push, 1)
	class Query
	{
	public:
		char _pad[0x4EA];
		MMG_ICipher *myEncrypter;
	};
	static_assert_offset(Query, myEncrypter, 0x4EA);
#pragma pack(pop)
};
// static_assert_size(MMG_AccountProtocol, 0x);