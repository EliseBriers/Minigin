#include "GamePCH.h"
#include "CoilyDirectionController.h"
#include "QbertSceneBehavior.h"

CoilyDirectionController::CoilyDirectionController( dae::TransformComponent* pTransform, QbertSceneBehavior* pSceneBehavior, GridHopper* pGridHopper )
	: m_pPlayer{ nullptr }
	, m_pTransform{ pTransform }
	, m_pSceneBehavior{ pSceneBehavior }
	, m_pGridHopper{ pGridHopper }
	, m_SafeHop{ }
{
}

MoveDirection CoilyDirectionController::GetMoveDirection( )
{
	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	m_pPlayer = m_pSceneBehavior->GetClosestPlayer( pos );

	m_SafeHop = true;
	// If we're on the player's last index, mimic the player's movement instead of going towards the player
	if( m_pGridHopper->GetCurrentIndex( ) == m_pPlayer->GetLastIndex( ) )
	{
		m_SafeHop = false;
		return m_pPlayer->GetLastMoveDirection( );
	}


	const glm::vec2 toPlayer{ m_pPlayer->GetPosition( ) - pos };

	if( toPlayer.x > 0.f )
	{
		// Move Right
		if( toPlayer.y > 0 )
		{
			// Move Down
			return MoveDirection::DownRight;
		}
		// Move Up
		return MoveDirection::UpRight;
	}
	// Move Left
	if( toPlayer.y > 0 )
	{
		// Move Down
		return MoveDirection::DownLeft;
	}
	// Move Up
	return MoveDirection::UpLeft;
}

bool CoilyDirectionController::GetSafeHop( ) const
{
	return m_SafeHop;
}
