#include "GamePCH.h"
#include "GameOverSceneBehavior.h"

GameOverSceneBehavior::GameOverSceneBehavior( size_t& score )
	: m_Score{ score }
{
}

size_t GameOverSceneBehavior::GetScore( ) const
{
	return m_Score;
}
