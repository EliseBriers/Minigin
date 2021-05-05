#include "MiniginPCH.h"
#include "QbertComponent.h"
#include "InitInfo.h"
#include "KillQbertCommand.h"
#include "UpdateInfo.h"

dae::QbertComponent::QbertComponent( GameObject& gameObject )
	: IComponent{ gameObject }
	, m_TestSound{ }
	, m_HasPlayed{ false }
{
}

void dae::QbertComponent::Init( const InitInfo& initInfo )
{
	initInfo.Input_AddKeyboardCommand( ButtonState::Pressed, SDLK_k, std::make_unique<KillQbertCommand>( *this ) );
	m_TestSound = initInfo.GetSound( "../Data/bell.wav" );
}

void dae::QbertComponent::Update( const UpdateInfo& updateInfo )
{
	if( m_HasPlayed )
		return;
	updateInfo.PushSound( m_TestSound, 30.f );
	m_HasPlayed = true;
}

void dae::QbertComponent::Die( ) const
{
	m_PlayerDied.Notify( );
}

void dae::QbertComponent::AddObserver( const std::function<void( )>& observer )
{
	m_PlayerDied.AddObserver( observer );
}
