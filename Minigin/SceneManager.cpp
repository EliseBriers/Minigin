#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate( const UpdateInfo& updateInfo ) const
{
	if( m_pActiveScene )
		m_pActiveScene->FixedUpdate( updateInfo );
}

void dae::SceneManager::Update( const UpdateInfo& updateInfo ) const
{
	if( m_pActiveScene )
		m_pActiveScene->Update( updateInfo );
}

void dae::SceneManager::Render( Renderer& renderer ) const
{
	if( m_pActiveScene )
		m_pActiveScene->Render( renderer );
}

dae::SceneManager::~SceneManager( ) = default;

dae::Scene* dae::SceneManager::GetActiveScene( ) const
{
	return m_pActiveScene;
}

void dae::SceneManager::AddScene( std::unique_ptr<Scene> pScene )
{
	m_Scenes.emplace_back( std::move( pScene ) );
}

void dae::SceneManager::AddSceneAndMakeActive( std::unique_ptr<Scene> pScene )
{
	m_pActiveScene = pScene.get( );
	AddScene( std::move( pScene ) );
}

dae::Scene* dae::SceneManager::GetSceneByName( const std::string& sceneName )
{
	const auto it{
		std::find_if( m_Scenes.begin( ), m_Scenes.end( ), [&sceneName]( std::unique_ptr<Scene>& pScene )
		{
			return pScene->GetName( ) == sceneName;
		} )
	};


	return ( it == m_Scenes.end( ) ) ? nullptr : it->get( );
}

void dae::SceneManager::SetActiveScene( const std::string& sceneName )
{
	m_pActiveScene = GetSceneByName( sceneName );
}
