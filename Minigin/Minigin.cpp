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

using namespace std;
using namespace std::chrono;

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
}

dae::Minigin::Minigin( )
	: m_Window{ nullptr }
	, m_GameActive{ true }
	, m_pSoundSystem{ nullptr }
{
}
/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame( )
{
	auto& scene = m_SceneManager.CreateScene( "Demo" );
	InitInfo io{ m_ResourceManager, m_Renderer, m_InputManager, scene, *m_pSoundSystem };


	{
		auto go = std::make_unique<GameObject>( "Background" );
		auto texture{ std::make_unique<TextureComponent>( "background.jpg", *go ) };
		auto transform{ std::make_unique<TransformComponent>( *go, 0.f, 0.f, 0.f ) };
		go->AddComponent( std::move( texture ) );
		go->AddComponent( std::move( transform ) );
		scene.Add( std::move( go ), io );
	}

	{
		auto go = std::make_unique<GameObject>( "Logo" );
		auto texture{ std::make_unique<TextureComponent>( "logo.png", *go ) };
		auto transform{ std::make_unique<TransformComponent>( *go, 216.f, 180.f, 0.f ) };
		go->AddComponent( std::move( texture ) );
		go->AddComponent( std::move( transform ) );
		scene.Add( std::move( go ), io );
	}

	{
		auto go = std::make_unique<GameObject>( "Title" );
		auto texture{ std::make_unique<StaticTextComponent>( *go, "Programming 4 Assignment", "Lingua.otf", 36u ) };
		auto transform{ std::make_unique<TransformComponent>( *go, 80.f, 20.f, 0.f ) };
		go->AddComponent( std::move( texture ) );
		go->AddComponent( std::move( transform ) );
		scene.Add( std::move( go ), io );
	}

	{
		auto go = std::make_unique<GameObject>( "FPS Counter" );
		auto texture{ std::make_unique<DynamicTextComponent>( *go, ' ', uint8_t( 'z' - ' ' + 1 ), 0.f, "Lingua.otf", 36u ) };
		auto framerateCounter{ std::make_unique<FpsCounterComponent>( *go ) };
		auto transform{ std::make_unique<TransformComponent>( *go, 20.f, 20.f, 0.f ) };
		go->AddComponent( std::move( texture ) );
		go->AddComponent( std::move( transform ) );
		go->AddComponent( std::move( framerateCounter ) );
		scene.Add( std::move( go ), io );
	}

	{
		auto go = std::make_unique<GameObject>( "Qbert" );
		auto qbert{ std::make_unique<QbertComponent>( *go ) };
		go->AddComponent( std::move( qbert ) );
		scene.Add( std::move( go ), io );
	}

	{
		auto go = std::make_unique<GameObject>( "Lives Counter" );
		auto counter{ std::make_unique<LivesCounterComponent>( *go ) };
		auto text{ std::make_unique<DynamicTextComponent>( *go, '0', uint8_t( 10u ), 0.f, "Lingua.otf", 12u ) };
		auto transform{ std::make_unique<TransformComponent>( *go, 20.f, 80.f, 0.f ) };
		go->AddComponent( std::move( counter ) );
		go->AddComponent( std::move( text ) );
		go->AddComponent( std::move( transform ) );
		scene.Add( std::move( go ), io );
	}

	{
		// m_InputManager.AddKeyboardCommand( ButtonState::Pressed, SDLK_ESCAPE, std::make_unique<SetVarCommand>( m_GameActive, false ) );
		m_InputManager.AddSDLEventCommand( SDL_QUIT, std::make_unique<SetVarCommand<bool>>( m_GameActive, false ) );
	}

	//{
	//	auto go = std::make_unique<GameObject>( );
	//	auto imGuiDemo{ std::make_unique<ImGuiDemoComponent>( *go ) };
	//	go->AddComponent( std::move( imGuiDemo ) );
	//	go->Init( io );
	//	scene.Add( std::move( go ) );
	//}

	//{
	//	const uint16_t id{ m_pSoundSystem->GetSound( "../Data/bell.wav" ) };
	//	m_pSoundSystem->PushSound( id, 30.f );
	//}
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

	LoadGame( );
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

dae::Minigin::~Minigin( ) = default;
