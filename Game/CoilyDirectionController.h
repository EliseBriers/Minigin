#pragma once
#include "IDirectionController.h"
#include "QbertPlayer.h"

class CoilyDirectionController final : public IDirectionController
{
public:
	CoilyDirectionController( dae::TransformComponent* pTransform, QbertSceneBehavior* pSceneBehavior, GridHopper* pGridHopper );

	MoveDirection GetMoveDirection( ) override;
	bool GetSafeHop( ) const;

	// Rule of 5
	~CoilyDirectionController( ) override = default;
	CoilyDirectionController( const CoilyDirectionController& other ) = delete;
	CoilyDirectionController( CoilyDirectionController&& other ) noexcept = delete;
	CoilyDirectionController& operator=( const CoilyDirectionController& other ) = delete;
	CoilyDirectionController& operator=( CoilyDirectionController&& other ) noexcept = delete;
private:
	QbertPlayer* m_pPlayer;
	dae::TransformComponent* m_pTransform;
	QbertSceneBehavior* m_pSceneBehavior;
	GridHopper* m_pGridHopper;
	bool m_SafeHop;
};
