#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

Scene::Scene( const std::string& name )
	: m_Name{ name }
{
}

Scene::~Scene( ) = default;

const std::string& Scene::GetName( ) const
{
	return m_Name;
}

void Scene::Add( std::unique_ptr<GameObject> object )
{
	m_UninitializedObjects.emplace_back( std::move( object ) );
}

void Scene::FixedUpdate( const UpdateInfo& updateInfo )
{
	for( auto& object : m_Objects )
	{
		object->FixedUpdate( updateInfo );
	}
}

void Scene::Update( const UpdateInfo& updateInfo )
{
	for( auto& object : m_Objects )
	{
		object->Update( updateInfo );
	}
}

void Scene::Render( Renderer& renderer ) const
{
	for( const auto& object : m_Objects )
	{
		object->Draw( renderer );
	}
}

void Scene::InitGameObjects( InitInfo& initInfo )
{
	for( std::unique_ptr<GameObject>& pObject : m_UninitializedObjects )
	{
		pObject->Init( initInfo );
		m_Objects.emplace_back( std::move( pObject ) );
	}
	m_UninitializedObjects.clear( );
}

GameObject* Scene::GetGameObject( const std::string& name ) const
{
	const auto it{
		std::find_if( m_Objects.cbegin( ), m_Objects.cend( ), [&name]( const std::unique_ptr<GameObject>& ptr )
		{
			return ptr->GetName( ) == name;
		} )
	};

	return it == m_Objects.cend( ) ? nullptr : it->get( );
}
