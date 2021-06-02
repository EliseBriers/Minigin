#pragma once
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SceneFactory.h"
struct SDL_Window;

namespace dae
{
	class ISoundSystem;

	class Minigin
	{
	public:
		Minigin( );
		void Run( );

		template <typename T>
		void RegisterComponent( );

		void SetActiveScene( const std::string& sceneName );
		void AddSceneFromFile( const std::string& fileName );
		
		// Rule of 5
		~Minigin( );
		Scene* GetActiveScene( ) const;
		Minigin( const Minigin& other ) = delete;
		Minigin( Minigin&& other ) noexcept = delete;
		Minigin& operator=( const Minigin& other ) = delete;
		Minigin& operator=( Minigin&& other ) noexcept = delete;
	private:
		void RegisterComponents( );
		void Initialize( );
		void Cleanup( );
		
		static ResourceManager::DataPaths GetDataPaths( );
		
		static constexpr int MsPerFrame = 8; //16 for 60 fps, 33 for 30 fps
		static constexpr float FixedDeltaTime{ 1.f / 144.f };
		SDL_Window* m_Window;
		Renderer m_Renderer;
		InputManager m_InputManager;
		SceneManager m_SceneManager;
		ResourceManager m_ResourceManager;
		// ComponentFactory m_ComponentFactory;
		// GameObjectFactory m_GameObjectFactory;
		SceneFactory m_SceneFactory;
		bool m_GameActive;
		std::unique_ptr<ISoundSystem> m_pSoundSystem;
	};

	template <typename T>
	void Minigin::RegisterComponent( )
	{
		m_SceneFactory.RegisterComponent<T>( );
	}
}
