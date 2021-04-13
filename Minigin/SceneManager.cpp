#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate( float dt )
{
	for( auto& scene : m_Scenes )
	{
		scene->FixedUpdate( dt );
	}
}

void dae::SceneManager::Update( float dt )
{
	for( auto& scene : m_Scenes )
	{
		scene->Update( dt );
	}
}

void dae::SceneManager::Render( Renderer& renderer )
{
	for( const auto& scene : m_Scenes )
	{
		scene->Render( renderer );
	}
}

dae::SceneManager::~SceneManager( ) = default;

dae::Scene& dae::SceneManager::CreateScene( const std::string& name )
{
	auto pScene = std::make_unique<Scene>( name, Scene::ConstructorTag{ } );
	Scene& sceneRef{ *pScene };
	m_Scenes.emplace_back( std::move( pScene ) );
	return sceneRef;
}
