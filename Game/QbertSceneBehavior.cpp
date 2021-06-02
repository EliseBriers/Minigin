#include "GamePCH.h"
#include "QbertSceneBehavior.h"

void QbertSceneBehavior::Update( const dae::UpdateInfo& )
{
	m_TriggerManager.Update( );
}

void QbertSceneBehavior::RegisterOverlapDetector( SphereOverlapDetector& overlapDetector )
{
	m_TriggerManager.AddOverlapDetector( overlapDetector );
}
