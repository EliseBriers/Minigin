#include "GamePCH.h"
#include "MenuSceneBehavior.h"

MenuSceneBehavior::MenuSceneBehavior( const scene_array_t& sceneArray )
	: m_SceneArray{ sceneArray }
{
}

void MenuSceneBehavior::OnSceneSelected( size_t index )
{
	if( index > m_SceneArray.size( ) )
	{
		dae::Logger::LogWarning( "MenuSceneBehavior::OnSceneSelected > Invalid index: " + std::to_string( index ) );
		return;
	}

	GetScene( ).GetSceneManager( )->SetActiveScene( m_SceneArray[index] );
}
