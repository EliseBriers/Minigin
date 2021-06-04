#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "IComponent.h"

dae::GameObject::GameObject( const std::string& name )
	: m_Name{ name }
	, m_IsActive{ true }
{
}

dae::GameObject::GameObject( std::string&& name )
	: m_Name{ std::move( name ) }
	, m_IsActive{ true }
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

void dae::GameObject::Deactivate( )
{
	m_IsActive = false;
	for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
	{
		pComponent->Deactivate( );
	}
}

void dae::GameObject::Activate( )
{
	m_IsActive = true;
	for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
	{
		pComponent->Activate( );
	}
}

void dae::GameObject::Pause( )
{
	m_IsPaused = true;
	for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
	{
		pComponent->Pause( );
	}
}

void dae::GameObject::UnPause( )
{
	m_IsPaused = false;
	for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
	{
		pComponent->UnPause( );
	}
}

const std::string& dae::GameObject::GetName( ) const
{
	return m_Name;
}

bool dae::GameObject::IsActive( ) const
{
	return m_IsActive;
}

bool dae::GameObject::IsPaused( ) const
{
	return m_IsPaused;
}

dae::GameObject::~GameObject( ) = default;
