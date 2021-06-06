#include "GamePCH.h"
#include "QbertPlayer.h"
#include "JsonObjectWrapper.h"
#include "GridHopper.h"
#include "InitInfo.h"
#include "SetVarCommand.h"
#include "HopperSpriteComponent.h"
#include "TimerComponent.h"
#include "SphereOverlapDetector.h"
#include "DiskComponent.h"

QbertPlayer::QbertPlayer( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: BaseComponent{ gameObject, std::move( name ) }
	, m_Controller{ }
	, m_State{ PlayerState::Idle }
	, m_pGridHopper{ nullptr }
	, m_pSprite{ nullptr }
	, m_pRespawnTimer{ nullptr }
	, m_pForgetTimer{ nullptr }
	, m_InputLeft{ false }
	, m_InputRight{ false }
	, m_InputUp{ false }
	, m_InputDown{ false }
	, m_pTransform{ nullptr }
	, m_pSceneBehavior{ nullptr }
	, m_JumpSound{ 0u }
	, m_FallSound{ 0u }
	, m_HitSound{ 0u }
	, m_PlayFallSound{ false }
	, m_PlayHitSound{ false }
	, m_IsFirstPlayer{ jsonObject.GetBool( "is_main_player" ) }
	, m_LastIndex{ -1 }
	, m_LastMoveDirection{ MoveDirection::DownLeft }
{
}

void QbertPlayer::Update( const dae::UpdateInfo& updateInfo )
{
	if( !m_State.Equals( PlayerState::Idle ) )
		return;


	const MoveDirection move{ m_Controller.GetMoveDirection( ) };

	switch( move )
	{
	case MoveDirection::UpLeft:
		m_LastIndex = m_pGridHopper->GetCurrentIndex( );
		m_pGridHopper->Hop( MoveDirection::UpLeft );
		m_State.Set( PlayerState::Jumping );
		m_LastMoveDirection = MoveDirection::UpLeft;
		updateInfo.PushSound( m_JumpSound, 1.f );
		break;
	case MoveDirection::DownRight:
		m_LastIndex = m_pGridHopper->GetCurrentIndex( );
		m_pGridHopper->Hop( MoveDirection::DownRight );
		m_State.Set( PlayerState::Jumping );
		m_LastMoveDirection = MoveDirection::DownRight;
		updateInfo.PushSound( m_JumpSound, 1.f );
		break;
	case MoveDirection::UpRight:
		m_LastIndex = m_pGridHopper->GetCurrentIndex( );
		m_pGridHopper->Hop( MoveDirection::UpRight );
		m_State.Set( PlayerState::Jumping );
		m_LastMoveDirection = MoveDirection::UpRight;
		updateInfo.PushSound( m_JumpSound, 1.f );
		break;
	case MoveDirection::DownLeft:
		m_LastIndex = m_pGridHopper->GetCurrentIndex( );
		m_pGridHopper->Hop( MoveDirection::DownLeft );
		m_State.Set( PlayerState::Jumping );
		m_LastMoveDirection = MoveDirection::DownLeft;
		updateInfo.PushSound( m_JumpSound, 1.f );
		break;
	default: ;
	}
}

void QbertPlayer::Init( const dae::InitInfo& initInfo )
{
	m_JumpSound = initInfo.GetSound( "QBertJump.wav" );
	m_FallSound = initInfo.GetSound( "QBertFall.wav" );
	m_HitSound = initInfo.GetSound( "QBertHit.wav" );

	m_pGridHopper = m_GameObject.get( ).GetComponent<GridHopper>( );
	m_pSprite = m_GameObject.get( ).GetComponent<HopperSpriteComponent>( );
	m_pTransform = m_GameObject.get( ).GetComponent<dae::TransformComponent>( );
	m_pRespawnTimer = m_GameObject.get( ).GetComponentByName<dae::TimerComponent>( "respawn_timer" );
	m_pForgetTimer = m_GameObject.get( ).GetComponentByName<dae::TimerComponent>( "forget_timer" );


	if( !m_pGridHopper )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pGridHopper is nullptr" );
	if( !m_pSprite )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pSprite is nullptr" );
	if( !m_pRespawnTimer )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pRespawnTimer is nullptr" );
	if( !m_pTransform )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pTransform is nullptr" );
	if( !m_pForgetTimer )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pForgetTimer is nullptr" );

	m_pGridHopper->SetTouchdownCallback( [this]( GridHopper::TouchdownType touchdownType )
	{
		OnTouchDown( touchdownType );
	} );

	m_pRespawnTimer->SetCallback( [this]( )
	{
		Respawn( );
	} );

	m_pForgetTimer->SetCallback( [this]( )
	{
		m_LastIndex = -1;
	} );

	m_Controller.InitControls( initInfo, m_IsFirstPlayer );
	InitCollider( );

	m_State.AddObserver( [this]( PlayerState playerState )
	{
		switch( playerState )
		{
		case PlayerState::Moving:
		case PlayerState::Idle:
			m_pSprite->SetState( SpriteState::Idle );
			break;
		case PlayerState::AwaitingMove:
		case PlayerState::Jumping:
			m_pSprite->SetState( SpriteState::Jumping );
			break;
		case PlayerState::Dead:
			m_pSprite->SetState( SpriteState::Dead );
			break;
		default:
			dae::Logger::LogWarning( "QbertPlayer::m_State callback > Invalid playerState" );
		}
	} );

	m_pSceneBehavior = initInfo.Scene_GetSceneBehaviorAs<QbertSceneBehavior>( );
	if( !m_pSceneBehavior )
	{
		dae::Logger::LogWarning( "QbertPlayer::Init > No QbertSceneBehavior found" );
		return;
	}

	m_pSceneBehavior->RegisterPlayer( this );
}

void QbertPlayer::PersistentUpdate( const dae::UpdateInfo& updateInfo )
{
	if( m_PlayFallSound )
	{
		m_PlayFallSound = false;
		updateInfo.PushSound( m_FallSound, 1.f );
	}
	if( m_PlayHitSound )
	{
		m_PlayHitSound = false;
		updateInfo.PushSound( m_HitSound, 1.f );
	}
}

void QbertPlayer::OnTouchDown( GridHopper::TouchdownType touchdownType )
{
	if( m_State.Equals( PlayerState::AwaitingMove ) )
	{
		m_State.Set( PlayerState::Moving );
		return;
	}
	if( ! m_State.Equals( PlayerState::Jumping ) )
		return;
	if( touchdownType == GridHopper::TouchdownType::Void )
	{
		OnDeath( true );
		m_PlayFallSound = true;
	}
	else
	{
		m_State.Set( PlayerState::Idle );
		m_pForgetTimer->Start( );
	}
}

void QbertPlayer::Respawn( )
{
	m_pGridHopper->Teleport( 0 );
	m_pGridHopper->ResetToSpawnIndex( );
	m_pGridHopper->Teleport( static_cast<size_t>(m_pGridHopper->GetCurrentIndex( )) );
	m_State.Set( PlayerState::Idle );
	m_pSprite->SetDirection( SpriteDirection::Down );
	m_pSprite->SetState( SpriteState::Idle );
	m_pSceneBehavior->OnPlayerRespawn( );
}

void QbertPlayer::OnDeath( bool byFall )
{
	if( byFall )
		m_PlayFallSound = true;
	else
		m_PlayHitSound = true;

	m_pRespawnTimer->Start( );
	m_pSprite->SetState( SpriteState::Dead );
	m_State.Set( PlayerState::Dead );
	m_pSceneBehavior->OnPlayerDeath( );
}

void QbertPlayer::RegisterStateObserver( state_observer_t stateObserver )
{
	m_State.AddObserver( std::move( stateObserver ) );
}

void QbertPlayer::NextRotation( ) const
{
	m_pSprite->NextRotation( );
}

void QbertPlayer::SetPosition( const glm::vec2& newPos ) const
{
	m_pTransform->SetPosition( newPos.x, newPos.y, 0.f );
}

glm::vec2 QbertPlayer::GetPosition( ) const
{
	return m_pTransform->GetPosition( );
}

int QbertPlayer::GetLastIndex( ) const
{
	return m_LastIndex;
}

MoveDirection QbertPlayer::GetLastMoveDirection( ) const
{
	return m_LastMoveDirection;
}

void QbertPlayer::InitCollider( )
{
	SphereOverlapDetector* pCollider = m_GameObject.get( ).GetComponent<SphereOverlapDetector>( );
	if( !pCollider )
	{
		dae::Logger::LogError( "QbertPlayer::InitCollider > pCollider was nullptr" );
		return;
	}

	pCollider->SetCallback( [this]( dae::GameObject* pOther, TriggerAction triggerAction )
	{
		if( triggerAction == TriggerAction::Enter )
		{
			DiskComponent* pDisk{ pOther->GetComponent<DiskComponent>( ) };
			if( pDisk )
			{
				m_State.Set( PlayerState::AwaitingMove );
				pDisk->RegisterPlayer( this );
				pDisk->RegisterStateObserver( [this]( DiskState diskState )
				{
					if( diskState == DiskState::Arrived )
					{
						m_State.Set( PlayerState::Jumping );
						m_pGridHopper->HopToIndex( 0u );
					}
				} );
			}
		}
	} );
}
