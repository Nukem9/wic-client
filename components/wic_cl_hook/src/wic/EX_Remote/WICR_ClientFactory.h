#pragma once

class EXG_Game;

class WICR_ClientFactory
{
public:
	bool ListenForClients();
	bool Send(EXG_Game *aGame);
	bool Receive();
};