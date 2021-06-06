#pragma once
#include <IComponent.h>
#include "Enums.h"
#include "SpriteSheet.h"
#include "QbertPlayer.h"

class SphereOverlapDetector;

class DiskComponent final : public dae::IComponent
{
	using state_observer_t = dae::ObservableVariable<DiskState>::observer_t;
public:
	DiskComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );

	virtual void Update( const dae::UpdateInfo& ) override;
	virtual void Draw( dae::Renderer& ) override;
	virtual void Init( const dae::InitInfo& ) override;
	virtual void Deactivate( ) override;

	void NextRotation( );
	void RegisterPlayer( QbertPlayer* pPlayer );
	void RegisterStateObserver( state_observer_t observer );
	
	// Rule of 5
	~DiskComponent( ) override = default;
	DiskComponent( const DiskComponent& other ) = delete;
	DiskComponent( DiskComponent&& other ) noexcept = delete;
	DiskComponent& operator=( const DiskComponent& other ) = delete;
	DiskComponent& operator=( DiskComponent&& other ) noexcept = delete;
private:
	glm::vec2 m_EndPos;
	dae::ObservableVariable<DiskState> m_State;
	LevelColor m_Color;
	const float m_MovementSpeed;
	const size_t m_SpawnIndex;
	size_t m_CurrentRotation;
	dae::SpriteSheet m_SpriteSheet;
	dae::TransformComponent* m_pTransform;
	MoveDirection m_SpawnDirection;
	QbertPlayer* m_pPlayer;
	static constexpr size_t m_RotationCount{ 4 };
	const glm::vec2 m_Offset;

	QbertSceneBehavior* m_pSceneBehavior;

	size_t m_DiskSound;
	bool m_PlaySound;

	void UpdateMovement( const dae::UpdateInfo& );
	size_t GetSpriteIndex( ) const;
	void TeleportToSpawn( const dae::InitInfo& initInfo );
	void StartRotationTimer( );
	void StartEndTimer( );
	void InitTransform( );
	void StartMove( );
};
