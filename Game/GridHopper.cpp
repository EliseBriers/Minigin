#include "GridHopper.h"
#include "InitInfo.h"
#include "Logger.h"
#include "JsonObjectWrapper.h"
#include "UpdateInfo.h"
#include "TransformComponent.h"

GridHopper::GridHopper( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject )
	: IComponent{ gameObject }
	, m_pCubeGrid{ nullptr }
	, m_pTransform{ nullptr }
	, m_CurrentIndex{ 0u }
	, m_Speed{ jsonObject.GetOptionalFloat( "speed", 1.f ) }
	, m_JumpHeight{ jsonObject.GetOptionalFloat( "jump_height", 15.f ) }
	, m_IsInitialized{ false }
	, m_State{ State::Idle }
{
}

void GridHopper::Init( const dae::InitInfo& initInfo )
{
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

	m_IsInitialized = true;
}

void GridHopper::Update( const dae::UpdateInfo& updateInfo )
{
	if( !m_IsInitialized )
		return;
	if( m_State != State::Hopping )
		return;
	const float dt{ updateInfo.GetDeltaTime( ) };
	m_MovementPercentage += dt * m_Speed;
	if( m_MovementPercentage > 1.f )
	{
		m_MovementPercentage = 0.f;
		m_State = State::Idle;
		return;
	}

	const float scale{ m_pTransform->GetScale( ) };
	const float height{ scale * m_JumpHeight * sinf( m_MovementPercentage * static_cast<float>(M_PI) ) };

	const glm::vec2 toTarget{ m_ToPos - m_FromPos };
	const glm::vec2 newPos{ m_FromPos + toTarget * m_MovementPercentage };
	m_pTransform->SetPosition( newPos.x, newPos.y - height, 0.f );
}

void GridHopper::Hop( MoveDirection direction )
{
	if( !CanHop( ) )
	{
		dae::Logger::LogWarning( "GridHopper::Hop > Attempting to hop while not idle" );
		return;
	}


	const int cubeCount{ static_cast<int>(m_pCubeGrid->GetCubeCount( )) };
	if( m_CurrentIndex == -1 || m_CurrentIndex >= cubeCount )
	{
		dae::Logger::LogWarning( "GridHopper::Hop > Invalid current index" );
		m_State = State::OutOfGrid;
		return;
	}
	const CubeGrid::Cube& cube{ m_pCubeGrid->GetCube( static_cast<size_t>(m_CurrentIndex) ) };
	const int desiredIndex{ GetToIndex( cube, direction ) };
	if( desiredIndex == -1 )
	{
		m_State = State::OutOfGrid;
		dae::Logger::LogInfo( "GridHopper::Hop > Out of grid" );
		return;
	}

	dae::Logger::LogInfo( "GridHopper::Hop > Hopping from index " + std::to_string( m_CurrentIndex ) + " to index " + std::to_string( desiredIndex ) );
	m_FromPos = m_pCubeGrid->GetCubePos( m_CurrentIndex );
	m_ToPos = m_pCubeGrid->GetCubePos( desiredIndex );
	m_CurrentIndex = desiredIndex;

	m_State = State::Hopping;
}

bool GridHopper::CanHop( ) const
{
	return m_State == State::Idle;
}

bool GridHopper::IsHopping( ) const
{
	return m_State == State::Hopping;
}

int GridHopper::GetToIndex( const CubeGrid::Cube& cube, MoveDirection direction )
{
	switch( direction )
	{
	case MoveDirection::Left:
		return cube.ConnectionLeft;
	case MoveDirection::Right:
		return cube.ConnectionRight;
	case MoveDirection::Up:
		return cube.ConnectionUp;
	case MoveDirection::Down:
		return cube.ConnectionDown;
	default:
		dae::Logger::LogWarning( "GridHopper::GetToIndex > direction invalid" );
		return -1;
	}
}
