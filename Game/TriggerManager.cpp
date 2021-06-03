#include "GamePCH.h"
#include "TriggerManager.h"
#include "SphereOverlapDetector.h"

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

void TriggerManager::RemoveOverlapDetector( const SphereOverlapDetector& overlapDetector )
{
	const auto it{
		std::find_if( m_OverlapDetectors.begin( ), m_OverlapDetectors.end( ), [&overlapDetector]( std::reference_wrapper<SphereOverlapDetector> ref )
		{
			return ref.get( ).Id == overlapDetector.Id;
		} )
	};

	if( it != m_OverlapDetectors.end( ) )
		m_OverlapDetectors.erase( it );
	else
		dae::Logger::LogWarning( "TriggerManager::RemoveOverlapDetector > No overlapDetector of id " + std::to_string( overlapDetector.Id.GetValue( ) ) + " found" );
}
