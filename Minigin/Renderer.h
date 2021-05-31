#pragma once
#pragma warning (disable:4201)
#include <glm/vec2.hpp>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace dae
{
	class SceneManager;
	class Texture2D;

	struct Rect2D
	{
		float x;
		float y;
		float w;
		float h;
	};
	
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final
	{
	public:
		Renderer( );
		void Init( SDL_Window* window, bool useVSync );
		void Render( SceneManager& sceneManager );

		void RenderTexture( const Texture2D& texture, const glm::vec2& pos ) const;
		void RenderTexture( const Texture2D& texture, const glm::vec2& pos, const glm::vec2& pivot, const glm::vec2& scale ) const;
		void RenderTexture( const Texture2D& texture, const glm::vec2& pos, const glm::vec2& pivot, const glm::vec2& scale, const Rect2D& src ) const;
		void RenderTexture( const Texture2D& texture, const glm::vec2& pos, const glm::vec2& pivot, float scale ) const;
		void RenderTexture( const Texture2D& texture, const glm::vec2& pos, const glm::vec2& pivot, float scale, const Rect2D& src ) const;
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
		void RenderTexture( const Texture2D& texture, const SDL_Rect& dest ) const;
		void RenderTexture( const Texture2D& texture, const SDL_Rect& dest, const SDL_Rect& src ) const;
	};
}
