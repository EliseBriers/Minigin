#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Logger.h"
#include "SceneBehavior.h"

dae::Scene::Scene( const std::string& name )
	: m_pSceneManager{ nullptr }
	, m_Name{ name }
	, m_pSceneBehavior{ nullptr }
	, m_InitializedBehavior{ false }
{
}

dae::SceneManager* dae::Scene::GetSceneManager( ) const
{
	return m_pSceneManager;
}

void dae::Scene::SetSceneManager( SceneManager* pSceneManager )
{
	m_pSceneManager = pSceneManager;
}

dae::Scene::~Scene( ) = default;

void dae::Scene::MoveActiveGameObjects( )
{
}

const std::string& dae::Scene::GetName( ) const
{
	return m_Name;
}

void dae::Scene::Add( std::unique_ptr<GameObject> object )
{
	m_UninitializedObjects.emplace_back( std::move( object ) );
}

void dae::Scene::FixedUpdate( const UpdateInfo& updateInfo )
{
	if( m_pSceneBehavior )
		m_pSceneBehavior->FixedUpdate( updateInfo );

	for( auto& object : m_Objects )
	{
		if( object->IsActive( ) && !object->IsPaused( ) )
			object->FixedUpdate( updateInfo );
	}
}

void dae::Scene::Update( const UpdateInfo& updateInfo )
{
	if( m_pSceneBehavior )
		m_pSceneBehavior->Update( updateInfo );


	for( auto& object : m_Objects )
	{
		if( object->IsActive( ) && !object->IsPaused( ) )
			object->Update( updateInfo );
	}
}

void dae::Scene::Render( Renderer& renderer ) const
{
	if( m_pSceneBehavior )
		m_pSceneBehavior->Draw( renderer );
	for( const auto& object : m_Objects )
	{
		if( object->IsActive( ) )
			object->Draw( renderer );
	}
}

void dae::Scene::Init( InitInfo& initInfo )
{
	for( std::unique_ptr<GameObject>& pObject : m_UninitializedObjects )
	{
		pObject->Init( initInfo );
		pObject->Activate( );
		m_Objects.emplace_back( std::move( pObject ) );
	}
	m_UninitializedObjects.clear( );
	if( !m_InitializedBehavior && m_pSceneBehavior )
	{
		m_pSceneBehavior->Init( initInfo );
		m_InitializedBehavior = true;
	}
}

dae::GameObject* dae::Scene::GetGameObject( const std::string& name ) const
{
	auto it{
		std::find_if( m_Objects.begin( ), m_Objects.end( ), [&name]( const std::unique_ptr<GameObject>& ptr )
		{
			return ptr->GetName( ) == name;
		} )
	};

	if( it == m_Objects.end( ) )
	{
		it = std::find_if( m_UninitializedObjects.begin( ), m_UninitializedObjects.end( ), [&name]( const std::unique_ptr<GameObject>& ptr )
		{
			return ptr->GetName( ) == name;
		} );
		return it == m_UninitializedObjects.end( ) ? nullptr : it->get( );
	}
	return it->get( );
}

void dae::Scene::SetBehavior( std::unique_ptr<SceneBehavior> pBehavior )
{
	if( m_pSceneBehavior )
		Logger::LogWarning( "dae::Scene::SetBehavior > This scene already has a behavior, overwriting" );
	m_pSceneBehavior = std::move( pBehavior );
	m_pSceneBehavior->SetScene( this );
	m_InitializedBehavior = false;
}
