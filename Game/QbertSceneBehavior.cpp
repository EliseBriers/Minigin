#include "GamePCH.h"
#include "QbertSceneBehavior.h"
#include <Scene.h>

QbertSceneBehavior::QbertSceneBehavior( const std::string& nextLevel, bool isLastLevel )
	: m_NextLevel{ nextLevel }
	, m_IsLastLevel{ isLastLevel }
{
}

void QbertSceneBehavior::Update( const dae::UpdateInfo& )
{
	m_TriggerManager.Update( );
}

void QbertSceneBehavior::RegisterOverlapDetector( SphereOverlapDetector& overlapDetector )
{
	m_TriggerManager.AddOverlapDetector( overlapDetector );
}

void QbertSceneBehavior::UnRegisterOverlapDetector( const SphereOverlapDetector& sphereOverlapDetector )
{
	m_TriggerManager.RemoveOverlapDetector( sphereOverlapDetector );
}

void QbertSceneBehavior::EndLevel( ) const
{
	if( !m_IsLastLevel )
		GetScene( ).GetSceneManager( )->SetActiveScene( m_NextLevel );
}
