#include "GamePCH.h"
#include "CubeGrid.h"
#include "JsonObjectWrapper.h"
#include "Logger.h"
#include "TimerComponent.h"
#include "QbertSceneBehavior.h"
#include <InitInfo.h>
#include "EnumHelpers.h"

using namespace dae;

CubeGrid::CubeGrid( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name )
	: BaseComponent{ gameObject, std::move( name ) }
	, m_Color{ EnumHelpers::StringToLevelColor( jsonObject.GetString( "color" ) ) }
	, m_pSceneBehavior{ nullptr }
	, m_pAnimationFlipTimer{ nullptr }
	, m_pAnimationTimer{ nullptr }
	, m_EndAnimationState{ CubeState::Default }
	, m_SpriteSheet{ jsonObject.GetObjectWrapper( "sprite_sheet" ) }
	, m_pTransform{ nullptr }
	, m_Rows{ 7 }
	, m_GameCompleted{ false }
{
	m_Cubes.reserve( GetCubeCount( m_Rows ) );
}

void CubeGrid::Update( const UpdateInfo& )
{
	if( m_GameCompleted )
		return;

	CheckGameComplete( );
}

void CubeGrid::Draw( Renderer& renderer )
{
	if( !m_pTransform )
	{
		Logger::LogError( "CubeGrid::Draw > Failed to draw: m_pTransform is nullptr" );
		return;
	}
	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const float scale{ m_pTransform->GetScale( ) };

	const glm::vec2 cubePivot{ 0.5, 0.5 };
	for( const Cube& cube : m_Cubes )
	{
		const CubeState state{ m_GameCompleted ? m_EndAnimationState : cube.PlayerState };
		const size_t spriteIndex{ GetSpriteIdx( state, m_Color ) };
		m_SpriteSheet.Draw( renderer, cube.Offset * scale + pos, cubePivot, scale, spriteIndex );
	}
}

void CubeGrid::Init( const InitInfo& initInfo )
{
	m_SpriteSheet.Init( initInfo );
	m_CubeSize = { m_SpriteSheet.GetSpriteSource( 0 ).w, m_SpriteSheet.GetSpriteSource( 0 ).h };
	m_pTransform = m_GameObject.get( ).GetComponent<TransformComponent>( );
	if( !m_pTransform )
	{
		Logger::LogError( "CubeGrid::Init > m_pTransform was nullptr" );
		return;
	}

	for( int y{ }; y < m_Rows; ++y )
	{
		for( int x{ }; x < GetColumnCount( y ); ++x )
		{
			// const int idx{ RowColToIdx( x, y ) };
			// Logger::LogInfo( "y: " + std::to_string( y ) + " x: " + std::to_string( x ) + " i: " + std::to_string( idx ) );

			// Calculate connections
			const int connectionUpRight{ RowColToIdx( x, y - 1 ) };
			const int connectionDownLeft{ RowColToIdx( x, y + 1 ) };
			const int connectionUpLeft{ RowColToIdx( x - 1, y - 1 ) };
			const int connectionDownRight{ RowColToIdx( x + 1, y + 1 ) };
			const int connectionLeft{ RowColToIdx( x - 1, y ) };
			const int connectionRight{ RowColToIdx( x + 1, y ) };


			Cube c{ CalculateOffset( x, y ), CubeState::Default, connectionUpRight, connectionDownLeft, connectionDownRight, connectionUpLeft, connectionRight, connectionLeft };
			m_Cubes.push_back( c );
		}
	}

	m_pAnimationFlipTimer = GetGameObject( ).GetComponentByName<TimerComponent>( "AnimationFlipTimer" );

	if( !m_pAnimationFlipTimer )
	{
		Logger::LogError( "CubeGrid::Init > m_pAnimationFlipTimer was nullptr" );
		return;
	}

	m_pAnimationFlipTimer->SetCallback( [this]( )
	{
		DoAnimationSwap( );
	} );


	m_pAnimationTimer = GetGameObject( ).GetComponentByName<TimerComponent>( "AnimationTimer" );

	if( !m_pAnimationTimer )
	{
		Logger::LogError( "CubeGrid::Init > m_pAnimationTimer was nullptr" );
		return;
	}

	m_pAnimationTimer->SetCallback( [this]( )
	{
		OnLevelEnd( );
	} );

	m_pSceneBehavior = initInfo.Scene_GetSceneBehaviorAs<QbertSceneBehavior>( );
}

const CubeGrid::Cube& CubeGrid::GetCube( size_t idx ) const
{
	return m_Cubes[idx];
}

