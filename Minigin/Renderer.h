#pragma once

struct SDL_Window;
struct SDL_Renderer;

namespace dae
{
	class SceneManager;
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final
	{
	public:
		Renderer( );
		void Init( SDL_Window* window, bool useVSync );
		void Render( SceneManager& sceneManager );

		void RenderTexture( const Texture2D& texture, float x, float y ) const;
		void RenderTexture( const Texture2D& texture, float x, float y, float width, float height ) const;

		SDL_Renderer* GetSDLRenderer( ) const;

		~Renderer( );
		Renderer( const Renderer& other ) = delete;
		Renderer( Renderer&& other ) noexcept = delete;
		Renderer& operator=( const Renderer& other ) = delete;
		Renderer& operator=( Renderer&& other ) noexcept = delete;
	private:
		// ReSharper disable once CppInconsistentNaming
		SDL_Renderer* m_pSDLRenderer;
		SDL_Window* m_pWindow;
		bool m_ShowDemo;

		// ReSharper disable once CppInconsistentNaming
		static int GetOpenGLDriverIndex( );
	};
}
