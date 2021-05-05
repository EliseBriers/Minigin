#pragma once
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
struct SDL_Window;

namespace dae
{
	class ISoundSystem;

	class Minigin
	{
	public:
		Minigin( );
		void Initialize( );
		void LoadGame( );
		void Cleanup( );
		void Run( );
		~Minigin( );
	private:
		static constexpr int MsPerFrame = 8; //16 for 60 fps, 33 for 30 fps
		static constexpr float FixedDeltaTime{ 1.f / 144.f };
		SDL_Window* m_Window;
		Renderer m_Renderer;
		InputManager m_InputManager;
		SceneManager m_SceneManager;
		ResourceManager m_ResourceManager;
		bool m_GameActive;
		std::unique_ptr<ISoundSystem> m_pSoundSystem;
	};
}
