#pragma once
#include <SceneBehavior.h>
#include "TriggerManager.h"

class QbertSceneBehavior final : public dae::SceneBehavior
{
public:

	QbertSceneBehavior( const std::string& nextLevel, bool isLastLevel );

	void Update( const dae::UpdateInfo& ) override;
	void RegisterOverlapDetector( SphereOverlapDetector& overlapDetector );
	void UnRegisterOverlapDetector( const SphereOverlapDetector& sphereOverlapDetector );
	void EndLevel( ) const;
	
	// Rule of 5
	~QbertSceneBehavior( ) override = default;
	QbertSceneBehavior( const QbertSceneBehavior& other ) = delete;
	QbertSceneBehavior( QbertSceneBehavior&& other ) noexcept = delete;
	QbertSceneBehavior& operator=( const QbertSceneBehavior& other ) = delete;
	QbertSceneBehavior& operator=( QbertSceneBehavior&& other ) noexcept = delete;
private:
	std::string m_NextLevel;
	TriggerManager m_TriggerManager;
	bool m_IsLastLevel;
};
