#pragma once

class SphereOverlapDetector;

class TriggerManager
{
public:
	void Update( );
	void AddOverlapDetector( SphereOverlapDetector& overlapDetector );
	void RemoveOverlapDetector( const SphereOverlapDetector& overlapDetector );
	void QueueUnRegisterOverlapDetector( SphereOverlapDetector& overlapDetector );
private:
	std::vector<std::reference_wrapper<SphereOverlapDetector>> m_OverlapDetectors;
	std::vector<std::reference_wrapper<SphereOverlapDetector>> m_RemoveQueue;
};
