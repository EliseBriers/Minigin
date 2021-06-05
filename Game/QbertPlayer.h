#pragma once
#include <IComponent.h>
#include "GridHopper.h"
#include "ObservableVariable.h"
#include "Enums.h"

namespace dae
{
	class JsonObjectWrapper;
	class TimerComponent;
}

class HopperSpriteComponent;

class QbertPlayer final : public dae::IComponent
{
	using state_observer_t = dae::ObservableVariable<PlayerState>::observer_t;
public:
	QbertPlayer( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );

	void Update( const dae::UpdateInfo& ) override;
	void Init( const dae::InitInfo& initInfo ) override;

	void OnTouchDown( GridHopper::TouchdownType touchdownType );
	void Respawn( );
	void OnDeath( );

	void RegisterStateObserver( state_observer_t stateObserver );
	void NextRotation( ) const;
	void SetPosition( const glm::vec2& newPos ) const;
	glm::vec2 GetPosition( ) const;

	int GetLastIndex( ) const;
	MoveDirection GetLastMoveDirection( ) const;
	
	// Rule of 5
	~QbertPlayer( ) override = default;
	QbertPlayer( const QbertPlayer& other ) = delete;
	QbertPlayer( QbertPlayer&& other ) noexcept = delete;
	QbertPlayer& operator=( const QbertPlayer& other ) = delete;
	QbertPlayer& operator=( QbertPlayer&& other ) noexcept = delete;
private:
	dae::ObservableVariable<PlayerState> m_State;
	GridHopper* m_pGridHopper;
	HopperSpriteComponent* m_pSprite;
	dae::TimerComponent* m_pRespawnTimer;
	dae::TimerComponent* m_pForgetTimer;
	bool m_InputLeft;
	bool m_InputRight;
	bool m_InputUp;
	bool m_InputDown;
	dae::TransformComponent* m_pTransform;
	QbertSceneBehavior* m_pSceneBehavior;

	bool m_IsFirstPlayer;

	// Last index and direction used so coily will jump off the edge
	int m_LastIndex;
	MoveDirection m_LastMoveDirection;

	int GetInputCount( ) const;

	// Inits
	void InitCollider( );
	void InitInputs( const dae::InitInfo& initInfo );
};
