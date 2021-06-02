#include "GamePCH.h"
#include "QbertPlayer.h"
#include "JsonObjectWrapper.h"
#include "GridHopper.h"
#include "InitInfo.h"
#include "SetVarCommand.h"
#include "QbertSpriteComponent.h"
#include "TimerComponent.h"

QbertPlayer::QbertPlayer( dae::GameObject& gameObject, const dae::JsonObjectWrapper&, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_State{ State::Idle }
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
	if( m_State != State::Idle )
		return;


	if( GetInputCount( ) != 1 )
		return;


	if( m_InputLeft )
	{
		m_pSprite->SetDirection( QbertSpriteComponent::Direction::Left );
		m_pGridHopper->Hop( MoveDirection::Left );
		m_State = State::Jumping;
		m_pSprite->SetState( QbertSpriteComponent::State::Jumping );
	}
	if( m_InputRight )
	{
		m_pSprite->SetDirection( QbertSpriteComponent::Direction::Right );
		m_pGridHopper->Hop( MoveDirection::Right );
		m_State = State::Jumping;
		m_pSprite->SetState( QbertSpriteComponent::State::Jumping );
	}
	if( m_InputUp )
	{
		m_pSprite->SetDirection( QbertSpriteComponent::Direction::Up );
		m_pGridHopper->Hop( MoveDirection::Up );
		m_State = State::Jumping;
		m_pSprite->SetState( QbertSpriteComponent::State::Jumping );
	}
	if( m_InputDown )
	{
		m_pSprite->SetDirection( QbertSpriteComponent::Direction::Down );
		m_pGridHopper->Hop( MoveDirection::Down );
		m_State = State::Jumping;
		m_pSprite->SetState( QbertSpriteComponent::State::Jumping );
	}
}

void QbertPlayer::Init( const dae::InitInfo& initInfo )
{
	m_pGridHopper = m_GameObject.get( ).GetComponent<GridHopper>( );
	m_pSprite = m_GameObject.get( ).GetComponent<QbertSpriteComponent>( );
	m_pRespawnTimer = m_GameObject.get( ).GetComponentByName<dae::TimerComponent>( "respawn_timer" );

	if( !m_pGridHopper )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pGridHopper is nullptr" );
	if( !m_pSprite )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pSprite is nullptr" );
	if( !m_pRespawnTimer )
		dae::Logger::LogWarning( "QbertPlayer::Init > m_pRespawnTimer is nullptr" );

	m_pGridHopper->SetTouchdownCallback( [this]( GridHopper::TouchdownType touchdownType )
	{
		OnTouchDown( touchdownType );
	} );

	m_pRespawnTimer->SetCallback( [this]( )
	{
		Respawn( );
	} );
	
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

void QbertPlayer::OnTouchDown( GridHopper::TouchdownType touchdownType )
{
	if( m_State != State::Jumping )
		return;
	if( touchdownType == GridHopper::TouchdownType::Void )
	{
		OnDeath( );
	}
	else
	{
		m_State = State::Idle;
	}
}

void QbertPlayer::Respawn( )
{
	m_pGridHopper->Teleport( 0 );
	m_State = State::Idle;
	m_pSprite->SetDirection( QbertSpriteComponent::Direction::Down );
	m_pSprite->SetState( QbertSpriteComponent::State::Idle );
}

void QbertPlayer::OnDeath( )
{
	m_pRespawnTimer->Start( );
	m_pSprite->SetState( QbertSpriteComponent::State::Dead );
	m_State = State::Dead;
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
