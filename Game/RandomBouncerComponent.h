#pragma once
#include <IComponent.h>
#include "Enums.h"
#include "TimerComponent.h"
#include "ObservableVariable.h"

class QbertPlayer;
class HopperSpriteComponent;
class GridHopper;
class SphereOverlapDetector;

class RandomBouncerComponent final : public dae::IComponent
{
public:
	RandomBouncerComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );

	void Init( const dae::InitInfo& ) override;
	void Activate( ) override;
	void Update( const dae::UpdateInfo& updateInfo ) override;

	~RandomBouncerComponent( ) override = default;
	RandomBouncerComponent( const RandomBouncerComponent& other ) = delete;
	RandomBouncerComponent( RandomBouncerComponent&& other ) noexcept = delete;
	RandomBouncerComponent& operator=( const RandomBouncerComponent& other ) = delete;
	RandomBouncerComponent& operator=( RandomBouncerComponent&& other ) noexcept = delete;
private:
	RandomBounceDirection m_RandomBounceDirection;
	dae::ObservableVariable<RandomBouncerState> m_State;
	glm::vec2 m_DownDirection;

	HopperSpriteComponent* m_pSprite;
	GridHopper* m_pGridHopper;
	SphereOverlapDetector* m_pCollider;
	dae::TimerComponent* m_pNextActionTimer;
	dae::TimerComponent* m_pFallTimer;
	dae::TransformComponent* m_pTransform;
	const float m_FallSpeed;
	const float m_SpawnDistance;
	glm::vec2 m_SpawnPos;
	bool m_IsLethal;

	void GetComponentPointers( );

	void UpdateFall( const dae::UpdateInfo& updateInfo ) const;
	void UpdateSpawn( const dae::UpdateInfo& updateInfo );

	void OnDeath( );
	void KillPlayer( QbertPlayer* pPlayer );
};
