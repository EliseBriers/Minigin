#include "MiniginPCH.h"
#include "IComponent.h"

dae::IComponent::IComponent( GameObject& gameObject )
	: m_GameObject{ gameObject }
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

dae::GameObject& dae::IComponent::GetGameObject( ) const
{
	return m_GameObject.get( );
}
