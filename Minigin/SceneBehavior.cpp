#include "MiniginPCH.h"
#include "SceneBehavior.h"
#include "Logger.h"

// ReSharper disable CppMemberFunctionMayBeStatic
void dae::SceneBehavior::Draw( Renderer& )
{
}

void dae::SceneBehavior::FixedUpdate( const UpdateInfo& )
{
}

void dae::SceneBehavior::Update( const UpdateInfo& )
{
}

void dae::SceneBehavior::Init( InitInfo& )
{
}

void dae::SceneBehavior::SetScene( Scene* pScene )
{
	m_pScene = pScene;
}

dae::Scene& dae::SceneBehavior::GetScene( ) const
{
	if( !m_pScene )
		Logger::LogError( "dae::SceneBehavior::GetScene > Scene was nullptr, this is not allowed" );
	return *m_pScene;
}
// ReSharper restore CppMemberFunctionMayBeStatic
dae::SceneBehavior::SceneBehavior( )
	: m_pScene{ nullptr }
{
}
