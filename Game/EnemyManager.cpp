#include "GamePCH.h"
#include "EnemyManager.h"

EnemyManager::RespawnEntry::RespawnEntry( dae::GameObject* pObject, float time )
	: pGameObject{ pObject }
	, Elapsed{ time }
{
}

EnemyManager::RespawnEntry::~RespawnEntry( )
{
	pGameObject->Activate( );
}

void EnemyManager::DeactivateAll( )
{
	for( dae::GameObject* pGameObject : m_pEnemies )
		pGameObject->Deactivate( );
}

void EnemyManager::ActivateAll( )
{
	for( dae::GameObject* pGameObject : m_pEnemies )
		pGameObject->Activate( );
}

void EnemyManager::PauseAll( )
{
	for( dae::GameObject* pGameObject : m_pEnemies )
		pGameObject->Pause( );
}

void EnemyManager::UnPauseAll( )
{
	for( dae::GameObject* pGameObject : m_pEnemies )
		pGameObject->UnPause( );
}

void EnemyManager::ClearRespawns( )
{
	m_RespawnEntries.clear( );
}

void EnemyManager::Update( float dt )
{
	const auto it{
		std::remove_if( m_RespawnEntries.begin( ), m_RespawnEntries.end( ), [dt]( RespawnEntry& entry )
		{
			return ( entry.Elapsed -= dt ) < 0.f;
		} )
	};
	m_RespawnEntries.erase( it, m_RespawnEntries.end( ) );
}

void EnemyManager::RegisterKilledEnemy( dae::GameObject* pGameObject, float respawnTime )
{
	m_RespawnEntries.emplace_back( pGameObject, respawnTime );
}

void EnemyManager::RegisterEnemy( dae::GameObject* pGameObject )
{
	m_pEnemies.push_back( pGameObject );
}
