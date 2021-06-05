#include "GamePCH.h"
#include "RandomBouncerComponent.h"
#include <JsonObjectWrapper.h>
#include "HopperSpriteComponent.h"
#include "GridHopper.h"
#include "SphereOverlapDetector.h"
#include "QbertPlayer.h"
#include "EnumHelpers.h"
#include <UpdateInfo.h>
#include "QbertSceneBehavior.h"
#include <InitInfo.h>

RandomBouncerComponent::RandomBouncerComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_RandomBounceDirection{ RandomBounceDirection::Down }
	, m_State{ RandomBouncerState::Spawning }
	, m_DownDirection{ 0.f, 1.f }
	, m_pSprite{ nullptr }
	, m_pGridHopper{ nullptr }
	, m_pCollider{ nullptr }
	, m_pNextActionTimer{ nullptr }
	, m_pFallTimer{ nullptr }
	, m_pTransform{ nullptr }
	, m_FallSpeed{ 350.f }
	, m_SpawnDistance{ m_FallSpeed * jsonObject.GetFloat( "spawn_time" ) }
	, m_RespawnTime{ jsonObject.GetFloat( "respawn_time" ) }
	, m_SpawnPos{ 0.f, 0.f }
	, m_IsLethal{ }
	, m_pSceneBehavior{ nullptr }
{
}

void RandomBouncerComponent::Init( const dae::InitInfo& initInfo )
{
	GetComponentPointers( );
	m_pSceneBehavior = initInfo.Scene_GetSceneBehaviorAs<QbertSceneBehavior>( );
	if( !m_pSceneBehavior )
	{
		dae::Logger::LogWarning( "RandomBouncerComponent::Init > No QbertSceneBehavior found" );
		return;
	}
	m_pSceneBehavior->RegisterEnemy( &GetGameObject( ) );
	switch( m_pSprite->GetHopperType( ) )
	{
	case HopperType::Ugg: ;
		m_RandomBounceDirection = RandomBounceDirection::UpLeft;
		m_IsLethal = true;
		break;
	case HopperType::Wrongway:
		m_RandomBounceDirection = RandomBounceDirection::UpRight;
		m_IsLethal = true;
		break;
	case HopperType::Slick:
	case HopperType::Sam:
		m_IsLethal = false;
		m_RandomBounceDirection = RandomBounceDirection::Down;
		break;

	default:
		dae::Logger::LogWarning( "RandomBouncerComponent::Init > m_pSprite->GetHopperType( ) returned invalid hopper type" );
	}

	m_pCollider->SetCallback( [this]( dae::GameObject* pOther, TriggerAction triggerAction )
	{
		if( triggerAction == TriggerAction::Enter )
		{
			// dae::Logger::LogInfo( GetGameObject( ).GetName( ) + " Hit by Object " + pOther->GetName( ) );
			QbertPlayer* pPlayer{ pOther->GetComponent<QbertPlayer>( ) };
			if( pPlayer )
			{
				if( m_State.Equals( RandomBouncerState::Active ) || m_State.Equals( RandomBouncerState::Spawning ) )
				{
					if( m_IsLethal )
						KillPlayer( pPlayer );
					else
						OnDeath( true );
				}
			}
		}
	} );

	m_pNextActionTimer->SetCallback( [this]( )
	{
		if( m_State.Equals( RandomBouncerState::Active ) )
			m_pGridHopper->Hop( EnumHelpers::GetRandomMoveDirection( m_RandomBounceDirection ) );
	} );

	m_pGridHopper->SetTouchdownCallback( [this]( GridHopper::TouchdownType touchDownType )
	{
		if( touchDownType == GridHopper::TouchdownType::Block )
		{
			m_pNextActionTimer->Start( );
		}
		else
		{
			m_State.Set( RandomBouncerState::Falling );
		}
	} );

	m_DownDirection = -m_pGridHopper->GetUpDirection( );

	m_State.AddObserver( [this]( RandomBouncerState state )
	{
		switch( state )
		{
		case RandomBouncerState::Spawning:
			m_pGridHopper->SetState( GridHopper::HopperState::NoControl );
			m_pGridHopper->ResetToSpawnIndex( );
			break;
		case RandomBouncerState::Active:
			m_pGridHopper->SetState( GridHopper::HopperState::Idle );
			m_pNextActionTimer->Start( );
			break;
		case RandomBouncerState::Falling:
			m_pGridHopper->SetState( GridHopper::HopperState::NoControl );
			m_pFallTimer->Start( );
		}
	} );

	m_pFallTimer->SetCallback( [this]( )
	{
		OnDeath( false );
	} );
}

