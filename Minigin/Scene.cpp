#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene( const std::string& name, const ConstructorTag& )
	: m_Name{ name }
{
}

Scene::~Scene( ) = default;

void Scene::Add( std::unique_ptr<GameObject> object, InitInfo& initInfo )
{
	object->Init( initInfo );
	m_Objects.emplace_back( std::move( object ) );
}

void Scene::FixedUpdate( float dt )
{
	for( auto& object : m_Objects )
	{
		object->FixedUpdate( dt );
	}
}

void Scene::Update( float dt )
{
	for( auto& object : m_Objects )
	{
		object->Update( dt );
	}
}

void Scene::Render( Renderer& renderer ) const
{
	for( const auto& object : m_Objects )
	{
		object->Draw( renderer );
	}
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
