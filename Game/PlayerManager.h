#pragma once

class PlayerManager
{
public:
	void RegisterPlayer( dae::GameObject* pPlayer );
private:
	// ToDo: pause other player when other player dies
	std::vector<dae::GameObject*> m_pPlayers;
};
