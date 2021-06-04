#include "MiniginPCH.h"
#include "SpriteSheet.h"
#include "JsonObjectWrapper.h"
#include "InitInfo.h"
#include "Logger.h"

dae::SpriteSheet::SpriteSheet( const JsonObjectWrapper& jsonObject )
	: m_FileName{ jsonObject.GetString( "texture_file_name" ) }
	, m_SpriteSources{ jsonObject.GetVector<Rect2D>( "rects", ObjectToRect ) }
	, m_pTexture{ nullptr }
{
}

void dae::SpriteSheet::Init( const InitInfo& initInfo )
{
	m_pTexture = &initInfo.Resource_GetTexture( m_FileName );
}

void dae::SpriteSheet::Draw( Renderer& renderer, const glm::vec2& pos, const glm::vec2& pivot, float scale, size_t spriteIdx ) const
{
	if( !m_pTexture )
	{
		Logger::LogError( "dae::SpriteSheet::Draw > Failed to draw: m_pTexture is nullptr" );
		return;
	}
	if( spriteIdx >= m_SpriteSources.size( ) )
	{
		Logger::LogError( "dae::SpriteSheet::Draw > Failed to draw: spriteIdx is invalid" );
		return;
	}

	renderer.RenderTexture( *m_pTexture, pos, pivot, scale, m_SpriteSources[spriteIdx] );
}

const dae::Rect2D& dae::SpriteSheet::GetSpriteSource( size_t idx ) const
{
	return m_SpriteSources[idx];
}

dae::Rect2D dae::SpriteSheet::ObjectToRect( const JsonObjectWrapper& jsonObject )
{
	const float x{ jsonObject.GetFloat( "x" ) };
	const float y{ jsonObject.GetFloat( "y" ) };
	const float w{ jsonObject.GetFloat( "w" ) };
	const float h{ jsonObject.GetFloat( "h" ) };

	return { x, y, w, h };
}