void RandomBouncerComponent::Activate( )
{
	// m_pNextActionTimer->Start( );
	m_State.Set( RandomBouncerState::Spawning );

	const int spawnIdx{ m_pGridHopper->GetCurrentIndex( ) };
	const glm::vec2 cubePos{ m_pGridHopper->GetCubeGrid( )->GetCubePos( spawnIdx, m_pGridHopper->GetOffset( ) ) };
	const glm::vec2 spawnPos{ cubePos + m_SpawnDistance * -m_DownDirection };
	m_SpawnPos = cubePos;
	m_pTransform->SetPosition( spawnPos.x, spawnPos.y, 0.f );
}

void RandomBouncerComponent::Update( const dae::UpdateInfo& updateInfo )
{
	switch( m_State.Get( ) )
	{
	case RandomBouncerState::Spawning:
		UpdateSpawn( updateInfo );
		break;
	case RandomBouncerState::Active:
		break;
	case RandomBouncerState::Falling:
		UpdateFall( updateInfo );
	}
}

void RandomBouncerComponent::GetComponentPointers( )
{
	m_pTransform = GetGameObject( ).GetComponent<dae::TransformComponent>( );
	if( !m_pTransform )
	{
		dae::Logger::LogWarning( "RandomBouncerComponent::Init > m_pTransform was nullptr" );
		return;
	}

	m_pGridHopper = GetGameObject( ).GetComponent<GridHopper>( );
	if( !m_pGridHopper )
	{
		dae::Logger::LogWarning( "RandomBouncerComponent::Init > m_pGridHopper was nullptr" );
		return;
	}

	m_pNextActionTimer = GetGameObject( ).GetComponentByName<dae::TimerComponent>( "next_action_timer" );
	if( !m_pNextActionTimer )
	{
		dae::Logger::LogWarning( "RandomBouncerComponent::Init > m_pNextActionTimer was nullptr" );
		return;
	}

	m_pFallTimer = GetGameObject( ).GetComponentByName<dae::TimerComponent>( "fall_timer" );
	if( !m_pFallTimer )
	{
		dae::Logger::LogWarning( "RandomBouncerComponent::Init > m_pFallTimer was nullptr" );
		return;
	}

	m_pSprite = GetGameObject( ).GetComponent<HopperSpriteComponent>( );
	if( !m_pSprite )
	{
		dae::Logger::LogWarning( "RandomBouncerComponent::Init > m_pSprite was nullptr" );
		return;
	}

	m_pCollider = GetGameObject( ).GetComponent<SphereOverlapDetector>( );
	if( !m_pCollider )
	{
		dae::Logger::LogWarning( "RandomBouncerComponent::Init > m_pCollider was nullptr" );
	}
}

void RandomBouncerComponent::UpdateFall( const dae::UpdateInfo& updateInfo ) const
{
	const float dt{ updateInfo.GetDeltaTime( ) };
	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const glm::vec2 newPos{ pos + m_FallSpeed * m_DownDirection * dt };
	m_pTransform->SetPosition( newPos.x, newPos.y, 0.f );
}

void RandomBouncerComponent::UpdateSpawn( const dae::UpdateInfo& updateInfo )
{
	const float dt{ updateInfo.GetDeltaTime( ) };
	if( m_pTransform->MoveTo( m_SpawnPos, m_FallSpeed * dt ) )
	{
		m_State.Set( RandomBouncerState::Active );
	}
}

void RandomBouncerComponent::OnDeath( bool killedByPlayer ) const
{
	GetGameObject( ).Deactivate( );
	m_pSceneBehavior->RegisterKilledEnemy( &GetGameObject( ), killedByPlayer, m_RespawnTime );
	if( killedByPlayer )
		m_pSceneBehavior->AddPoints( 300u );
}

void RandomBouncerComponent::KillPlayer( QbertPlayer* pPlayer )
{
	pPlayer->OnDeath( );
}
