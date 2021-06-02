#include "GamePCH.h"
#include "QbertPlayer.h"
#include "JsonObjectWrapper.h"
#include "GridHopper.h"
#include "InitInfo.h"
#include "SetVarCommand.h"
#include "QbertSpriteComponent.h"

QbertPlayer::QbertPlayer( dae::GameObject& gameObject, const dae::JsonObjectWrapper& )
	: IComponent{ gameObject }
	, m_pGridHopper{ nullptr }
	, m_pSprite{ nullptr }
	, m_InputLeft{ false }
	, m_InputRight{ false }
	, m_InputUp{ false }
	, m_InputDown{ false }
{
}

void QbertPlayer::Update( const dae::UpdateInfo& )
{
	m_pSprite->SetState( m_pGridHopper->IsHopping( ) ? QbertSpriteComponent::State::Jumping : QbertSpriteComponent::State::Idle );
	
	if( GetInputCount( ) != 1 )
		return;

	if( !m_pGridHopper->CanHop( ) )
		return;


	if( m_InputLeft )
	{
		m_pSprite->SetDirection( QbertSpriteComponent::Direction::Left );
		m_pGridHopper->Hop( MoveDirection::Left );
	}
	if( m_InputRight )
	{
		m_pSprite->SetDirection( QbertSpriteComponent::Direction::Right );
		m_pGridHopper->Hop( MoveDirection::Right );
	}
	if( m_InputUp )
	{
		m_pSprite->SetDirection( QbertSpriteComponent::Direction::Up );
		m_pGridHopper->Hop( MoveDirection::Up );
	}
	if( m_InputDown )
	{
		m_pSprite->SetDirection( QbertSpriteComponent::Direction::Down );
		m_pGridHopper->Hop( MoveDirection::Down );
	}
}

void QbertPlayer::Init( const dae::InitInfo& initInfo )
{
	m_pGridHopper = m_GameObject.get( ).GetComponent<GridHopper>( );
	m_pSprite = m_GameObject.get( ).GetComponent<QbertSpriteComponent>( );

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

int QbertPlayer::GetInputCount( ) const
{
	int count{ };

	if( m_InputLeft ) ++count;
	if( m_InputRight ) ++count;
	if( m_InputUp ) ++count;
	if( m_InputDown ) ++count;

	return count;
}
