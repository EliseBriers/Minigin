#include "MiniginPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

dae::ResourceManager::ResourceManager( )
	: m_Textures{ TextureAllocator }
	, m_Fonts{ FontAllocator }
	, m_TextTextures{ TextAllocator }
{
}

void dae::ResourceManager::Init( const std::string& dataPath )
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if( ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) != IMG_INIT_PNG )
	{
		throw std::runtime_error( std::string( "Failed to load support for png's: " ) + SDL_GetError( ) );
	}

	if( ( IMG_Init( IMG_INIT_JPG ) & IMG_INIT_JPG ) != IMG_INIT_JPG )
	{
		throw std::runtime_error( std::string( "Failed to load support for jpg's: " ) + SDL_GetError( ) );
	}

	if( TTF_Init( ) != 0 )
	{
		throw std::runtime_error( std::string( "Failed to load support for fonts: " ) + SDL_GetError( ) );
	}
}

const dae::Texture2D& dae::ResourceManager::GetTexture( const std::string& file, Renderer& renderer )
{
	return m_Textures.GetElement( file, m_DataPath + file, renderer );
}

dae::Font& dae::ResourceManager::LoadFont( const std::string& file, unsigned int size )
{
	return m_Fonts.GetElement( file + ":_:" + std::to_string( size ), m_DataPath + file, size );
}

const dae::Texture2D& dae::ResourceManager::LoadText( const std::string& text, const std::string& fileName, uint32_t size, Renderer& renderer )
{
	const Font& font{ LoadFont( fileName, size ) };

	std::string elementName{ fileName + ":_:" + text + std::to_string( size ) };

	return m_TextTextures.GetElement( elementName, text, renderer, font );
}

std::unique_ptr<dae::Texture2D> dae::ResourceManager::TextureAllocator( const std::string& file, Renderer& renderer )
{
	auto texture = IMG_LoadTexture( renderer.GetSDLRenderer( ), file.c_str( ) );
	if( texture == nullptr )
	{
		throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError( ) );
	}
	return std::make_unique<Texture2D>( texture );
}

std::unique_ptr<dae::Font> dae::ResourceManager::FontAllocator( const std::string& file, uint32_t size )
{
	return std::make_unique<Font>( file, size );
}

std::unique_ptr<dae::Texture2D> dae::ResourceManager::TextAllocator( const std::string& text, Renderer& renderer, const Font& font )
{
	const SDL_Color color = { 255, 255, 255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended( font.GetFont( ), text.c_str( ), color );
	if( !surf )
		throw std::runtime_error{ std::string{ "Render text failed: " } + SDL_GetError( ) };

	auto texture = SDL_CreateTextureFromSurface( renderer.GetSDLRenderer( ), surf );

	if( !texture )
		throw std::runtime_error{ std::string{ "Create text texture from surface failed: " } + SDL_GetError( ) };

	SDL_FreeSurface( surf );
	return std::make_unique<Texture2D>( texture );
}