glm::vec2 CubeGrid::GetCubePos( size_t idx, const glm::vec2& offset ) const
{
	const Cube& cube{ GetCube( idx ) };
	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const float scale{ m_pTransform->GetScale( ) };
	return cube.Offset * scale + pos + offset * m_CubeSize * scale;
}

size_t CubeGrid::GetCubeCount( ) const
{
	return m_Cubes.size( );
}

void CubeGrid::SetCubeState( size_t idx, CubeState cubeState )
{
	m_Cubes[idx].PlayerState = cubeState;
}

CubeGrid::CubeState CubeGrid::GetCubeState( size_t idx ) const
{
	return m_Cubes[idx].PlayerState;
}

void CubeGrid::DoAnimationSwap( )
{
	if( m_EndAnimationState == CubeState::Done )
		m_EndAnimationState = CubeState::Default;
	else
		m_EndAnimationState = CubeState::Done;
}

void CubeGrid::OnLevelEnd( ) const
{
	m_pSceneBehavior->EndLevel( );
}

int CubeGrid::GetIndexAfterMove( size_t index, MoveDirection direction ) const
{
	const Cube& cube{ m_Cubes[index] };
	switch( direction )
	{
	case MoveDirection::UpLeft:
		return cube.ConnectionUpLeft;
	case MoveDirection::DownRight:
		return cube.ConnectionDownRight;
	case MoveDirection::UpRight:
		return cube.ConnectionUpRight;
	case MoveDirection::DownLeft:
		return cube.ConnectionDownLeft;

	case MoveDirection::Left:
		return cube.ConnectionLeft;
	case MoveDirection::Right:
		return cube.ConnectionRight;
	default:
		Logger::LogWarning( "CubeGrid::GetIndexAfterMove > direction invalid" );
		return -1;
	}
}

size_t CubeGrid::GetSpriteIdx( CubeState state, LevelColor color )
{
	const size_t offset{ 0u };
	const size_t stateCount{ static_cast<size_t>(CubeState::CubeState_Size) };

	return static_cast<size_t>(state) + static_cast<size_t>(color) * stateCount + offset;
}

int CubeGrid::GetColumnCount( int row )
{
	return row + 1;
}

int CubeGrid::GetCubeCount( int rows )
{
	return rows == 0 ? 0 : rows + GetCubeCount( rows - 1 );
}

int CubeGrid::RowColToIdx( int c, int r ) const
{
	if( c < 0 ) return -1;
	if( r < 0 ) return -1;
	if( r >= m_Rows ) return -1;
	if( c > r ) return -1;
	return c + GetCubeCount( r );
}

glm::vec2 CubeGrid::CalculateOffset( int c, int r ) const
{
	const float y{ 0.75f * m_CubeSize.y * static_cast<float>(r) };
	const float xOffset{ static_cast<float>(GetColumnCount( r )) * 0.5f - 0.5f };
	const float x{ ( static_cast<float>(c) - xOffset ) * m_CubeSize.x };
	const glm::vec2 offset{ x, y };
	return offset * 0.95f;
}

void CubeGrid::CheckGameComplete( )
{
	for( const Cube& cube : m_Cubes )
	{
		if( cube.PlayerState != CubeState::Done )
			return;
	}

	// Logger::LogInfo( "Game Completed" );
	m_GameCompleted = true;
	m_pAnimationFlipTimer->Start( );
	m_pAnimationTimer->Start( );
	m_pSceneBehavior->GameCompleted( );
}

glm::vec2 CubeGrid::CalculateImaginaryBlockPos( size_t idx, MoveDirection direction, const glm::vec2& offset ) const
{
	glm::vec2 posOffset{ };

	switch( direction )
	{
	case MoveDirection::UpLeft:
		posOffset = { -0.5f, -0.5f };
		break;
	case MoveDirection::DownRight:
		posOffset = { 0.5f, 0.5f };
		break;
	case MoveDirection::UpRight:
		posOffset = { 0.5f, -0.5f };
		break;
	case MoveDirection::DownLeft:
		posOffset = { - 0.5f, 0.5f };
		break;
	case MoveDirection::Left:
		posOffset = { -1.f, 0.f };
		break;
	case MoveDirection::Right:
		posOffset = { 1.f, 0.f };
		break;
	default:
		Logger::LogWarning( "CubeGrid::CalculateImaginaryBlockPos > invalid direction" );
		return { };
	}
	posOffset *= m_CubeSize;
	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const float scale{ m_pTransform->GetScale( ) };

	return pos + ( posOffset + m_Cubes[idx].Offset ) * scale + offset * m_CubeSize * scale;
}
