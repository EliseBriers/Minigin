#pragma once
struct SDL_Texture;
#pragma warning (disable:4201)
#include <glm/vec2.hpp>

namespace dae
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D
	{
	public:
		SDL_Texture* GetSDLTexture( ) const;
		const glm::vec2& GetSize( ) const;
		explicit Texture2D( SDL_Texture* pTexture );
		~Texture2D( );

		Texture2D( const Texture2D& ) = delete;
		Texture2D( Texture2D&& ) = delete;
		Texture2D& operator=( const Texture2D& ) = delete;
		Texture2D& operator=( const Texture2D&& ) = delete;
	private:
		SDL_Texture* m_pTexture;
		glm::vec2 m_Size;
	};
}
