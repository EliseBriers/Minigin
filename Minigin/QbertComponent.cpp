#include "MiniginPCH.h"
#include "QbertComponent.h"
#include "InitInfo.h"
#include "KillQbertCommand.h"

dae::QbertComponent::QbertComponent( GameObject& gameObject )
	: IComponent{ gameObject }
{
}

void dae::QbertComponent::Init( const InitInfo& initInfo )
{
	initInfo.Input_AddKeyboardCommand( ButtonState::Pressed, SDLK_k, std::make_unique<KillQbertCommand>( *this ) );
}

void dae::QbertComponent::Die( ) const
{
	m_PlayerDied.Notify( );
}

void dae::QbertComponent::AddObserver( const std::function<void( )>& observer )
{
	m_PlayerDied.AddObserver( observer );
}
