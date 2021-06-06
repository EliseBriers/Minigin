#include "GamePCH.h"
#include "GridHopper.h"
#include "InitInfo.h"
#include "Logger.h"
#include "JsonObjectWrapper.h"
#include "UpdateInfo.h"
#include <glm/detail/func_geometric.inl>
#include "QbertSceneBehavior.h"

GridHopper::GridHopper( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_StompBehavior{ GetStompBehavior( jsonObject.GetString( "stomp_behavior" ) ) }
	, m_Callback{ VoidTouchdown }
	, m_UpDirection{ normalize( jsonObject.GetVec2( "up_direction" ) ) }
	, m_Offset{ jsonObject.GetVec2( "offset" ) }
	, m_pCubeGrid{ nullptr }
	, m_pSprite{ nullptr }
	, m_pTransform{ nullptr }
	, m_SpawnIndex{ jsonObject.GetUint( "spawn_index" ) }
	, m_CurrentIndex{ static_cast<int>(m_SpawnIndex) }
	, m_Speed{ jsonObject.GetOptionalFloat( "speed", 1.f ) }
	, m_MovementPercentage{ 0.f }
	, m_JumpHeight{ jsonObject.GetOptionalFloat( "jump_height", 15.f ) }
	, m_InitializedBehavior{ false }
	, m_State{ HopperState::Idle }
	, m_pSceneBehavior{ nullptr }
{
}

void GridHopper::Init( const dae::InitInfo& initInfo )
{
	m_pSceneBehavior = initInfo.Scene_GetSceneBehaviorAs<QbertSceneBehavior>( );
	if( !m_pSceneBehavior )
	{
		dae::Logger::LogWarning( "GridHopper::Init > No QbertSceneBehavior found" );
		return;
	}

	dae::GameObject* pGameObject{ initInfo.Scene_GetGameObject( "CubeGrid" ) };
	if( !pGameObject )
	{
		dae::Logger::LogWarning( "GridHopper::Init > no GameObject \"CubeGrid\" found" );
		return;
	}
	m_pCubeGrid = pGameObject->GetComponent<CubeGrid>( );
	if( !m_pCubeGrid )
	{
		dae::Logger::LogWarning( "GridHopper::Init > GameObject \"CubeGrid\" has no CubeGrid component" );
		return;
	}
	m_pTransform = m_GameObject.get( ).GetComponent<dae::TransformComponent>( );
	if( !m_pTransform )
	{
		dae::Logger::LogWarning( "GridHopper::Init > m_pTransform is nullptr" );
		return;
	}
	m_pSprite = m_GameObject.get( ).GetComponent<HopperSpriteComponent>( );
	if( !m_pSprite )
	{
		dae::Logger::LogWarning( "GridHopper::Init > m_pSprite is nullptr" );
		return;
	}

	const glm::vec2 spawnPos{ m_pCubeGrid->GetCubePos( m_CurrentIndex, m_Offset ) };
	m_pTransform->SetPosition( spawnPos.x, spawnPos.y, 0.f );

	m_InitializedBehavior = true;

	m_State.AddObserver( [this ]( HopperState state )
	{
		switch( state )
		{
		case HopperState::Hopping:
			break;
		case HopperState::Idle:
			m_Callback( m_CurrentIndex == -1 ? TouchdownType::Void : TouchdownType::Block );
			break;
		case HopperState::OutOfGrid:
			break;
		case HopperState::NoControl:
			break;
		}
	} );
}

void GridHopper::OnHopComplete( )
{
	m_MovementPercentage = 0.f;
	m_State.Set( HopperState::Idle );

	if( m_CurrentIndex == -1 )
		return;

	const CubeGrid::CubeState state{ m_pCubeGrid->GetCubeState( m_CurrentIndex ) };
	switch( m_StompBehavior )
	{
	case StompBehavior::Complete:
		if( state != CubeGrid::CubeState::Done )
			m_pSceneBehavior->AddPoints( 25u );
		m_pCubeGrid->SetCubeState( m_CurrentIndex, CubeGrid::CubeState::Done );
		break;
	case StompBehavior::Advance:
		if( state != CubeGrid::CubeState::Done )
			m_pSceneBehavior->AddPoints( 25u );
		if( state == CubeGrid::CubeState::Default )
			m_pCubeGrid->SetCubeState( m_CurrentIndex, CubeGrid::CubeState::Marked );
		else
			m_pCubeGrid->SetCubeState( m_CurrentIndex, CubeGrid::CubeState::Done );
		break;
	case StompBehavior::UndoOne:
		if( state == CubeGrid::CubeState::Done )
			m_pCubeGrid->SetCubeState( m_CurrentIndex, CubeGrid::CubeState::Marked );
		break;
	case StompBehavior::UndoAll:
		if( state == CubeGrid::CubeState::Done )
			m_pCubeGrid->SetCubeState( m_CurrentIndex, CubeGrid::CubeState::Marked );
		else
			m_pCubeGrid->SetCubeState( m_CurrentIndex, CubeGrid::CubeState::Default );
		break;
	case StompBehavior::Toggle:
		if( state != CubeGrid::CubeState::Done )
			m_pSceneBehavior->AddPoints( 25u );
		if( state == CubeGrid::CubeState::Done )
			m_pCubeGrid->SetCubeState( m_CurrentIndex, CubeGrid::CubeState::Default );
		else
			m_pCubeGrid->SetCubeState( m_CurrentIndex, CubeGrid::CubeState::Done );
		break;
	case StompBehavior::Nothing:
		break;
	}
}

void GridHopper::Update( const dae::UpdateInfo& updateInfo )
{
	if( !m_InitializedBehavior )
		return;
	if( !m_State.Equals( HopperState::Hopping ) )
		return;
	const float dt{ updateInfo.GetDeltaTime( ) };
	m_MovementPercentage += dt * m_Speed;
	if( m_MovementPercentage > 1.f )
	{
		OnHopComplete( );
		return;
	}

	const float scale{ m_pTransform->GetScale( ) };
	const float height{ scale * m_JumpHeight * sinf( m_MovementPercentage * static_cast<float>(M_PI) ) };
	const glm::vec2 jumpOffset{ height * m_UpDirection };
	const glm::vec2 toTarget{ m_ToPos - m_FromPos };
	const glm::vec2 newPos{ m_FromPos + toTarget * m_MovementPercentage + jumpOffset + m_Offset };
	m_pTransform->SetPosition( newPos.x, newPos.y, 0.f );
}

void GridHopper::Hop( MoveDirection direction )
{
	if( !CanHop( ) )
	{
		dae::Logger::LogWarning( "GridHopper::Hop > Attempting to hop while not idle" );
		return;
	}

	SetSpriteDirection( direction );

	const int cubeCount{ static_cast<int>(m_pCubeGrid->GetCubeCount( )) };
	if( m_CurrentIndex == -1 || m_CurrentIndex >= cubeCount )
	{
		dae::Logger::LogWarning( "GridHopper::Hop > Invalid current index" );
		m_State.Set( HopperState::OutOfGrid );
		return;
	}
	// const CubeGrid::Cube& cube{ m_pCubeGrid->GetCube( static_cast<size_t>(m_CurrentIndex) ) };
	const int desiredIndex{ GetIndexAfterHop( direction ) };
	if( desiredIndex == -1 )
	{
		m_State.Set( HopperState::Hopping );
		m_FromPos = m_pCubeGrid->GetCubePos( m_CurrentIndex, m_Offset );
		m_ToPos = m_pCubeGrid->CalculateImaginaryBlockPos( m_CurrentIndex, direction, m_Offset );
		// dae::Logger::LogInfo( "GridHopper::Hop > Out of grid" );
		m_CurrentIndex = -1;
		return;
	}

	// dae::Logger::LogInfo( "GridHopper::Hop > Hopping from index " + std::to_string( m_CurrentIndex ) + " to index " + std::to_string( desiredIndex ) );
	m_FromPos = m_pCubeGrid->GetCubePos( m_CurrentIndex, m_Offset );
	m_ToPos = m_pCubeGrid->GetCubePos( desiredIndex, m_Offset );
	m_CurrentIndex = desiredIndex;

	m_State.Set( HopperState::Hopping );
}

void GridHopper::SafeHop( MoveDirection desired, MoveDirection secondOption )
{
	if( m_CurrentIndex == -1 )
		return;
	if( m_pCubeGrid->GetIndexAfterMove( m_CurrentIndex, desired ) != -1 )
		Hop( desired );
	else
		Hop( secondOption );
}

bool GridHopper::CanHop( ) const
{
	return m_State.Equals( HopperState::Idle );
}

bool GridHopper::IsHopping( ) const
{
	return m_State.Equals( HopperState::Hopping );
}

void GridHopper::Teleport( size_t index )
{
	if( index >= m_pCubeGrid->GetCubeCount( ) )
	{
		dae::Logger::LogWarning( "GridHopper::Teleport > invalid index" );
		return;
	}

	m_CurrentIndex = static_cast<int>(index);
	const glm::vec2 pos{ m_pCubeGrid->GetCubePos( index, m_Offset ) };
	m_pTransform->SetPosition( pos.x, pos.y, 0.f );
	m_State.Set( HopperState::Idle );
}

void GridHopper::SetTouchdownCallback( const touchdown_callback_t& onTouchdown )
{
	m_Callback = onTouchdown;
}

const glm::vec2& GridHopper::GetUpDirection( ) const
{
	return m_UpDirection;
}

int GridHopper::GetCurrentIndex( ) const
{
	return m_CurrentIndex;
}

CubeGrid* GridHopper::GetCubeGrid( ) const
{
	return m_pCubeGrid;
}

const glm::vec2& GridHopper::GetOffset( ) const
{
	return m_Offset;
}

void GridHopper::SetState( HopperState state )
{
	m_State.Set( state );
}

void GridHopper::ResetToSpawnIndex( )
{
	m_CurrentIndex = static_cast<int>(m_SpawnIndex);
}

int GridHopper::GetIndexAfterHop( MoveDirection direction ) const
{
	return m_pCubeGrid->GetIndexAfterMove( static_cast<size_t>(m_CurrentIndex), direction );
}

void GridHopper::HopToIndex( size_t index )
{
	const glm::vec2 pos{ m_pTransform->GetPosition( ) };

	m_FromPos = pos;
	m_ToPos = m_pCubeGrid->GetCubePos( index, m_Offset );
	m_CurrentIndex = static_cast<int>(index);

	m_State.Set( HopperState::Hopping );
}

void GridHopper::VoidTouchdown( TouchdownType )
{
}

GridHopper::StompBehavior GridHopper::GetStompBehavior( const std::string& str )
{
	if( str == "complete" )
		return StompBehavior::Complete;
	if( str == "advance" )
		return StompBehavior::Advance;
	if( str == "undo_one" )
		return StompBehavior::UndoOne;
	if( str == "undo_all" )
		return StompBehavior::UndoAll;
	if( str == "toggle" )
		return StompBehavior::Toggle;
	if( str == "nothing" )
		return StompBehavior::Nothing;

	dae::Logger::LogWarning( "GridHopper::GetStompBehavior > invalid string provided" );
	return StompBehavior::Complete;
}

void GridHopper::SetSpriteDirection( MoveDirection direction ) const
{
	switch( direction )
	{
	case MoveDirection::UpLeft:
		m_pSprite->SetDirection( SpriteDirection::Left );
		break;
	case MoveDirection::DownRight:
	case MoveDirection::Right:
		m_pSprite->SetDirection( SpriteDirection::Right );
		break;
	case MoveDirection::UpRight:
		m_pSprite->SetDirection( SpriteDirection::Up );
		break;
	case MoveDirection::Left:
	case MoveDirection::DownLeft:
		m_pSprite->SetDirection( SpriteDirection::Down );
		break;
	default: ;
	}
}
