#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "IComponent.h"

dae::GameObject::GameObject( const std::string& name )
	: m_Name{ name }
{
}

dae::GameObject::GameObject( std::string&& name )
	: m_Name{ std::move( name ) }
{
}

void dae::GameObject::AddComponent( std::unique_ptr<IComponent> pComponent )
{
	m_pComponents.emplace_back( std::move( pComponent ) );
}

void dae::GameObject::Draw( Renderer& renderer )
{
	for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
	{
		pComponent->Draw( renderer );
	}
}

void dae::GameObject::FixedUpdate( const UpdateInfo& updateInfo )
{
	for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
	{
		pComponent->FixedUpdate( updateInfo );
	}
}

void dae::GameObject::Update( const UpdateInfo& updateInfo )
{
	for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
	{
		pComponent->Update( updateInfo );
	}
}

void dae::GameObject::Init( InitInfo& initInfo )
{
	for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
	{
		pComponent->Init( initInfo );
	}
}

const std::string& dae::GameObject::GetName( ) const
{
	return m_Name;
}

dae::GameObject::~GameObject( ) = default;
