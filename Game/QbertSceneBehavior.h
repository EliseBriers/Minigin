#pragma once
#include <SceneBehavior.h>
#include "TriggerManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "ObservableVariable.h"

class QbertSceneBehavior final : public dae::SceneBehavior
{
public:

	QbertSceneBehavior( const std::string& nextLevel, bool isLastLevel );

	virtual void Update( const dae::UpdateInfo& ) override;

	
	void RegisterOverlapDetector( SphereOverlapDetector& overlapDetector );
	void UnRegisterOverlapDetector( const SphereOverlapDetector& sphereOverlapDetector );
	void EndLevel( ) const;
	void QueueUnRegisterOverlapDetector( SphereOverlapDetector& sphereOverlapDetector );
	void RegisterKilledEnemy( dae::GameObject* pGameObject, bool killedByPlayer, float respawnTime );
	void RegisterEnemy( dae::GameObject* pGameObject );
	void GameCompleted( );
	void RegisterPlayer( dae::GameObject* pGameObject );
	void OnPlayerDeath( );
	void OnPlayerRespawn( );
	void AddPoints( size_t amount );
	
	// Rule of 5
	~QbertSceneBehavior( ) override = default;
	QbertSceneBehavior( const QbertSceneBehavior& other ) = delete;
	QbertSceneBehavior( QbertSceneBehavior&& other ) noexcept = delete;
	QbertSceneBehavior& operator=( const QbertSceneBehavior& other ) = delete;
	QbertSceneBehavior& operator=( QbertSceneBehavior&& other ) noexcept = delete;
private:
	std::string m_NextLevel;
	TriggerManager m_TriggerManager;
	EnemyManager m_EnemyManager;
	PlayerManager m_PlayerManager;
	bool m_IsLastLevel;

	dae::ObservableVariable<size_t> m_Score;
};
