#include "GamePCH.h"
#include "PlayerDirectionController.h"
#include "SetVarCommand.h"

PlayerDirectionController::PlayerDirectionController( )
	: m_InputLeft{ false }
	, m_InputRight{ false }
	, m_InputUp{ false }
	, m_InputDown{ false }
{
}

void PlayerDirectionController::InitControls( const dae::InitInfo& initInfo, bool isFirstPlayer )
{
	const SDL_Keycode left{ isFirstPlayer ? SDLK_LEFT : SDLK_a };
	const SDL_Keycode right{ isFirstPlayer ? SDLK_RIGHT : SDLK_d };
	const SDL_Keycode down{ isFirstPlayer ? SDLK_DOWN : SDLK_s };
	const SDL_Keycode up{ isFirstPlayer ? SDLK_UP : SDLK_w };

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

MoveDirection PlayerDirectionController::GetMoveDirection( )
{
	if( GetInputCount( ) != 1 )
		return MoveDirection::None;

	if( m_InputLeft )
		return MoveDirection::UpLeft;
	if( m_InputRight )
		return MoveDirection::DownRight;
	if( m_InputUp )
		return MoveDirection::UpRight;
	if( m_InputDown )
		return MoveDirection::DownLeft;

	return MoveDirection::None;
}

int PlayerDirectionController::GetInputCount( ) const
{
	int count{ };

	if( m_InputLeft ) ++count;
	if( m_InputRight ) ++count;
	if( m_InputUp ) ++count;
	if( m_InputDown ) ++count;

	return count;
}
