#pragma once

enum CipherIdentifier
{
	CIPHER_UNKNOWN = 0x0,
	CIPHER_BLOCKTEA = 0x1,
	CIPHER_NULLCIPHER = 0x2,
	CIPHER_ILLEGAL_CIPHER = 0x3,
	NUM_CIPHERS = 0x4,
};

class MMG_ICipher
{
public:
	virtual MMG_ICipher *operator = (MMG_ICipher *);

	virtual void SetKey		(const char *);
	virtual void SetRawKey	(MMG_CryptoHash *);

	virtual MMG_CryptoHash *GetHashOfKey(MMG_CryptoHash *);

	virtual void Encrypt(char *, uint32_t);
	virtual void Decrypt(char *, uint32_t);

	virtual CipherIdentifier GetIdentifier();
};

CHECK_SIZE(MMG_ICipher, 0x4);