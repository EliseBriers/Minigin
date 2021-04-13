#include "MiniginPCH.h"
#include "IComponent.h"

dae::IComponent::IComponent( GameObject& gameObject )
	: m_GameObject{ gameObject }
{
}

void dae::IComponent::FixedUpdate( float )
{
}

void dae::IComponent::Update( float )
{
}

void dae::IComponent::Draw(Renderer& )
{
}

void dae::IComponent::Init( const InitInfo& )
{
}
