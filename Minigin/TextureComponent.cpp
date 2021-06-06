#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "Renderer.h"
#include "InitInfo.h"
#include "JsonObjectWrapper.h"
#include "Logger.h"

dae::TextureComponent::TextureComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name )
	: BaseComponent{ gameObject, std::move( name ) }
	, m_FileName{ jsonObject.GetString( "texture_file_name" ) }
	, m_pTexture{ nullptr }
	, m_pTransformComponent{ nullptr }
	, m_Pivot{ jsonObject.GetVec2( "pivot" ) }
{
}

void dae::TextureComponent::Draw( Renderer& renderer )
{
	if( !m_pTransformComponent )
	{
		Logger::LogError( "dae::TextureComponent::Draw > Failed to draw because m_pTransformComponent is nullptr" );
		return;
	}

	const glm::vec2 pos{ m_pTransformComponent->GetPosition( ) };
	const float scale{ m_pTransformComponent->GetScale( ) };
	renderer.RenderTexture( *m_pTexture, pos, m_Pivot, scale );
}

void dae::TextureComponent::Init( const InitInfo& initInfo )
{
	m_pTransformComponent = m_GameObject.get( ).GetComponent<TransformComponent>( );
	m_pTexture = &initInfo.Resource_GetTexture( m_FileName );
}
