#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "Renderer.h"
#include "InitInfo.h"
#include "JsonObjectWrapper.h"

dae::TextureComponent::TextureComponent( const std::string& fileName, GameObject& gameObject )
	: IComponent{ gameObject }
	, m_FileName{ fileName }
	, m_pTexture{ nullptr }
	, m_pTransformComponent{ nullptr }
{
}

dae::TextureComponent::TextureComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject )
	: IComponent{ gameObject }
	, m_FileName{ jsonObject.GetString( "texture_file_name" ) }
	, m_pTexture{ nullptr }
	, m_pTransformComponent{ nullptr }
	, m_Pivot{ jsonObject.GetVec2( "pivot" ) }
{
}

void dae::TextureComponent::Draw( Renderer& renderer )
{
	const glm::vec2 pos{ m_pTransformComponent->GetPosition( ) };
	const glm::vec2& size{ m_pTexture->GetSize( ) };
	const glm::vec2 offset{ size * m_Pivot };
	const glm::vec2 finalPos{ pos - offset };
	renderer.RenderTexture( *m_pTexture, finalPos);
}

void dae::TextureComponent::Init( const InitInfo& initInfo )
{
	m_pTransformComponent = m_GameObject.get( ).GetComponent<TransformComponent>( );
	m_pTexture = &initInfo.Resource_GetTexture( m_FileName );
}
