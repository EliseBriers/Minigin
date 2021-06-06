#pragma once
#include <SceneBehavior.h>
#include "TriggerManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "ObservableVariable.h"
#include "CoilyComponent.h"

class QbertSceneBehavior final : public dae::SceneBehavior
{
public:
	using score_observer_t = dae::ObservableVariable<size_t>::observer_t;

	QbertSceneBehavior( const std::string& nextLevel, size_t& globalScore );

	void Update( const dae::UpdateInfo& ) override;
	void Init( dae::InitInfo& ) override;

	void RegisterOverlapDetector( SphereOverlapDetector& overlapDetector );
	void UnRegisterOverlapDetector( const SphereOverlapDetector& sphereOverlapDetector );
	void EndLevel( ) const;
	void QueueUnRegisterOverlapDetector( SphereOverlapDetector& sphereOverlapDetector );
	void RegisterKilledEnemy( dae::GameObject* pGameObject, bool killedByPlayer, float respawnTime );
	void RegisterEnemy( dae::GameObject* pGameObject );
	void GameCompleted( );
	void RegisterPlayer( QbertPlayer* pGameObject );
	void OnPlayerDeath( );
	void OnPlayerRespawn( );
	void AddPoints( size_t amount );
	QbertPlayer* GetClosestPlayer( const glm::vec2& pos );
	void AddScoreObserver( score_observer_t observer );
	size_t GetScore( ) const;
	void AddDisk( );
	void RemoveDisk( );
	
	// Rule of 5
	~QbertSceneBehavior( ) override = default;
	QbertSceneBehavior( const QbertSceneBehavior& other ) = delete;
	QbertSceneBehavior( QbertSceneBehavior&& other ) noexcept = delete;
	QbertSceneBehavior& operator=( const QbertSceneBehavior& other ) = delete;
	QbertSceneBehavior& operator=( QbertSceneBehavior&& other ) noexcept = delete;
private:
	size_t m_DiskCount;
	std::string m_NextLevel;
	TriggerManager m_TriggerManager;
	EnemyManager m_EnemyManager;
	PlayerManager m_PlayerManager;

	size_t& m_GlobalScore;

	size_t m_LevelStartSound;
	bool m_HasPlayedStartSound;

	size_t m_EndLevelSound;
	bool m_PlayLevelEnd;

	dae::ObservableVariable<size_t> m_Score;
};
