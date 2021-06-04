#pragma once
#include <IComponent.h>
#include "CubeGrid.h"
#include "Enums.h"
#include "HopperSpriteComponent.h"
#include "ObservableVariable.h"

class GridHopper final : public dae::IComponent
{
public:
	enum class TouchdownType
	{
		Block,
		Void
	};

	using touchdown_callback_t = std::function<void( TouchdownType )>;

	enum class HopperState
	{
		Hopping,
		Idle,
		OutOfGrid,
		NoControl
	};

	enum class StompBehavior
	{
		Complete,
		Advance,
		UndoOne,
		UndoAll,
		Toggle,
		Nothing
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
	const glm::vec2& GetUpDirection( ) const;
	int GetCurrentIndex( ) const;
	CubeGrid* GetCubeGrid( ) const;
	const glm::vec2& GetOffset( ) const;
	void SetState( HopperState state );
	void ResetToSpawnIndex( );

	// Rule of 5
	~GridHopper( ) override = default;
	GridHopper( const GridHopper& other ) = delete;
	GridHopper( GridHopper&& other ) noexcept = delete;
	GridHopper& operator=( const GridHopper& other ) = delete;
	GridHopper& operator=( GridHopper&& other ) noexcept = delete;
private:
	const StompBehavior m_StompBehavior;
	touchdown_callback_t m_Callback;
	glm::vec2 m_FromPos;
	glm::vec2 m_ToPos;
	const glm::vec2 m_UpDirection;
	const glm::vec2 m_Offset;
	CubeGrid* m_pCubeGrid;
	HopperSpriteComponent* m_pSprite;
	dae::TransformComponent* m_pTransform;
	const size_t m_SpawnIndex;
	int m_CurrentIndex;
	float m_Speed;
	float m_MovementPercentage;
	float m_JumpHeight;
	bool m_InitializedBehavior;
	dae::ObservableVariable<HopperState> m_State;
	QbertSceneBehavior* m_pSceneBehavior;

	static void VoidTouchdown( TouchdownType );
	static StompBehavior GetStompBehavior( const std::string& str );
	void SetSpriteDirection( MoveDirection direction ) const;
};
