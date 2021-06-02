#include "GamePCH.h"
#include "GridHopper.h"
#include "InitInfo.h"
#include "Logger.h"
#include "JsonObjectWrapper.h"
#include "UpdateInfo.h"

GridHopper::GridHopper( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_Callback{ VoidTouchdown }
	, m_pCubeGrid{ nullptr }
	, m_pTransform{ nullptr }
	, m_CurrentIndex{ 0u }
	, m_Speed{ jsonObject.GetOptionalFloat( "speed", 1.f ) }
	, m_JumpHeight{ jsonObject.GetOptionalFloat( "jump_height", 15.f ) }
	, m_InitializedBehavior{ false }
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

	m_InitializedBehavior = true;
}

void GridHopper::Update( const dae::UpdateInfo& updateInfo )
{
	if( !m_InitializedBehavior )
		return;
	if( m_State != State::Hopping )
		return;
	const float dt{ updateInfo.GetDeltaTime( ) };
	m_MovementPercentage += dt * m_Speed;
	if( m_MovementPercentage > 1.f )
	{
		m_MovementPercentage = 0.f;
		m_State = State::Idle;
		m_Callback( m_CurrentIndex == -1 ? TouchdownType::Void : TouchdownType::Block );
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
		m_State = State::Hopping;
		m_FromPos = m_pCubeGrid->GetCubePos( m_CurrentIndex );
		m_ToPos = m_pCubeGrid->CalculateImaginaryBlockPos( m_CurrentIndex, direction );
		// dae::Logger::LogInfo( "GridHopper::Hop > Out of grid" );
		m_CurrentIndex = -1;
		return;
	}

	// dae::Logger::LogInfo( "GridHopper::Hop > Hopping from index " + std::to_string( m_CurrentIndex ) + " to index " + std::to_string( desiredIndex ) );
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

void GridHopper::Teleport( size_t index )
{
	if( index >= m_pCubeGrid->GetCubeCount( ) )
	{
		dae::Logger::LogWarning( "GridHopper::Teleport > invalid index" );
		return;
	}

	m_CurrentIndex = static_cast<int>(index);
	const glm::vec2 pos{ m_pCubeGrid->GetCubePos( index ) };
	m_pTransform->SetPosition( pos.x, pos.y, 0.f );
	m_State = State::Idle;
}

void GridHopper::SetTouchdownCallback( const touchdown_callback_t& onTouchdown )
{
	m_Callback = onTouchdown;
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

void GridHopper::VoidTouchdown( TouchdownType )
{
}
