#pragma once
#include <IComponent.h>
#include "CubeGrid.h"
#include "Enums.h"

class GridHopper : public dae::IComponent
{
public:
	enum class State
	{
		Hopping,
		Idle,
		OutOfGrid
	};

	GridHopper(dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name);
	void Init( const dae::InitInfo& ) override;
	void Update( const dae::UpdateInfo& ) override;
	void Hop( MoveDirection direction );
	bool CanHop( ) const;
	bool IsHopping( ) const;
	// Rule of 5
	
	~GridHopper( ) override = default;
	GridHopper( const GridHopper& other ) = delete;
	GridHopper( GridHopper&& other ) noexcept = delete;
	GridHopper& operator=( const GridHopper& other ) = delete;
	GridHopper& operator=( GridHopper&& other ) noexcept = delete;
private:
	glm::vec2 m_FromPos;
	glm::vec2 m_ToPos;
	CubeGrid* m_pCubeGrid;
	dae::TransformComponent* m_pTransform;
	int m_CurrentIndex;
	float m_Speed;
	float m_MovementPercentage;
	float m_JumpHeight;
	bool m_InitializedBehavior;
	State m_State;

	static int GetToIndex( const CubeGrid::Cube& cube, MoveDirection direction );
};
