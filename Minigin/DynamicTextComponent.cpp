#include "MiniginPCH.h"
#include "DynamicTextComponent.h"
#include "InitInfo.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include <SDL.h>
#include "JsonObjectWrapper.h"

dae::DynamicTextComponent::DynamicTextComponent( GameObject& gameObject, char start, uint8_t charCount, float spacing, std::string fontFileName, uint32_t fontSize )
	: IComponent{ gameObject }
	, m_FontFileName{ fontFileName }
	, m_FontSize{ fontSize }
	, m_Spacing{ spacing }
	, m_pTransform{ }
	, m_Start{ start }
	, m_CharCount{ charCount }
{
	m_pTextures.reserve( charCount );
}

dae::DynamicTextComponent::DynamicTextComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject )
	: IComponent{ gameObject }
	, m_pTextures{ }
	, m_FontFileName{ jsonObject.GetString( "font_file_name" ) }
	, m_Text{ }
	, m_FontSize{ jsonObject.GetUint( "size" ) }
	, m_Spacing{ }
	, m_pTransform{ nullptr }
	, m_Start{ jsonObject.GetString( "start_char" )[0] }
	, m_CharCount{ static_cast<uint8_t>(jsonObject.GetUint( "char_count" )) }
{
}

void dae::DynamicTextComponent::SetText( const std::string& text )
{
	for( char c : text )
	{
		if( c < m_Start || c >= m_Start + m_CharCount )
		{
			throw std::exception{ "Invalid character: " + c };
		}
	}

	m_Text = text;
}

void dae::DynamicTextComponent::EmplaceText( std::string&& text )
{
	m_Text = std::move( text );
}

void dae::DynamicTextComponent::Draw( Renderer& renderer )
{
	glm::vec3 transform{ m_pTransform->GetPosition( ) };

	for( char c : m_Text )
	{
		size_t offset{ size_t( c - m_Start ) };
		const Texture2D& charTexture{ m_pTextures[offset].get( ) };
		renderer.RenderTexture( charTexture, transform.x, transform.y );

		int width{ };
		int height{ };
		SDL_QueryTexture( charTexture.GetSDLTexture( ), nullptr, nullptr, &width, &height );

		transform.x += float( width ) + m_Spacing;
	}
}

void dae::DynamicTextComponent::Init( const InitInfo& initInfo )
{
	m_pTransform = m_GameObject.get( ).GetComponent<TransformComponent>( );

	for( char i{ }; i < m_CharCount; ++i )
	{
		std::string str{ m_Start + i };
		m_pTextures.push_back( initInfo.Resource_GetTextTexture( str, m_FontFileName, m_FontSize ) );
	}
}
