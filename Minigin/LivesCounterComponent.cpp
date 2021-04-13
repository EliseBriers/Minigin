#include "MiniginPCH.h"
#include "LivesCounterComponent.h"
#include "InitInfo.h"
#include "QbertComponent.h"

dae::LivesCounterComponent::LivesCounterComponent( GameObject& gameObject )
	: IComponent{ gameObject }
	, m_Lives{ 10 }
	, m_pTextComponent{ nullptr }
{
}

void dae::LivesCounterComponent::Init( const InitInfo& initInfo )
{
	m_pTextComponent = m_GameObject.get( ).GetComponent<DynamicTextComponent>( );
	m_pTextComponent->SetText( std::to_string( m_Lives ) );

	GameObject* pQbert{ initInfo.Scene_GetGameObject( "Qbert" ) };
	if( !pQbert )return;

	QbertComponent* pQbertComponent{ pQbert->GetComponent<QbertComponent>( ) };
	if( !pQbertComponent ) return;

	pQbertComponent->AddObserver( [this]( )
	{
		OnPlayerDeath( );
	} );
}

void dae::LivesCounterComponent::OnPlayerDeath( )
{
	if( m_Lives == 0 )return;
	--m_Lives;


	m_pTextComponent->SetText(std::to_string(m_Lives));
}
