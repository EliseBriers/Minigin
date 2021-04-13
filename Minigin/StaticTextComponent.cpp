#include "MiniginPCH.h"
#include "StaticTextComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "InitInfo.h"

dae::StaticTextComponent::StaticTextComponent( GameObject& gameObject, const std::string& text, const std::string& fontFileName, uint32_t size )
	: IComponent{ gameObject }
	, m_Text{ text }
	, m_FontFileName{ fontFileName }
	, m_Size{ size }
{
}

void dae::StaticTextComponent::Draw( Renderer& renderer )
{
	const glm::vec3 pos{ m_pTransformComponent->GetPosition( ) };
	renderer.RenderTexture( *m_pTexture, pos.x, pos.y );
}

void dae::StaticTextComponent::Init( const InitInfo& initInfo )
{
	m_pTransformComponent = m_GameObject.get( ).GetComponent<TransformComponent>( );

	m_pTexture = &initInfo.Resource_GetTextTexture( m_Text, m_FontFileName, m_Size );
}
