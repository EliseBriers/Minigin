#pragma once
#include "SphereOverlapDetector.h"

class TriggerManager
{
public:
	void Update( );
	void AddOverlapDetector( SphereOverlapDetector& overlapDetector );
private:
	std::vector<std::reference_wrapper<SphereOverlapDetector>> m_OverlapDetectors;
};
