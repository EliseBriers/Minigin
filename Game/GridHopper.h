#pragma once
#include <IComponent.h>
#include "CubeGrid.h"
#include "Enums.h"

class GridHopper final : public dae::IComponent
{
public:
	enum class TouchdownType
	{
		Block,
		Void
	};

	using touchdown_callback_t = std::function<void( TouchdownType )>;

	enum class PlayerState
	{
		Hopping,
		Idle,
		OutOfGrid
	};

	enum class StompBehavior
	{
		Complete,
		Advance,
		UndoOne,
		UndoAll,
		Toggle
	};

	GridHopper( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );
	void Init( const dae::InitInfo& ) override;
	void OnHopComplete( );
	void Update( const dae::UpdateInfo& ) override;
	void Hop( MoveDirection direction );
	void HopToIndex( size_t index );
	bool CanHop( ) const;
	bool IsHopping( ) const;
	void Teleport( size_t index );
	void SetTouchdownCallback( const touchdown_callback_t& onTouchdown );

	// Rule of 5
	~GridHopper( ) override = default;
	GridHopper( const GridHopper& other ) = delete;
	GridHopper( GridHopper&& other ) noexcept = delete;
	GridHopper& operator=( const GridHopper& other ) = delete;
	GridHopper& operator=( GridHopper&& other ) noexcept = delete;
private:
	StompBehavior m_StompBehavior;
	touchdown_callback_t m_Callback;
	glm::vec2 m_FromPos;
	glm::vec2 m_ToPos;
	glm::vec2 m_JumpDirection;
	glm::vec2 m_Offset;
	CubeGrid* m_pCubeGrid;
	dae::TransformComponent* m_pTransform;
	int m_CurrentIndex;
	float m_Speed;
	float m_MovementPercentage;
	float m_JumpHeight;
	bool m_InitializedBehavior;
	PlayerState m_State;

	static int GetToIndex( const CubeGrid::Cube& cube, MoveDirection direction );
	static void VoidTouchdown( TouchdownType );
	static StompBehavior GetStompBehavior( const std::string& str );
};
