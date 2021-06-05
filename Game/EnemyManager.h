#pragma once

class EnemyManager final
{
	struct RespawnEntry
	{
		RespawnEntry( dae::GameObject* pObject, float time );
		dae::GameObject* pGameObject{ nullptr };
		float Elapsed{ 0.f };

		// Rule of 5
		~RespawnEntry( );
		RespawnEntry( const RespawnEntry& other ) = delete;
		RespawnEntry( RespawnEntry&& other ) noexcept = default;
		RespawnEntry& operator=( const RespawnEntry& other ) = delete;
		RespawnEntry& operator=( RespawnEntry&& other ) noexcept = default;
	};

public:
	EnemyManager( ) = default;
	void DeactivateAll( );
	void ActivateAll( );
	void PauseAll( );
	void UnPauseAll( );
	void ClearRespawns( );
	void Update( float dt );
	void RegisterKilledEnemy( dae::GameObject* pGameObject, float respawnTime );
	void RegisterEnemy( dae::GameObject* pGameObject );
private:
	std::vector<dae::GameObject*> m_pEnemies;
	std::vector<RespawnEntry> m_RespawnEntries;
};
