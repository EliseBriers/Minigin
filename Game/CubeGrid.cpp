#include "MiniginPCH.h"
#include "CubeGrid.h"
#include "JsonObjectWrapper.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Logger.h"

using namespace dae;

CubeGrid::CubeGrid( GameObject& gameObject, const JsonObjectWrapper& jsonObject )
	: IComponent{ gameObject }
	, m_SpriteSheet{ jsonObject.GetObjectWrapper( "sprite_sheet" ) }
	, m_pTransform{ nullptr }
	, m_Rows{ 7 }
{
	m_Cubes.reserve( GetCubeCount( m_Rows ) );
}

void CubeGrid::Update( const UpdateInfo& )
{
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

	const glm::vec2 cubePivot{ 0.5, 0.25 };
	for( const Cube& cube : m_Cubes )
	{
		m_SpriteSheet.Draw( renderer, cube.Offset * scale + pos, cubePivot, scale, GetCubeIdx( cube ) );
	}
}

void CubeGrid::Init( const InitInfo& initInfo )
{
	m_SpriteSheet.Init( initInfo );
	m_CubeSize = { m_SpriteSheet.GetSpriteSource( 0 ).w, m_SpriteSheet.GetSpriteSource( 0 ).h };
	m_pTransform = m_GameObject.get( ).GetComponent<TransformComponent>( );

	for( int y{ }; y < m_Rows; ++y )
	{
		for( int x{ }; x < GetColumnCount( y ); ++x )
		{
			const int idx{ RowColToIdx( x, y ) };
			// Logger::LogInfo( "y: " + std::to_string( y ) + " x: " + std::to_string( x ) + " i: " + std::to_string( idx ) );

			// Calculate connections
			const int connectionUp{ RowColToIdx( x, y - 1 ) };
			const int connectionDown{ RowColToIdx( x, y + 1 ) };
			const int connectionLeft{ RowColToIdx( x - 1, y - 1 ) };
			const int connectionRight{ RowColToIdx( x + 1, y + 1 ) };

			// Calculate offset


			Cube c{ CalculateOffset( x, y ), CubeState::Default, CubeColor::Orange, connectionUp, connectionDown, connectionRight, connectionLeft };
			m_Cubes.push_back( c );
		}
	}
}

const CubeGrid::Cube& CubeGrid::GetCube( size_t idx ) const
{
	return m_Cubes[idx];
}

glm::vec2 CubeGrid::GetCubePos( size_t idx ) const
{
	const Cube& cube{ GetCube( idx ) };
	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const float scale{ m_pTransform->GetScale( ) };
	return cube.Offset * scale + pos;
}

size_t CubeGrid::GetCubeCount( ) const
{
	return m_Cubes.size( );
}

int CubeGrid::GetCubeIdx( const Cube& cube )
{
	const size_t offset{ 0u };
	const size_t stateCount{ static_cast<size_t>(CubeState::CubeState_Size) };

	return static_cast<int>(cube.State) + static_cast<int>(cube.Color) * stateCount + offset;
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
	const float y{ 0.75f * m_CubeSize.y * r };
	const float xOffset{ GetColumnCount( r ) * 0.5f - 0.5f };
	const float x{ ( c - xOffset ) * m_CubeSize.x };
	const glm::vec2 offset{ x, y };
	return offset * 0.95f;
}
