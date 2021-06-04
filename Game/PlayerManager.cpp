#include "GamePCH.h"
#include "PlayerManager.h"

void PlayerManager::RegisterPlayer( dae::GameObject* pPlayer )
{
	m_pPlayers.emplace_back( pPlayer );
	dae::Logger::LogInfo( "PlayerManager::RegisterPlayer > Player registered" );
}
