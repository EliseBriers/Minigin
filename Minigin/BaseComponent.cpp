#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "JsonObjectWrapper.h"

dae::BaseComponent::BaseComponent( GameObject& gameObject, std::string name )
	: m_GameObject{ gameObject }
	, m_Name{ std::move( name ) }
{
}

void dae::BaseComponent::FixedUpdate( const UpdateInfo& )
{
}

void dae::BaseComponent::PersistentFixedUpdate( const UpdateInfo& )
{
}

void dae::BaseComponent::Update( const UpdateInfo& )
{
}

void dae::BaseComponent::PersistentUpdate( const UpdateInfo& )
{
}

void dae::BaseComponent::Draw( Renderer& )
{
}

void dae::BaseComponent::Init( const InitInfo& )
{
}

void dae::BaseComponent::Deactivate( )
{
}

void dae::BaseComponent::Activate( )
{
}

void dae::BaseComponent::Pause( )
{
}

void dae::BaseComponent::UnPause( )
{
}

dae::GameObject& dae::BaseComponent::GetGameObject( ) const
{
	return m_GameObject.get( );
}

const std::string& dae::BaseComponent::GetName( ) const
{
	return m_Name;
}
