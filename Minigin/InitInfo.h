#pragma once
#include <string>
#include <SDL.h>
#include "InputManager.h"
#include "Scene.h"

namespace dae
{
	class Renderer;
	class Texture2D;
	class ResourceManager;
	class GameObject;
	class Scene;
	class ISoundSystem;

	class InitInfo
	{
	public:
		InitInfo( ResourceManager& resourceManager, Renderer& renderer, InputManager& inputManager, Scene& scene, ISoundSystem& soundSystem );
		const Texture2D& Resource_GetTexture( const std::string& file ) const;
		const Texture2D& Resource_GetTextTexture( const std::string& text, const std::string& fileName, uint32_t size, const SDL_Color& color ) const;

		void Input_AddControllerCommand( uint8_t controllerId, ButtonState buttonState, ControllerButton button, std::unique_ptr<ICommand> command ) const;
		void Input_AddKeyboardCommand( ButtonState buttonState, SDL_Keycode button, std::unique_ptr<ICommand> command ) const;
		void Input_AddSDLEventCommand( Uint32 event, std::unique_ptr<ICommand> command ) const;

		size_t GetSound( const std::string& fileName ) const;

		template <typename T>
		T* Scene_GetSceneBehaviorAs( ) const;

		~InitInfo( ) = default;
		GameObject* Scene_GetGameObject( const std::string& name ) const;
		InitInfo( const InitInfo& other ) = delete;
		InitInfo( InitInfo&& other ) noexcept = delete;
		InitInfo& operator=( const InitInfo& other ) = delete;
		InitInfo& operator=( InitInfo&& other ) noexcept = delete;
	private:
		ResourceManager& m_ResourceManager;
		Renderer& m_Renderer;
		InputManager& m_InputManager;
		Scene& m_Scene;
		ISoundSystem& m_SoundSystem;
	};

	template <typename T>
	T* InitInfo::Scene_GetSceneBehaviorAs( ) const
	{
		return m_Scene.GetSceneBehaviorAs<T>( );
	}
}
