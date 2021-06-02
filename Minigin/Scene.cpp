#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Logger.h"
#include "SceneBehavior.h"

dae::Scene::Scene( const std::string& name )
	: m_Name{ name }
	, m_pSceneBehavior{ nullptr }
	, m_InitializedBehavior{ false }
{
}

dae::Scene::~Scene( ) = default;

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
		object->FixedUpdate( updateInfo );
	}
}

void dae::Scene::Update( const UpdateInfo& updateInfo )
{
	if( m_pSceneBehavior )
		m_pSceneBehavior->Update( updateInfo );

	for( auto& object : m_Objects )
	{
		object->Update( updateInfo );
	}
}

void dae::Scene::Render( Renderer& renderer ) const
{
	if( m_pSceneBehavior )
		m_pSceneBehavior->Draw( renderer );
	for( const auto& object : m_Objects )
	{
		object->Draw( renderer );
	}
}

void dae::Scene::Init( InitInfo& initInfo )
{
	for( std::unique_ptr<GameObject>& pObject : m_UninitializedObjects )
	{
		pObject->Init( initInfo );
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
	const auto it{
		std::find_if( m_Objects.cbegin( ), m_Objects.cend( ), [&name]( const std::unique_ptr<GameObject>& ptr )
		{
			return ptr->GetName( ) == name;
		} )
	};

	return it == m_Objects.cend( ) ? nullptr : it->get( );
}

void dae::Scene::SetBehavior( std::unique_ptr<SceneBehavior> pBehavior )
{
	if( m_pSceneBehavior )
		Logger::LogWarning( "dae::Scene::SetBehavior > This scene already has a behavior, overwriting" );
	m_pSceneBehavior = std::move( pBehavior );
	m_pSceneBehavior->SetScene( this );
	m_InitializedBehavior = false;
}
