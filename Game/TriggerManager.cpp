#include "GamePCH.h"
#include "TriggerManager.h"

void TriggerManager::Update( )
{
	const size_t size{ m_OverlapDetectors.size( ) };
	for( size_t i{ }; i < size; ++i )
	{
		for( size_t j{ i + 1 }; j < size; ++j )
		{
			m_OverlapDetectors[i].get( ).DetectOverlap( m_OverlapDetectors[j] );
		}
	}
}

void TriggerManager::AddOverlapDetector( SphereOverlapDetector& overlapDetector )
{
	m_OverlapDetectors.emplace_back( overlapDetector );
}
