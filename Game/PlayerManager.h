#pragma once

class QbertPlayer;

class PlayerManager
{
public:
	void RegisterPlayer( QbertPlayer* pPlayer );
	QbertPlayer* GetClosestPlayer( const glm::vec2& pos );
	void PauseAll( );
private:
	// ToDo: pause other player when other player dies
	std::vector<QbertPlayer*> m_pPlayers;
};
