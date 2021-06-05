#include "GamePCH.h"
#include "CoilyComponent.h"
#include "QbertPlayer.h"
#include "SphereOverlapDetector.h"
#include <TimerComponent.h>
#include "QbertSceneBehavior.h"
#include "GridHopper.h"
#include "EnumHelpers.h"
#include "HopperSpriteComponent.h"

CoilyComponent::CoilyComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& /*jsonObject*/, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_State{ CoilyState::SpawningEgg }
	, m_SpawnPos{ }
	, m_pPlayer{ nullptr }
	, m_pTransform{ nullptr }
	, m_pGridHopper{ nullptr }
	, m_pSprite{ nullptr }
	, m_pHatchTimer{ nullptr }
	, m_pActionTimer{ nullptr }
	, m_pFallTimer{ nullptr }
	, m_pCollider{ nullptr }
	, m_pSceneBehavior{ nullptr }
	, m_FallSpeed{ 350.f }
	, m_SpawnDistance{ m_FallSpeed * 10.f }
{
}

void CoilyComponent::Update( const dae::UpdateInfo& updateInfo )
{
	switch( m_State.Get( ) )
	{
	case CoilyState::SpawningEgg:
		UpdateSpawn( updateInfo );
		break;
	case CoilyState::Egg:
		break;
	case CoilyState::Hatching:
		break;
	case CoilyState::FollowingPlayer:
		break;
	case CoilyState::Falling:
		break;
	case CoilyState::Dead:
		break;
	default: ;
	}
}

void CoilyComponent::Init( const dae::InitInfo& initInfo )
{
	LoadComponentPointers( );
	LoadSceneBahavior( initInfo );
	AddTimerCallbacks( );
	AddStateCallback( );
	AddTouchdownCallback( );
	AddColliderCallback( );

	m_pSceneBehavior->RegisterEnemy( &GetGameObject( ) );
}

void CoilyComponent::AddStateObserver( state_observer_t stateObserver )
{
	m_State.AddObserver( std::move( stateObserver ) );
}

void CoilyComponent::NextAction( )
{
	switch( m_State.Get( ) )
	{
	case CoilyState::FollowingPlayer:
		dae::Logger::LogInfo( "Following player" );
		break;
	case CoilyState::Egg:
		{
			const MoveDirection direction{ EnumHelpers::GetRandomMoveDirection( RandomBounceDirection::Down ) };
			if( m_pGridHopper->GetIndexAfterHop( direction ) == -1 )
			{
				m_State.Set( CoilyState::Hatching );
			}
			else
			{
				m_pGridHopper->Hop( direction );
			}
			break;
		}
	default:
		dae::Logger::LogWarning( "CoilyComponent::NextAction > Function called with invalid state" );
	}
}

void CoilyComponent::Activate( )
{
	m_State.Set( CoilyState::SpawningEgg );

	TeleportToSpawn( );
}

void CoilyComponent::Pause( )
{
	m_pFallTimer->Stop( );
	m_pActionTimer->Stop( );
	m_pHatchTimer->Stop( );
}

void CoilyComponent::UpdateSpawn( const dae::UpdateInfo& updateInfo )
{
	const float dt{ updateInfo.GetDeltaTime( ) };
	if( m_pTransform->MoveTo( m_SpawnPos, m_FallSpeed * dt ) )
	{
		m_State.Set( CoilyState::Egg );
	}
}

void CoilyComponent::TeleportToSpawn( )
{
	m_pGridHopper->ResetToSpawnIndex( );
	const glm::vec2 cubePos{ m_pGridHopper->GetCubeGrid( )->GetCubePos( 0, m_pGridHopper->GetOffset( ) ) };
	const glm::vec2 spawnPos{ cubePos + glm::vec2{ 0.f, -m_SpawnDistance } };
	m_SpawnPos = cubePos;
	m_pTransform->SetPosition( spawnPos.x, spawnPos.y, 0.f );
}

