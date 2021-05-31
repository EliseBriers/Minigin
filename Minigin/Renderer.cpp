#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl2.h"

dae::Renderer::Renderer( )
	: m_pSDLRenderer{ nullptr }
	, m_pWindow{ nullptr }
	, m_ShowDemo{ true }
{
}

void dae::Renderer::Init( SDL_Window* window, bool useVSync )
{
	m_pWindow = window;
	Uint32 flags{ SDL_RENDERER_ACCELERATED };
	if( useVSync )
		flags |= SDL_RENDERER_PRESENTVSYNC;
	m_pSDLRenderer = SDL_CreateRenderer( window, GetOpenGLDriverIndex( ), flags );
	if( m_pSDLRenderer == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError( ) );
	}

	IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );
	ImGui_ImplSDL2_InitForOpenGL( window, SDL_GL_GetCurrentContext( ) );
	ImGui_ImplOpenGL2_Init( );
}

void dae::Renderer::Render( SceneManager& sceneManager )
{
	SDL_RenderClear( m_pSDLRenderer );


	ImGui_ImplOpenGL2_NewFrame( );
	ImGui_ImplSDL2_NewFrame( m_pWindow );
	ImGui::NewFrame( );
	sceneManager.Render( *this );
	ImGui::Render( );
	ImGui_ImplOpenGL2_RenderDrawData( ImGui::GetDrawData( ) );

	SDL_RenderPresent( m_pSDLRenderer );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const glm::vec2& pos ) const
{
	RenderTexture( texture, pos.x, pos.y );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const glm::vec2& pos, const glm::vec2& pivot, const glm::vec2& scale ) const
{
	const glm::vec2& dimensions{ texture.GetSize( ) };
	const glm::vec2 offset{ -dimensions * pivot * scale };
	const glm::vec2 finalPos{ offset + pos };
	SDL_Rect dst;
	dst.x = static_cast<int>(finalPos.x);
	dst.y = static_cast<int>(finalPos.y);
	const glm::vec2 destSize{ dimensions * scale };
	dst.w = static_cast<int>(destSize.x);
	dst.h = static_cast<int>(destSize.y);

	RenderTexture( texture, dst );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const glm::vec2& pos, const glm::vec2& pivot, const glm::vec2& scale, const Rect2D& src ) const
{
	const glm::vec2 dimensions{ src.w, src.h };
	const glm::vec2 offset{ -dimensions * pivot * scale };
	const glm::vec2 finalPos{ offset + pos };
	SDL_Rect dst;
	dst.x = static_cast<int>(finalPos.x);
	dst.y = static_cast<int>(finalPos.y);
	const glm::vec2 destSize{ dimensions * scale };
	dst.w = static_cast<int>(destSize.x);
	dst.h = static_cast<int>(destSize.y);

	SDL_Rect sdlSrc{ };
	sdlSrc.x = static_cast<int>(src.x);
	sdlSrc.y = static_cast<int>(src.y);
	sdlSrc.w = static_cast<int>(src.w);
	sdlSrc.h = static_cast<int>(src.h);

	RenderTexture( texture, dst, sdlSrc );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const glm::vec2& pos, const glm::vec2& pivot, float scale ) const
{
	RenderTexture( texture, pos, pivot, { scale, scale } );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const glm::vec2& pos, const glm::vec2& pivot, float scale, const Rect2D& src ) const
{
	RenderTexture( texture, pos, pivot, { scale, scale }, src );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const float x, const float y ) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture( texture.GetSDLTexture( ), nullptr, nullptr, &dst.w, &dst.h );
	RenderTexture( texture, dst );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const float x, const float y, const float width, const float height ) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	RenderTexture( texture, dst );
}

SDL_Renderer* dae::Renderer::GetSDLRenderer( ) const
{
	return m_pSDLRenderer;
}

dae::Renderer::~Renderer( )
{
	ImGui_ImplOpenGL2_Shutdown( );
	ImGui_ImplSDL2_Shutdown( );
	ImGui::DestroyContext( );

	if( m_pSDLRenderer != nullptr )
	{
		SDL_DestroyRenderer( m_pSDLRenderer );
		m_pSDLRenderer = nullptr;
	}
}

int dae::Renderer::GetOpenGLDriverIndex( )
{
	int openglIndex{ -1 };
	const int driverCount{ SDL_GetNumRenderDrivers( ) };

	for( int i{ }; i < driverCount; ++i )
	{
		SDL_RendererInfo info;
		if( !SDL_GetRenderDriverInfo( i, &info ) )
			if( !strcmp( info.name, "opengl" ) )
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const SDL_Rect& dest ) const
{
	SDL_RenderCopy( GetSDLRenderer( ), texture.GetSDLTexture( ), nullptr, &dest );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const SDL_Rect& dest, const SDL_Rect& src ) const
{
	SDL_RenderCopy( GetSDLRenderer( ), texture.GetSDLTexture( ), &src, &dest );
}
