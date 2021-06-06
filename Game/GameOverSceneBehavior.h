#pragma once
#include <SceneBehavior.h>

class GameOverSceneBehavior final : public dae::SceneBehavior
{
public:
	GameOverSceneBehavior( size_t& score );

	size_t GetScore( ) const;

	~GameOverSceneBehavior( ) override = default;
	GameOverSceneBehavior( const GameOverSceneBehavior& other ) = delete;
	GameOverSceneBehavior( GameOverSceneBehavior&& other ) noexcept = delete;
	GameOverSceneBehavior& operator=( const GameOverSceneBehavior& other ) = delete;
	GameOverSceneBehavior& operator=( GameOverSceneBehavior&& other ) noexcept = delete;
private:
	size_t& m_Score;
};
