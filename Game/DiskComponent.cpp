#include "GamePCH.h"
#include "DiskComponent.h"
#include "SphereOverlapDetector.h"
#include "InitInfo.h"
#include "CubeGrid.h"
#include "TimerComponent.h"
#include "QbertPlayer.h"
#include <UpdateInfo.h>
#include <glm/detail/func_geometric.inl>
#include "EnumHelpers.h"

DiskComponent::DiskComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_State{ DiskState::Idle }
	, m_Color{ EnumHelpers::StringToLevelColor( jsonObject.GetString( "color" ) ) }
	, m_MovementSpeed{ jsonObject.GetOptionalFloat( "speed", 5.f ) }
	, m_SpawnIndex{ jsonObject.GetUint( "spawn_index" ) }
	, m_CurrentRotation{ }
	, m_SpriteSheet{ jsonObject.GetObjectWrapper( "sprite_sheet" ) }
	, m_pTransform{ nullptr }
	, m_SpawnDirection{ }
	, m_pPlayer{ nullptr }
	, m_Offset{ 0.f, -0.25f }
{
}

void DiskComponent::Update( const dae::UpdateInfo& updateInfo )
{
	if( m_State.Equals( DiskState::Moving ) )
		UpdateMovement( updateInfo );
}

void DiskComponent::Draw( dae::Renderer& renderer )
{
	if( m_State.Equals( DiskState::Inactive ) )
		return;

	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const float scale{ m_pTransform->GetScale( ) };

	m_SpriteSheet.Draw( renderer, pos, { 0.5f, 0.5f }, scale, GetSpriteIndex( ) );
}

void DiskComponent::Init( const dae::InitInfo& initInfo )
{
	m_SpriteSheet.Init( initInfo );

	InitTransform( );

	TeleportToSpawn( initInfo );
	StartRotationTimer( );
}

void DiskComponent::NextRotation( )
{
	++m_CurrentRotation;
	m_CurrentRotation %= m_RotationCount;
	if( m_pPlayer && m_State.Equals( DiskState::Moving ) )
	{
		m_pPlayer->NextRotation( );
	}
}

void DiskComponent::RegisterPlayer( QbertPlayer* pPlayer )
{
	m_pPlayer = pPlayer;
	m_pPlayer->RegisterStateObserver( [this]( PlayerState playerState )
	{
		if( m_State.Equals( DiskState::Idle ) )
			if( playerState == PlayerState::Moving )
				StartMove( );
	} );
}

void DiskComponent::RegisterStateObserver( state_observer_t observer )
{
	m_State.AddObserver( std::move( observer ) );
}

void DiskComponent::UpdateMovement( const dae::UpdateInfo& updateInfo )
{
	const float dt{ updateInfo.GetDeltaTime( ) };

	const glm::vec2 pos{ m_pTransform->GetPosition( ) };

	if( m_pTransform->MoveTo( m_EndPos, m_MovementSpeed * dt ) )
	{
		m_State.Set( DiskState::Arrived );
		StartEndTimer( );
	}
	m_pPlayer->SetPosition( pos );
}

size_t DiskComponent::GetSpriteIndex( ) const
{
	return static_cast<size_t>(m_Color) * m_RotationCount + m_CurrentRotation;
}

void DiskComponent::TeleportToSpawn( const dae::InitInfo& initInfo )
{
	dae::GameObject* pGameObject{ initInfo.Scene_GetGameObject( "CubeGrid" ) };
	if( !pGameObject )
	{
		dae::Logger::LogError( "DiskComponent::TeleportToSpawn > pGameObject was nullptr" );
		return;
	}
	CubeGrid* pCubeGrid{ pGameObject->GetComponent<CubeGrid>( ) };
	if( !pCubeGrid )
	{
		dae::Logger::LogError( "DiskComponent::TeleportToSpawn > pCubeGrid was nullptr" );
		return;
	}

	const CubeGrid::Cube& cube{ pCubeGrid->GetCube( m_SpawnIndex ) };

	if( cube.ConnectionDownLeft == -1 )
		m_SpawnDirection = MoveDirection::DownLeft;
	else if( cube.ConnectionUpRight == -1 )
		m_SpawnDirection = MoveDirection::UpRight;
	else if( cube.ConnectionUpLeft == -1 )
		m_SpawnDirection = MoveDirection::UpLeft;
	else if( cube.ConnectionDownRight == -1 )
		m_SpawnDirection = MoveDirection::DownRight;
	else
		dae::Logger::LogWarning( "DiskComponent::TeleportToSpawn > Couldn't find proper spawn location" );

	const glm::vec2 spawnPos{ pCubeGrid->CalculateImaginaryBlockPos( m_SpawnIndex, m_SpawnDirection, m_Offset ) };

	m_pTransform->SetPosition( spawnPos.x, spawnPos.y, 0.f );
	m_EndPos = pCubeGrid->CalculateImaginaryBlockPos( 0u, m_SpawnDirection, m_Offset );
}

void DiskComponent::StartRotationTimer( )
{
	dae::TimerComponent* pTimer = GetGameObject( ).GetComponentByName<dae::TimerComponent>( "RotationTimer" );
	if( !pTimer )
	{
		dae::Logger::LogError( "DiskComponent::StartRotationTimer > pTimer was nullptr" );
		return;
	}

	pTimer->SetCallback( [this]( )
	{
		NextRotation( );
	} );
	pTimer->Start( );
}

void DiskComponent::StartEndTimer( )
{
	dae::TimerComponent* pTimer = GetGameObject( ).GetComponentByName<dae::TimerComponent>( "EndTimer" );
	if( !pTimer )
	{
		dae::Logger::LogError( "DiskComponent::StartEndTimer > pTimer was nullptr" );
		return;
	}

	pTimer->SetCallback( [this]( )
	{
		m_State.Set( DiskState::Inactive );
		GetGameObject( ).Deactivate( );
	} );
	pTimer->Start( );
}

void DiskComponent::InitTransform( )
{
	m_pTransform = m_GameObject.get( ).GetComponent<dae::TransformComponent>( );
	if( !m_pTransform )
	{
		dae::Logger::LogError( "DiskSpriteComponent::Init > m_pTransform was nullptr" );
	}
}

void DiskComponent::StartMove( )
{
	m_State.Set( DiskState::Moving );
}
