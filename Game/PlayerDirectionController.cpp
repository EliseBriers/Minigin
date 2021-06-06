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
	const dae::ButtonState pressed{ dae::ButtonState::Pressed };
	const dae::ButtonState released{ dae::ButtonState::Released };

	{
		const SDL_Keycode left{ isFirstPlayer ? SDLK_LEFT : SDLK_a };
		const SDL_Keycode right{ isFirstPlayer ? SDLK_RIGHT : SDLK_d };
		const SDL_Keycode down{ isFirstPlayer ? SDLK_DOWN : SDLK_s };
		const SDL_Keycode up{ isFirstPlayer ? SDLK_UP : SDLK_w };

	
		// Left
		initInfo.Input_AddKeyboardCommand( pressed, left, std::make_unique<dae::SetVarCommand<bool>>( m_InputLeft, true ) );
		initInfo.Input_AddKeyboardCommand( released, left, std::make_unique<dae::SetVarCommand<bool>>( m_InputLeft, false ) );
		// Right
		initInfo.Input_AddKeyboardCommand( pressed, right, std::make_unique<dae::SetVarCommand<bool>>( m_InputRight, true ) );
		initInfo.Input_AddKeyboardCommand( released, right, std::make_unique<dae::SetVarCommand<bool>>( m_InputRight, false ) );
		// Up
		initInfo.Input_AddKeyboardCommand( pressed, up, std::make_unique<dae::SetVarCommand<bool>>( m_InputUp, true ) );
		initInfo.Input_AddKeyboardCommand( released, up, std::make_unique<dae::SetVarCommand<bool>>( m_InputUp, false ) );
		// Down
		initInfo.Input_AddKeyboardCommand( pressed, down, std::make_unique<dae::SetVarCommand<bool>>( m_InputDown, true ) );
		initInfo.Input_AddKeyboardCommand( released, down, std::make_unique<dae::SetVarCommand<bool>>( m_InputDown, false ) );
	}
	{
		const uint8_t controllerId{ ( isFirstPlayer ? 0u : 1u ) };
		const dae::ControllerButton left{ dae::ControllerButton::ButtonLeft };
		const dae::ControllerButton right{ dae::ControllerButton::ButtonRight };
		const dae::ControllerButton up{ dae::ControllerButton::ButtonUp };
		const dae::ControllerButton down{ dae::ControllerButton::ButtonDown };
	

		// Left
		initInfo.Input_AddControllerCommand( controllerId, pressed, left, std::make_unique<dae::SetVarCommand<bool>>( m_InputLeft, true ) );
		initInfo.Input_AddControllerCommand( controllerId, released, left, std::make_unique<dae::SetVarCommand<bool>>( m_InputLeft, false ) );
		// Right
		initInfo.Input_AddControllerCommand( controllerId, pressed, right, std::make_unique<dae::SetVarCommand<bool>>( m_InputRight, true ) );
		initInfo.Input_AddControllerCommand( controllerId, released, right, std::make_unique<dae::SetVarCommand<bool>>( m_InputRight, false ) );
		// Up
		initInfo.Input_AddControllerCommand( controllerId, pressed, up, std::make_unique<dae::SetVarCommand<bool>>( m_InputUp, true ) );
		initInfo.Input_AddControllerCommand( controllerId, released, up, std::make_unique<dae::SetVarCommand<bool>>( m_InputUp, false ) );
		// Down
		initInfo.Input_AddControllerCommand( controllerId, pressed, down, std::make_unique<dae::SetVarCommand<bool>>( m_InputDown, true ) );
		initInfo.Input_AddControllerCommand( controllerId, released, down, std::make_unique<dae::SetVarCommand<bool>>( m_InputDown, false ) );
	}
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
