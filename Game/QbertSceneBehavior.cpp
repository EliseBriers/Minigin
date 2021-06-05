#include "GamePCH.h"
#include "QbertSceneBehavior.h"
#include <Scene.h>
#include <UpdateInfo.h>

QbertSceneBehavior::QbertSceneBehavior( const std::string& nextLevel, bool isLastLevel )
	: m_NextLevel{ nextLevel }
	, m_IsLastLevel{ isLastLevel }
{
	// m_Score.AddObserver( []( size_t score )
	// {
	// 	dae::Logger::LogInfo( "Score: " + std::to_string( score ) );
	// } );
}

void QbertSceneBehavior::Update( const dae::UpdateInfo& updateInfo )
{
	const float dt{ updateInfo.GetDeltaTime( ) };
	m_TriggerManager.Update( );
	m_EnemyManager.Update( dt );
}

void QbertSceneBehavior::RegisterOverlapDetector( SphereOverlapDetector& overlapDetector )
{
	m_TriggerManager.AddOverlapDetector( overlapDetector );
}

void QbertSceneBehavior::UnRegisterOverlapDetector( const SphereOverlapDetector& sphereOverlapDetector )
{
	m_TriggerManager.RemoveOverlapDetector( sphereOverlapDetector );
}

void QbertSceneBehavior::EndLevel( ) const
{
	if( !m_IsLastLevel )
		GetScene( ).GetSceneManager( )->SetActiveScene( m_NextLevel );
}

void QbertSceneBehavior::QueueUnRegisterOverlapDetector( SphereOverlapDetector& sphereOverlapDetector )
{
	m_TriggerManager.QueueUnRegisterOverlapDetector( sphereOverlapDetector );
}

void QbertSceneBehavior::RegisterKilledEnemy( dae::GameObject* pGameObject, bool killedByPlayer, float respawnTime )
{
	m_EnemyManager.RegisterKilledEnemy( pGameObject, respawnTime );
	if( killedByPlayer )
		dae::Logger::LogInfo( "Enemy killed by player" );
	else
		dae::Logger::LogInfo( "Enemy killed environment" );
}

void QbertSceneBehavior::RegisterEnemy( dae::GameObject* pGameObject )
{
	m_EnemyManager.RegisterEnemy( pGameObject );
}

void QbertSceneBehavior::GameCompleted( )
{
	m_EnemyManager.PauseAll( );
	m_PlayerManager.PauseAll( );
}

void QbertSceneBehavior::RegisterPlayer( QbertPlayer* pGameObject )
{
	m_PlayerManager.RegisterPlayer( pGameObject );
}

void QbertSceneBehavior::OnPlayerDeath( )
{
	m_EnemyManager.PauseAll( );
}

void QbertSceneBehavior::OnPlayerRespawn( )
{
	m_EnemyManager.ClearRespawns( );
	m_EnemyManager.ActivateAll( );
	m_EnemyManager.UnPauseAll( );
}

void QbertSceneBehavior::AddPoints( size_t amount )
{
	m_Score.Set( m_Score.Get( ) + amount );
}

QbertPlayer* QbertSceneBehavior::GetClosestPlayer( const glm::vec2& pos )
{
	return m_PlayerManager.GetClosestPlayer( pos );
}
