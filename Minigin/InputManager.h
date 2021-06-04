#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <vector>
#include <memory>
#pragma comment(lib, "XInput.lib")
#include <unordered_map>
#include <SDL.h>
#include "ICommand.h"

namespace dae
{
	class Command;

	enum class ButtonState
	{
		Pressed,
		Released
	};

	enum class ControllerButton : WORD
	{
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y,
		BumperL = XINPUT_GAMEPAD_LEFT_SHOULDER,
		BumperR = XINPUT_GAMEPAD_RIGHT_SHOULDER
	};

	class InputManager final
	{
	private:
		// ReSharper disable once CppInconsistentNaming
		// using SDLInputKey = std::pair<ButtonState, SDL_Keycode>;
		struct SDLInputKey
		{
			ButtonState PlayerState;
			SDL_Keycode KeyCode;

			friend bool operator==( const SDLInputKey& lhs, const SDLInputKey& rhs )
			{
				return lhs.PlayerState == rhs.PlayerState
					&& lhs.KeyCode == rhs.KeyCode;
			}

			friend bool operator!=( const SDLInputKey& lhs, const SDLInputKey& rhs )
			{
				return !( lhs == rhs );
			}

			struct Hash
			{
				size_t operator()( const SDLInputKey& val ) const
				{
					// I sold my soul to Satan in exchange for this code
					return ( ( std::hash<int>( )( val.KeyCode ) ^ ( std::hash<int>( )( static_cast<int>(val.PlayerState) ) << 1 ) ) >> 1 );

					// As funny as that is I feel obligated to include my actual source:
					// https://stackoverflow.com/questions/29058254/how-to-fix-this-c3848-error-on-vs2013
				}
			};
		};

		using sdl_key_map_t = std::unordered_multimap<SDLInputKey, std::unique_ptr<ICommand>, SDLInputKey::Hash>;
		using sdl_event_map_t = std::unordered_multimap<Uint32, std::unique_ptr<ICommand>>;

		class ControllerInputObject
		{
		public:
			ControllerInputObject( uint8_t controllerId, ButtonState buttonState, ControllerButton button, std::unique_ptr<ICommand> command );
			void Process( const XINPUT_STATE& state );
			uint8_t GetId( ) const;
		private:
			std::unique_ptr<ICommand> m_Command;
			ButtonState m_UpdateOn;
			ControllerButton m_Button;
			bool m_WasPressed;
			uint8_t m_Id;
		};

	public:
		InputManager( );

		void ProcessInput( );
		void AddControllerCommand( uint8_t controllerId, ButtonState buttonState, ControllerButton button, std::unique_ptr<ICommand> command );
		void AddKeyboardCommand( ButtonState buttonState, SDL_Keycode button, std::unique_ptr<ICommand> command );
		void AddSDLEventCommand( Uint32 event, std::unique_ptr<ICommand> command );
	private:
		// XInput
		void ProcessControllerInput( );
		std::vector<XINPUT_STATE> m_States;
		uint8_t m_MaxControllers;
		std::vector<ControllerInputObject> m_InputObjects;

		// SDL
		void ProcessSDLInput( ) const;
		// SDL: Keyboard
		void ProcessSDLKeyPress( const SDLInputKey& key ) const;
		sdl_key_map_t m_SDLKeys;
		// SDL: General Events
		void ProcessSDLEvent( Uint32 event ) const;
		sdl_event_map_t m_SDLEvents;
	};
}
