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
	m_pSDLRenderer = SDL_CreateRenderer( window, GetOpenGLDriverIndex(), flags );
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

void dae::Renderer::RenderTexture( const Texture2D& texture, const float x, const float y ) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture( texture.GetSDLTexture( ), nullptr, nullptr, &dst.w, &dst.h );
	SDL_RenderCopy( GetSDLRenderer( ), texture.GetSDLTexture( ), nullptr, &dst );
	// SDL_RenderCopyEx( GetSDLRenderer( ), texture.GetSDLTexture( ), nullptr, &dst, 0.f, nullptr, { } );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const float x, const float y, const float width, const float height ) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy( GetSDLRenderer( ), texture.GetSDLTexture( ), nullptr, &dst );
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
