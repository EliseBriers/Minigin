#include "MiniginPCH.h"
#include "TimerComponent.h"
#include "UpdateInfo.h"
#include "JsonObjectWrapper.h"

dae::TimerComponent::TimerComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name )
	: BaseComponent{ gameObject, std::move( name ) }
	, m_Callback{ VoidCallback }
	, m_ElapsedSec{ 0.f }
	, m_MaxTime{ jsonObject.GetOptionalFloat( "duration", 1.f ) }
	, m_IsActive{ false }
	, m_Loop{ jsonObject.GetBool( "loop" ) }
{
}

void dae::TimerComponent::Update( const UpdateInfo& updateInfo )
{
	if( !m_IsActive )
		return;
	const float dt{ updateInfo.GetDeltaTime( ) };
	m_ElapsedSec += dt;
	if( m_ElapsedSec > m_MaxTime )
	{
		m_Callback( );
		if( m_Loop )
			Start( );
		else
			Stop( );
	}
}

void dae::TimerComponent::Start( )
{
	m_IsActive = true;
	m_ElapsedSec = 0.f;
}

void dae::TimerComponent::Stop( )
{
	m_IsActive = false;
	m_ElapsedSec = 0.f;
}

void dae::TimerComponent::SetLoop( bool loop )
{
	m_Loop = loop;
}

void dae::TimerComponent::SetMaxTime( float maxTime )
{
	m_MaxTime = maxTime;
}

void dae::TimerComponent::SetCallback( const timeout_callback_t& callback )
{
	m_Callback = callback;
}

bool dae::TimerComponent::IsRunning( ) const
{
	return m_IsActive;
}

void dae::TimerComponent::VoidCallback( )
{
}
