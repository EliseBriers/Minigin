#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate(const UpdateInfo& updateInfo)
{
	for( auto& scene : m_Scenes )
	{
		scene->FixedUpdate( updateInfo );
	}
}

void dae::SceneManager::Update(const UpdateInfo& updateInfo)
{
	for( auto& scene : m_Scenes )
	{
		scene->Update( updateInfo );
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
