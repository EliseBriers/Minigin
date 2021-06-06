#include "GamePCH.h"
#include "QbertSceneBehavior.h"
#include <Scene.h>
#include <UpdateInfo.h>

QbertSceneBehavior::QbertSceneBehavior( const std::string& nextLevel, size_t& globalScore )
	: m_NextLevel{ nextLevel }
	, m_GlobalScore{ globalScore }
	, m_LevelStartSound{ 0u }
	, m_HasPlayedStartSound{ false }
{
	// m_Score.AddObserver( []( size_t score )
	// {
	// 	dae::Logger::LogInfo( "Score: " + std::to_string( score ) );
	// } );
}

void QbertSceneBehavior::Update( const dae::UpdateInfo& updateInfo )
{
	if( m_PlayLevelEnd )
	{
		m_PlayLevelEnd = false;
		updateInfo.PushSound( m_EndLevelSound, 1.f );
	}

	const float dt{ updateInfo.GetDeltaTime( ) };
	m_TriggerManager.Update( );
	m_EnemyManager.Update( dt );
	if( !m_HasPlayedStartSound )
	{
		m_HasPlayedStartSound = true;
		updateInfo.PushSound( m_LevelStartSound, 1.f );
	}
}

void QbertSceneBehavior::Init( dae::InitInfo& initInfo )
{
	m_Score.Set( m_GlobalScore );

	m_LevelStartSound = initInfo.GetSound( "LevelIntro.wav" );
	m_EndLevelSound = initInfo.GetSound( "LevelCompleted.wav" );
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
	m_GlobalScore = m_Score.Get( );
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
	m_PlayLevelEnd = true;
	// dae::Logger::LogInfo( "Awarding points for " + std::to_string( m_DiskCount ) + " disks" );
	m_Score.Set( m_Score.Get( ) + m_DiskCount * 50 );
}

void QbertSceneBehavior::RegisterPlayer( QbertPlayer* pGameObject )
{
	m_PlayerManager.RegisterPlayer( pGameObject );
}

void QbertSceneBehavior::OnPlayerDeath( )
{
	m_EnemyManager.PauseAll( );

	const size_t penalty{ 600 };
	if( penalty > m_Score.Get( ) )
		m_Score.Set( 0u );
	else
		m_Score.Set( m_Score.Get( ) - penalty );
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

void QbertSceneBehavior::AddScoreObserver( score_observer_t observer )
{
	m_Score.AddObserver( std::move( observer ) );
}

size_t QbertSceneBehavior::GetScore( ) const
{
	return m_Score.Get( );
}

void QbertSceneBehavior::AddDisk( )
{
	++m_DiskCount;
}

void QbertSceneBehavior::RemoveDisk( )
{
	if( m_DiskCount == 0 )
	{
		dae::Logger::LogWarning( "QbertSceneBehavior::RemoveDisk > Removing disk while disk count is 0" );
		return;
	}
	--m_DiskCount;
}
