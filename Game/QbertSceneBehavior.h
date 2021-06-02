#pragma once
#include <SceneBehavior.h>
#include "TriggerManager.h"

class QbertSceneBehavior : public dae::SceneBehavior
{
public:
	QbertSceneBehavior( ) = default;

	virtual void Update( const dae::UpdateInfo& ) override;
	void RegisterOverlapDetector( SphereOverlapDetector& overlapDetector );
	
	// Rule of 5
	~QbertSceneBehavior( ) override = default;
	QbertSceneBehavior( const QbertSceneBehavior& other ) = delete;
	QbertSceneBehavior( QbertSceneBehavior&& other ) noexcept = delete;
	QbertSceneBehavior& operator=( const QbertSceneBehavior& other ) = delete;
	QbertSceneBehavior& operator=( QbertSceneBehavior&& other ) noexcept = delete;
private:
	TriggerManager m_TriggerManager;
};
