#pragma once
#include <BaseComponent.h>
#include "Enums.h"
#include <ObservableVariable.h>
#include "IDirectionController.h"
#include "PlayerDirectionController.h"

namespace dae
{
	class TimerComponent;
}

class QbertPlayer;
class SphereOverlapDetector;
class QbertSceneBehavior;
class HopperSpriteComponent;
class GridHopper;
class CoilyDirectionController;

class CoilyComponent final : public dae::BaseComponent
{
	using state_observer_t = dae::ObservableVariable<CoilyState>::observer_t;
public:
	CoilyComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );

	virtual void Update( const dae::UpdateInfo& ) override;
	virtual void PersistentUpdate( const dae::UpdateInfo& ) override;
	virtual void Init( const dae::InitInfo& initInfo ) override;
	virtual void Activate( ) override;
	virtual void Pause( ) override;
	void AddStateObserver( state_observer_t stateObserver );
	void NextAction( );

	// Rule of 5
	~CoilyComponent( ) override = default;
	CoilyComponent( const CoilyComponent& other ) = delete;
	CoilyComponent( CoilyComponent&& other ) noexcept = delete;
	CoilyComponent& operator=( const CoilyComponent& other ) = delete;
	CoilyComponent& operator=( CoilyComponent&& other ) noexcept = delete;
private:
	// Sound stuff
	size_t m_FallSound;
	size_t m_JumpSound;
	bool m_PlayJump;
	bool m_PlayFall;

	
	// Controller Stuff
	std::unique_ptr<IDirectionController> m_pController;
	CoilyDirectionController* m_pCoilyController;
	PlayerDirectionController* m_pPlayerController;
	bool m_IsPlayerControlled;

	
	dae::ObservableVariable<CoilyState> m_State;
	glm::vec2 m_SpawnPos;

	QbertPlayer* m_pPlayer;
	dae::TransformComponent* m_pTransform;
	GridHopper* m_pGridHopper;
	HopperSpriteComponent* m_pSprite;

	dae::TimerComponent* m_pHatchTimer;
	dae::TimerComponent* m_pActionTimer;

	SphereOverlapDetector* m_pCollider;

	QbertSceneBehavior* m_pSceneBehavior;
	float m_FallSpeed;
	float m_SpawnDistance;

	void UpdateSpawn( const dae::UpdateInfo& updateInfo );
	void TeleportToSpawn( );
	void DoFollow( );
	
	// Init stuff, splitted in different methods
	void LoadComponentPointers( );
	void LoadSceneBahavior( const dae::InitInfo& initInfo );
	void AddTimerCallbacks( );
	void AddStateCallback( );
	void AddColliderCallback( ) const;
	void AddTouchdownCallback( );
	void InitController( const dae::InitInfo& );
};
