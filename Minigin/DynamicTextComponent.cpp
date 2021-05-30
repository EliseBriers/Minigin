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
	, m_Pivot{ jsonObject.GetVec2( "pivot" ) }
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
	UpdateSize( );
}

void dae::DynamicTextComponent::EmplaceText( std::string&& text )
{
	for( char c : text )
	{
		if( c < m_Start || c >= m_Start + m_CharCount )
		{
			throw std::exception{ "Invalid character: " + c };
		}
	}

	m_Text = std::move( text );
	UpdateSize( );
}

void dae::DynamicTextComponent::Draw( Renderer& renderer )
{
	glm::vec2 transform{ GetDrawPos( ) };

	for( char c : m_Text )
	{
		const size_t offset{ size_t( c - m_Start ) };
		const Texture2D& charTexture{ m_pTextures[offset].get( ) };
		renderer.RenderTexture( charTexture, transform );

		const float width{ charTexture.GetSize( ).x };

		transform.x += width + m_Spacing;
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

void dae::DynamicTextComponent::UpdateSize( )
{
	m_Size = { };
	
	for( char c : m_Text )
	{
		const size_t offset{ size_t( c - m_Start ) };
		const Texture2D& charTexture{ m_pTextures[offset].get( ) };

		const glm::vec2 size{ charTexture.GetSize( ) };
		m_Size.x += size.x;
		m_Size.y = size.y;
	}
}

glm::vec2 dae::DynamicTextComponent::GetDrawPos( ) const
{
	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const glm::vec2 offset{ m_Size * m_Pivot };
	const glm::vec2 finalPos{ pos - offset };
	return finalPos;
}