void CoilyComponent::LoadComponentPointers( )
{
	m_pTransform = GetGameObject( ).GetComponent<dae::TransformComponent>( );
	if( !m_pTransform )
	{
		dae::Logger::LogWarning( "CoilyComponent::GetComponentPointers > m_pTransform is nullptr" );
		return;
	}
	m_pSprite = GetGameObject( ).GetComponent<HopperSpriteComponent>( );
	if( !m_pSprite )
	{
		dae::Logger::LogWarning( "CoilyComponent::GetComponentPointers > m_pSprite is nullptr" );
		return;
	}
	m_pGridHopper = GetGameObject( ).GetComponent<GridHopper>( );
	if( !m_pGridHopper )
	{
		dae::Logger::LogWarning( "CoilyComponent::GetComponentPointers > m_pGridHopper is nullptr" );
		return;
	}

	m_pActionTimer = GetGameObject( ).GetComponentByName<dae::TimerComponent>( "next_action_timer" );
	m_pFallTimer = GetGameObject( ).GetComponentByName<dae::TimerComponent>( "fall_timer" );
	m_pHatchTimer = GetGameObject( ).GetComponentByName<dae::TimerComponent>( "egg_hatch_timer" );

	if( !m_pActionTimer )
	{
		dae::Logger::LogWarning( "CoilyComponent::GetComponentPointers > m_pActionTimer is nullptr" );
		return;
	}
	if( !m_pFallTimer )
	{
		dae::Logger::LogWarning( "CoilyComponent::GetComponentPointers > m_pFallTimer is nullptr" );
		return;
	}
	if( !m_pHatchTimer )
	{
		dae::Logger::LogWarning( "CoilyComponent::GetComponentPointers > m_pHatchTimer is nullptr" );
		return;
	}

	m_pCollider = GetGameObject( ).GetComponent<SphereOverlapDetector>( );

	if( !m_pCollider )
	{
		dae::Logger::LogWarning( "CoilyComponent::GetComponentPointers > m_pCollider is nullptr" );
	}
}

void CoilyComponent::LoadSceneBahavior( const dae::InitInfo& initInfo )
{
	m_pSceneBehavior = initInfo.Scene_GetSceneBehaviorAs<QbertSceneBehavior>( );
	if( !m_pSceneBehavior )
	{
		dae::Logger::LogWarning( "CoilyComponent::GetSceneBahavior > m_pSceneBehavior is nullptr" );
	}
}

void CoilyComponent::AddTimerCallbacks( )
{
	m_pActionTimer->SetCallback( [this]( )
	{
		if( m_State.Equals( CoilyState::FollowingPlayer ) || m_State.Equals( CoilyState::Egg ) )
			NextAction( );
		else
			dae::Logger::LogWarning( "CoilyComponent.m_pActionTimer->Callback > Action callback while invalid state" );
	} );

	m_pFallTimer->SetCallback( [this]( )
	{
		if( m_State.Equals( CoilyState::Falling ) )
			m_State.Set( CoilyState::Dead );
		else
			dae::Logger::LogWarning( "CoilyComponent.m_pFallTimer->Callback > Fall timer expired while coily is not falling" );
	} );

	m_pHatchTimer->SetCallback( [this]( )
	{
		if( m_State.Equals( CoilyState::Hatching ) )
			m_State.Set( CoilyState::FollowingPlayer );
		else
			dae::Logger::LogWarning( "CoilyComponent.m_pHatchTimer->Callback > Hatch timer expired while coily is not hatching" );
	} );
}

void CoilyComponent::AddStateCallback( )
{
	m_State.AddObserver( [this]( const CoilyState& state )
	{
		switch( state )
		{
		case CoilyState::SpawningEgg:
			m_pSprite->SetType( HopperType::Egg );
			TeleportToSpawn();
			dae::Logger::LogInfo( "Coily reached state: \"SpawningEgg\"" );
			break;
		case CoilyState::Egg:
			m_pSprite->SetType( HopperType::Egg );
			dae::Logger::LogInfo( "Coily reached state: \"Egg\"" );
			m_pActionTimer->Start( );
			break;
		case CoilyState::Hatching:
			m_pSprite->SetType( HopperType::Egg );
			dae::Logger::LogInfo( "Coily reached state: \"Hatching\"" );
			m_pHatchTimer->Start( );
			m_pActionTimer->Stop( );
			break;
		case CoilyState::FollowingPlayer:
			m_pSprite->SetType( HopperType::Coily );
			dae::Logger::LogInfo( "Coily reached state: \"FollowingPlayer\"" );
			m_pActionTimer->Start( );
			break;
		case CoilyState::Falling:
			dae::Logger::LogInfo( "Coily reached state: \"Falling\"" );
			m_pFallTimer->Start( );
			m_pActionTimer->Stop( );
			break;
		case CoilyState::Dead:
			dae::Logger::LogInfo( "Coily reached state: \"Dead\"" );
			break;
		default:
			dae::Logger::LogWarning( "CoilyComponent.m_State.Callback > Invalid state" );
		}
	} );
}

void CoilyComponent::AddColliderCallback( ) const
{
	m_pCollider->SetCallback( []( dae::GameObject* pOther, TriggerAction action )
	{
		if( action == TriggerAction::Enter )
		{
			QbertPlayer* pPlayer{ pOther->GetComponent<QbertPlayer>( ) };
			if( pPlayer )
			{
				pPlayer->OnDeath( );
			}
		}
	} );
}

void CoilyComponent::AddTouchdownCallback( )
{
	using TouchdownType = GridHopper::TouchdownType;
	m_pGridHopper->SetTouchdownCallback( [this]( TouchdownType type )
	{
		if( type == TouchdownType::Block )
		{
			m_pActionTimer->Start( );
		}
		else
		{
			m_State.Set( CoilyState::Falling );
		}
	} );
}
