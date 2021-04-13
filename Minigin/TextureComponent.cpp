#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "Renderer.h"
#include "InitInfo.h"

dae::TextureComponent::TextureComponent( const std::string& fileName, GameObject& gameObject )
	: IComponent{ gameObject }
	, m_FileName{ fileName }
	, m_pTexture{ nullptr }
	, m_pTransformComponent{ nullptr }
{
}

void dae::TextureComponent::Draw( Renderer& renderer )
{
	const glm::vec3 pos{ m_pTransformComponent->GetPosition( ) };
	renderer.RenderTexture( *m_pTexture, pos.x, pos.y );
}

void dae::TextureComponent::Init( const InitInfo& initInfo )
{
	m_pTransformComponent = m_GameObject.get( ).GetComponent<TransformComponent>( );

	m_pTexture = &initInfo.Resource_GetTexture( m_FileName );
}
