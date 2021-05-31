#pragma once
#include "IComponent.h"
#include "SpriteSheet.h"

namespace dae
{
	class TransformComponent;
}

class CubeGrid : public dae::IComponent
{
	enum class CubeState : int
	{
		Default,
		Marked,
		Done,
		CubeState_Size
	};

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

public:
	CubeGrid( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject );

	void Update( const dae::UpdateInfo& ) override;
	void Draw( dae::Renderer& ) override;
	void Init( const dae::InitInfo& initInfo ) override;

	~CubeGrid( ) override = default;
	CubeGrid( const CubeGrid& other ) = delete;
	CubeGrid( CubeGrid&& other ) noexcept = delete;
	CubeGrid& operator=( const CubeGrid& other ) = delete;
	CubeGrid& operator=( CubeGrid&& other ) noexcept = delete;
private:
	dae::SpriteSheet m_SpriteSheet;
	dae::TransformComponent* m_pTransform;
	std::vector<Cube> m_Cubes;
	int m_Rows;
	glm::vec2 m_CubeSize;

	int GetCubeIdx( const Cube& cube );
	static int GetColumnCount( int row );
	static int GetCubeCount( int rows );
	int RowColToIdx( int c, int r ) const;
	glm::vec2 CalculateOffset( int c, int r ) const;
};
