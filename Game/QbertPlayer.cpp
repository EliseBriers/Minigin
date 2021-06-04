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

QbertPlayer::QbertPlayer( dae::GameObject& gameObject, const dae::JsonObjectWrapper&, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_State{ PlayerState::Idle }
	, m_pGridHopper{ nullptr }
	, m_pSprite{ nullptr }
	, m_pRespawnTimer{ nullptr }
	, m_InputLeft{ false }
	, m_InputRight{ false }
	, m_InputUp{ false }
	, m_InputDown{ false }
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
		m_pSprite->SetDirection( SpriteDirection::Left );
		m_pGridHopper->Hop( MoveDirection::Left );
		m_State.Set( PlayerState::Jumping );
	}
	if( m_InputRight )
	{
		m_pSprite->SetDirection( SpriteDirection::Right );
		m_pGridHopper->Hop( MoveDirection::Right );
		m_State.Set( PlayerState::Jumping );
	}
	if( m_InputUp )
	{
		m_pSprite->SetDirection( SpriteDirection::Up );
		m_pGridHopper->Hop( MoveDirection::Up );
		m_State.Set( PlayerState::Jumping );
	}
	if( m_InputDown )
	{
		m_pSprite->SetDirection( SpriteDirection::Down );
		m_pGridHopper->Hop( MoveDirection::Down );
		m_State.Set( PlayerState::Jumping );
	}
}

void QbertPlayer::Init( const dae::InitInfo& initInfo )
{
	m_pGridHopper = m_GameObject.get( ).GetComponent<GridHopper>( );
	m_pSprite = m_GameObject.get( ).GetComponent<HopperSpriteComponent>( );
	m_pTransform = m_GameObject.get( ).GetComponent<dae::TransformComponent>( );
	m_pRespawnTimer = m_GameObject.get( ).GetComponentByName<dae::TimerComponent>( "respawn_timer" );


	if( !m_pGridHopper )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pGridHopper is nullptr" );
	if( !m_pSprite )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pSprite is nullptr" );
	if( !m_pRespawnTimer )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pRespawnTimer is nullptr" );
	if( !m_pTransform )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pTransform is nullptr" );

	m_pGridHopper->SetTouchdownCallback( [this]( GridHopper::TouchdownType touchdownType )
	{
		OnTouchDown( touchdownType );
	} );

	m_pRespawnTimer->SetCallback( [this]( )
	{
		Respawn( );
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
	}
}

void QbertPlayer::Respawn( )
{
	m_pGridHopper->Teleport( 0 );
	m_State.Set( PlayerState::Idle );
	m_pSprite->SetDirection( SpriteDirection::Down );
	m_pSprite->SetState( SpriteState::Idle );
}

void QbertPlayer::OnDeath( )
{
	m_pRespawnTimer->Start( );
	m_pSprite->SetState( SpriteState::Dead );
	m_State.Set( PlayerState::Dead );
}

void QbertPlayer::RegisterStateObserver( state_observer_t stateObserver )
{
	m_State.AddObserver( std::move( stateObserver ) );
}

void QbertPlayer::NextRotation( )
{
	m_pSprite->NextRotation( );
}

void QbertPlayer::SetPosition( const glm::vec2& newPos ) const
{
	m_pTransform->SetPosition( newPos.x, newPos.y, 0.f );
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
	// Left
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Pressed, SDLK_LEFT, std::make_unique<dae::SetVarCommand<bool>>( m_InputLeft, true ) );
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Released, SDLK_LEFT, std::make_unique<dae::SetVarCommand<bool>>( m_InputLeft, false ) );
	// Right
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Pressed, SDLK_RIGHT, std::make_unique<dae::SetVarCommand<bool>>( m_InputRight, true ) );
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Released, SDLK_RIGHT, std::make_unique<dae::SetVarCommand<bool>>( m_InputRight, false ) );
	// Up
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Pressed, SDLK_UP, std::make_unique<dae::SetVarCommand<bool>>( m_InputUp, true ) );
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Released, SDLK_UP, std::make_unique<dae::SetVarCommand<bool>>( m_InputUp, false ) );
	// Down
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Pressed, SDLK_DOWN, std::make_unique<dae::SetVarCommand<bool>>( m_InputDown, true ) );
	initInfo.Input_AddKeyboardCommand( dae::ButtonState::Released, SDLK_DOWN, std::make_unique<dae::SetVarCommand<bool>>( m_InputDown, false ) );
}
