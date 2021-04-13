#include "MiniginPCH.h"
#include "FpsCounterComponent.h"
#include "DynamicTextComponent.h"

dae::FpsCounterComponent::FpsCounterComponent( GameObject& gameObject )
	: IComponent{ gameObject }
	, m_pTextComponent{ }
{
}

void dae::FpsCounterComponent::Update( float dt )
{
	m_pTextComponent->EmplaceText( std::to_string( uint32_t( 1.f / dt ) ) );
}

void dae::FpsCounterComponent::Init( const InitInfo& )
{
	m_pTextComponent = m_GameObject.get( ).GetComponent<DynamicTextComponent>( );
}
