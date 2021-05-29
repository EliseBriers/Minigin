#include "MiniginPCH.h"
#include "FpsCounterComponent.h"
#include "DynamicTextComponent.h"
#include "UpdateInfo.h"

dae::FpsCounterComponent::FpsCounterComponent( GameObject& gameObject )
	: IComponent{ gameObject }
	, m_pTextComponent{ }
{
}

dae::FpsCounterComponent::FpsCounterComponent( GameObject& gameObject, const rapidjson::Value::Object& )
	: IComponent{ gameObject }
	, m_pTextComponent{ }
{
}

void dae::FpsCounterComponent::Update( const UpdateInfo& updateInfo )
{
	m_pTextComponent->EmplaceText( std::to_string( uint32_t( 1.f / updateInfo.GetDeltaTime( ) ) ) );
}

void dae::FpsCounterComponent::Init( const InitInfo& )
{
	m_pTextComponent = m_GameObject.get( ).GetComponent<DynamicTextComponent>( );
}
