#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "ManagerMap.h"
#include "InitInfo.h"
#include "UpdateInfo.h"

// #pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#include "TextureComponent.h"
#include "StaticTextComponent.h"
#include "DynamicTextComponent.h"
#include "FpsCounterComponent.h"
#include "SetVarCommand.h"
#include "ImGuiDemoComponent.h"
#include "QbertComponent.h"
#include "LivesCounterComponent.h"

#include <SDL_mixer.h>
#include "ISoundSystem.h"
#include "SDLSoundSystem.h"
#include "ThreadWrapper.h"
// #pragma warning(pop)
#include <document.h>
#include <istreamwrapper.h>
#include <fstream>

using namespace std;
using namespace std::chrono;

dae::Minigin::Minigin( )
	: m_Window{ nullptr }
	, m_SceneManager{ }
	, m_SceneFactory{ "../Data/Scenes/" }
	, m_GameActive{ true }
	, m_pSoundSystem{ nullptr }
{
	RegisterComponents( );
}

void dae::Minigin::Initialize( )
{
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError( ) );
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if( m_Window == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError( ) );
	}

	m_Renderer.Init( m_Window, true );

	m_pSoundSystem = std::make_unique<SDLSoundSystem>( );
	m_pSoundSystem->Init( );

	// Quit when SDL_QUIT happens
	m_InputManager.AddSDLEventCommand( SDL_QUIT, std::make_unique<SetVarCommand<bool>>( m_GameActive, false ) );
}

void dae::Minigin::RegisterComponents( )
{
	m_SceneFactory.RegisterComponent<TextureComponent>( );
	m_SceneFactory.RegisterComponent<TransformComponent>( );
	m_SceneFactory.RegisterComponent<StaticTextComponent>( );
	m_SceneFactory.RegisterComponent<DynamicTextComponent>( );
	m_SceneFactory.RegisterComponent<FpsCounterComponent>( );
}

void dae::Minigin::Cleanup( )
{
	SDL_DestroyWindow( m_Window );
	m_Window = nullptr;
	SDL_Quit( );
}

void dae::Minigin::Run( )
{
	Initialize( );

	// tell the resource manager where he can find the game data
	m_ResourceManager.Init( "../Data/" );

	RegisterComponents( );
	{
		ThreadWrapper audioThread{
			[this]( )
			{
				while( m_GameActive )
					m_pSoundSystem->ProcessSounds( );
			}
		};

		float timeSinceUpdate{ };
		auto lastUpdate = high_resolution_clock::now( );
		while( m_GameActive )
		{
			{
				Scene* activeScene{ m_SceneManager.GetActiveScene( ) };
				if( activeScene )
				{
					InitInfo io{ m_ResourceManager, m_Renderer, m_InputManager, *activeScene, *m_pSoundSystem };
					activeScene->InitGameObjects( io );
				}
			}
			
			// Time
			const auto currentTime = high_resolution_clock::now( );
			const float elapsedSec{ duration_cast<duration<float>>( currentTime - lastUpdate ).count( ) };
			timeSinceUpdate += elapsedSec;
			lastUpdate = currentTime;

			// Update
			m_InputManager.ProcessInput( );
			{
				const UpdateInfo updateInfo{ FixedDeltaTime, *m_pSoundSystem };
				while( timeSinceUpdate >= FixedDeltaTime )
				{
					m_SceneManager.FixedUpdate( updateInfo );
					timeSinceUpdate -= FixedDeltaTime;
				}
			}


			{
				const UpdateInfo updateInfo{ elapsedSec, *m_pSoundSystem };
				m_SceneManager.Update( updateInfo );
			}

			// Render
			m_Renderer.Render( m_SceneManager );

			// Cap framerate
			const auto sleepTime = duration_cast<duration<float>>( currentTime + milliseconds( MsPerFrame ) - high_resolution_clock::now( ) );
			this_thread::sleep_for( sleepTime );
		}
	}

	Cleanup( );
}

void dae::Minigin::SetActiveScene( const std::string& sceneName )
{
	m_SceneManager.SetActiveScene( sceneName );
}

void dae::Minigin::AddSceneFromFile( const std::string& fileName )
{
	m_SceneManager.AddScene( m_SceneFactory.ReadScene( fileName ) );
}

dae::Minigin::~Minigin( ) = default;
