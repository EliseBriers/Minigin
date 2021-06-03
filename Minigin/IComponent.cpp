#include "MiniginPCH.h"
#include "IComponent.h"
#include "JsonObjectWrapper.h"

dae::IComponent::IComponent( GameObject& gameObject, std::string name )
	: m_GameObject{ gameObject }
	, m_Name{ std::move( name ) }
{
}

void dae::IComponent::FixedUpdate( const UpdateInfo& )
{
}

void dae::IComponent::Update( const UpdateInfo& )
{
}

void dae::IComponent::Draw( Renderer& )
{
}

void dae::IComponent::Init( const InitInfo& )
{
}

void dae::IComponent::Deactivate( )
{
}

dae::GameObject& dae::IComponent::GetGameObject( ) const
{
	return m_GameObject.get( );
}

const std::string& dae::IComponent::GetName( ) const
{
	return m_Name;
}
