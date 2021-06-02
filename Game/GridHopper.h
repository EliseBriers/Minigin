#pragma once
#include <IComponent.h>
#include "CubeGrid.h"
#include "Enums.h"

class GridHopper : public dae::IComponent
{
public:
	enum class TouchdownType
	{
		Block,
		Void
	};

	using touchdown_callback_t = std::function<void( TouchdownType )>;

	enum class State
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
		UndoAll
	};

	GridHopper( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );
	void Init( const dae::InitInfo& ) override;
	void OnHopComplete( );
	void Update( const dae::UpdateInfo& ) override;
	void Hop( MoveDirection direction );
	bool CanHop( ) const;
	bool IsHopping( ) const;
	void Teleport( size_t index );

	// Rule of 5
	~GridHopper( ) override = default;
	void SetTouchdownCallback( const touchdown_callback_t& onTouchdown );
	GridHopper( const GridHopper& other ) = delete;
	GridHopper( GridHopper&& other ) noexcept = delete;
	GridHopper& operator=( const GridHopper& other ) = delete;
	GridHopper& operator=( GridHopper&& other ) noexcept = delete;
private:
	StompBehavior m_StompBehavior;
	touchdown_callback_t m_Callback;
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
	static void VoidTouchdown( TouchdownType );
	static StompBehavior GetStompBehavior( const std::string& str );
};
