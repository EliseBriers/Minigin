#pragma once
#include "IComponent.h"
#include "SpriteSheet.h"
#include "Enums.h"

namespace dae
{
	class TransformComponent;
	class TimerComponent;
}
class QbertSceneBehavior;

class CubeGrid : public dae::IComponent
{
	enum class CubeColor : int
	{
		Teal,
		Orange,
		Gray,
		Blue,
		Black,
		LimeOrange,
		LimeBlue,
		Green,
		Void,
		CubeColor_Size
	};

public:
	enum class CubeState : int
	{
		Default,
		Marked,
		Done,
		CubeState_Size
	};

	struct Cube
	{
		glm::vec2 Offset;
		CubeState State;
		CubeColor Color;
		// Connections, -1 = no connection
		int ConnectionUp;
		int ConnectionDown;
		int ConnectionRight;
		int ConnectionLeft;
	};

	CubeGrid( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );

	void Update( const dae::UpdateInfo& ) override;
	void Draw( dae::Renderer& ) override;
	void Init( const dae::InitInfo& initInfo ) override;

	const Cube& GetCube( size_t idx ) const;
	glm::vec2 GetCubePos( size_t idx ) const;
	glm::vec2 CalculateImaginaryBlockPos( size_t idx, MoveDirection direction ) const;
	size_t GetCubeCount( ) const;

	void SetCubeState( size_t idx, CubeState cubeState );
	CubeState GetCubeState( size_t idx ) const;
	void DoAnimationSwap( );
	void EndAnimation( );

	~CubeGrid( ) override = default;
	CubeGrid( const CubeGrid& other ) = delete;
	CubeGrid( CubeGrid&& other ) noexcept = delete;
	CubeGrid& operator=( const CubeGrid& other ) = delete;
	CubeGrid& operator=( CubeGrid&& other ) noexcept = delete;
private:
	QbertSceneBehavior* m_pSceneBehavior;
	dae::TimerComponent* m_pAnimationFlipTimer;
	dae::TimerComponent* m_pAnimationTimer;
	CubeState m_EndAnimationState;
	dae::SpriteSheet m_SpriteSheet;
	dae::TransformComponent* m_pTransform;
	std::vector<Cube> m_Cubes;
	int m_Rows;
	glm::vec2 m_CubeSize;
	bool m_GameCompleted;

	static size_t GetSpriteIdx( const Cube& cube );
	static size_t GetSpriteIdx( CubeState state, CubeColor color );
	static int GetColumnCount( int row );
	static int GetCubeCount( int rows );
	int RowColToIdx( int c, int r ) const;
	glm::vec2 CalculateOffset( int c, int r ) const;
	void CheckGameComplete( );
};
