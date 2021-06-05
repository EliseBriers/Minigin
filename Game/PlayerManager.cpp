#include "GamePCH.h"
#include "PlayerManager.h"
#include "QbertPlayer.h"
#include <glm/detail/func_geometric.inl>

void PlayerManager::RegisterPlayer( QbertPlayer* pPlayer )
{
	m_pPlayers.emplace_back( pPlayer );
	dae::Logger::LogInfo( "PlayerManager::RegisterPlayer > Player registered" );
}

QbertPlayer* PlayerManager::GetClosestPlayer( const glm::vec2& pos )
{
	float distance{ FLT_MAX };
	QbertPlayer* pClosest{ nullptr };
	for( QbertPlayer* pPlayer : m_pPlayers )
	{
		const float newLength{ length( pPlayer->GetPosition( ) - pos ) };
		if( newLength < distance )
		{
			distance = newLength;
			pClosest = pPlayer;
		}
	}
	return pClosest;
}

void PlayerManager::PauseAll( )
{
	for( QbertPlayer* pPlayer : m_pPlayers )
	{
		pPlayer->Pause( );
	}
}
