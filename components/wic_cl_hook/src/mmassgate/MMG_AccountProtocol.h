#pragma once

class MMG_AccountProtocol
{
public:
	static void InitializeHook();

private:
	static void WriteEncryptionKeys(void *accountQuery, MN_WriteMessage *aMessage);
	static void Query__ToStream_00BD1C73();
};