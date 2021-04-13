#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "ICommand.h"
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

dae::InputManager::ControllerInputObject::ControllerInputObject( uint8_t controllerId, ButtonState buttonState, ControllerButton button, std::unique_ptr<ICommand> command )
	: m_Command{ std::move( command ) }
	, m_UpdateOn{ buttonState }
	, m_Button{ button }
	, m_WasPressed{ false }
	, m_Id{ controllerId }
{
}

void dae::InputManager::ControllerInputObject::Process( const XINPUT_STATE& state )
{
	const bool pressed{ bool( state.Gamepad.wButtons & static_cast<WORD>(m_Button) ) };
	if( pressed != m_WasPressed )
	{
		if( m_UpdateOn == ButtonState::Pressed ? pressed : !pressed )
		{
			m_Command->Execute( );
		}
	}
	m_WasPressed = pressed;
}

uint8_t dae::InputManager::ControllerInputObject::GetId( ) const
{
	return m_Id;
}

dae::InputManager::InputManager( )
	: m_MaxControllers{ 4u }
{
}

void dae::InputManager::ProcessInput( )
{
	ProcessControllerInput( );
	ProcessSDLInput( );
}

void dae::InputManager::AddControllerCommand( uint8_t controllerId, ButtonState buttonState, ControllerButton button, std::unique_ptr<ICommand> command )
{
	m_InputObjects.emplace_back( controllerId, buttonState, button, std::move( command ) );
}

void dae::InputManager::AddKeyboardCommand( ButtonState buttonState, SDL_Keycode button, std::unique_ptr<ICommand> command )
{
	m_SDLKeys.insert( { SDLInputKey{ buttonState, button }, std::move( command ) } );
}

void dae::InputManager::AddSDLEventCommand( Uint32 event, std::unique_ptr<ICommand> command )
{
	m_SDLEvents.insert( { event, std::move( command ) } );
}

void dae::InputManager::ProcessControllerInput( )
{
	m_States.clear( );
	for( DWORD i{ }; i < m_MaxControllers; ++i )
	{
		XINPUT_STATE state{ };
		RtlZeroMemory( &state, sizeof(state) );
		const DWORD errorCode{ XInputGetState( i, &state ) };
		if( errorCode == ERROR_SUCCESS )
		{
			m_States.push_back( state );
		}
	}

	for( ControllerInputObject& io : m_InputObjects )
	{
		const uint8_t idx{ io.GetId( ) };
		if( idx < m_States.size( ) )
		{
			io.Process( m_States[idx] );
		}
	}
}

void dae::InputManager::ProcessSDLInput( ) const
{
	SDL_Event e;
	while( SDL_PollEvent( &e ) )
	{
		if( e.type == SDL_KEYDOWN )
		{
			ProcessSDLKeyPress( { ButtonState::Pressed, e.key.keysym.sym } );
		}
		if( e.type == SDL_KEYUP )
		{
			ProcessSDLKeyPress( { ButtonState::Released, e.key.keysym.sym } );
		}
		ProcessSDLEvent( e.type );
	}
}

void dae::InputManager::ProcessSDLKeyPress( const SDLInputKey& key ) const
{
	using sdl_key_map_range_t = std::pair<sdl_key_map_t::const_iterator, sdl_key_map_t::const_iterator>;
	const sdl_key_map_range_t range{ m_SDLKeys.equal_range( key ) };
	sdl_key_map_t::const_iterator it{ range.first };
	for( ; it != range.second; ++it )
	{
		it->second->Execute( );
	}
}

void dae::InputManager::ProcessSDLEvent( Uint32 event ) const
{
	using sdl_event_map_range_t = std::pair<sdl_event_map_t::const_iterator, sdl_event_map_t::const_iterator>;
	const sdl_event_map_range_t range{ m_SDLEvents.equal_range( event ) };
	sdl_event_map_t::const_iterator it{ range.first };
	for( ; it != range.second; ++it )
	{
		it->second->Execute( );
	}
}
