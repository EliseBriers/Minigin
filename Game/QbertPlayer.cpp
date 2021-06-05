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
	: IComponent{ gameObject, std::move( name ) }
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
	, m_IsFirstPlayer{ jsonObject.GetBool( "is_main_player" ) }
	, m_LastIndex{ -1 }
	, m_LastMoveDirection{ MoveDirection::DownLeft }
{
}

void QbertPlayer::Update( const dae::UpdateInfo& )
{
	if( !m_State.Equals( PlayerState::Idle ) )
		return;


	if( GetInputCount( ) != 1 )
		return;


	if( m_InputLeft )
	{
		m_pGridHopper->Hop( MoveDirection::UpLeft );
		m_State.Set( PlayerState::Jumping );
		m_LastMoveDirection = MoveDirection::UpLeft;
	}
	if( m_InputRight )
	{
		m_pGridHopper->Hop( MoveDirection::DownRight );
		m_State.Set( PlayerState::Jumping );
		m_LastMoveDirection = MoveDirection::DownRight;
	}
	if( m_InputUp )
	{
		m_pGridHopper->Hop( MoveDirection::UpRight );
		m_State.Set( PlayerState::Jumping );
		m_LastMoveDirection = MoveDirection::UpRight;
	}
	if( m_InputDown )
	{
		m_pGridHopper->Hop( MoveDirection::DownLeft );
		m_State.Set( PlayerState::Jumping );
		m_LastMoveDirection = MoveDirection::DownLeft;
	}
}

void QbertPlayer::Init( const dae::InitInfo& initInfo )
{
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

	InitInputs( initInfo );
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
		OnDeath( );
	}
	else
	{
		m_State.Set( PlayerState::Idle );
		m_LastIndex = m_pGridHopper->GetCurrentIndex( );
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

void QbertPlayer::OnDeath( )
{
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

int QbertPlayer::GetInputCount( ) const
{
	int count{ };

	if( m_InputLeft ) ++count;
	if( m_InputRight ) ++count;
	if( m_InputUp ) ++count;
	if( m_InputDown ) ++count;

	return count;
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

void QbertPlayer::InitInputs( const dae::InitInfo& initInfo )
{
	const SDL_Keycode left{ m_IsFirstPlayer ? SDLK_LEFT : SDLK_a };
	const SDL_Keycode right{ m_IsFirstPlayer ? SDLK_RIGHT : SDLK_d };
	const SDL_Keycode down{ m_IsFirstPlayer ? SDLK_DOWN : SDLK_s };
	const SDL_Keycode up{ m_IsFirstPlayer ? SDLK_UP : SDLK_w };
	
	// Left
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Pressed, left, std::make_unique<dae::SetVarCommand<bool>>( m_InputLeft, true ) );
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Released, left, std::make_unique<dae::SetVarCommand<bool>>( m_InputLeft, false ) );
	// Right
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Pressed, right, std::make_unique<dae::SetVarCommand<bool>>( m_InputRight, true ) );
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Released, right, std::make_unique<dae::SetVarCommand<bool>>( m_InputRight, false ) );
	// Up
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Pressed, up, std::make_unique<dae::SetVarCommand<bool>>( m_InputUp, true ) );
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Released, up, std::make_unique<dae::SetVarCommand<bool>>( m_InputUp, false ) );
	// Down
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Pressed, down, std::make_unique<dae::SetVarCommand<bool>>( m_InputDown, true ) );
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Released, down, std::make_unique<dae::SetVarCommand<bool>>( m_InputDown, false ) );
}
