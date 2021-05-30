#include "MiniginPCH.h"
#include "Texture2D.h"
#include <SDL.h>

dae::Texture2D::~Texture2D( )
{
	SDL_DestroyTexture( m_pTexture );
}

SDL_Texture* dae::Texture2D::GetSDLTexture( ) const
{
	return m_pTexture;
}

const glm::vec2& dae::Texture2D::GetSize( ) const
{
	return m_Size;
}

dae::Texture2D::Texture2D( SDL_Texture* pTexture )
	: m_pTexture{ pTexture }
{
	int width;
	int height;
	SDL_QueryTexture( m_pTexture, nullptr, nullptr, &width, &height );
	m_Size.x = static_cast<float>(width);
	m_Size.y = static_cast<float>(height);
}
