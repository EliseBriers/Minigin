#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "JsonObjectWrapper.h"
#include <glm/detail/func_geometric.inl>

dae::TransformComponent::TransformComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name )
	: BaseComponent{ gameObject, std::move( name ) }
	, m_Position{ jsonObject.GetFloat( "x" ), jsonObject.GetFloat( "y" ), jsonObject.GetFloat( "z" ) }
	, m_Scale{ jsonObject.GetOptionalFloat( "scale", 1.f ) }
{
}

const glm::vec3& dae::TransformComponent::GetPosition( ) const
{
	return m_Position;
}

float dae::TransformComponent::GetScale( ) const
{
	return m_Scale;
}

void dae::TransformComponent::SetPosition( float x, float y, float z )
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::TransformComponent::SetScale( float scale )
{
	m_Scale = scale;
}

bool dae::TransformComponent::MoveTo( const glm::vec2& endPos, float amount )
{
	const glm::vec2 pos{ GetPosition( ) };
	const glm::vec2 toTarget{ endPos - pos };
	const float distance{ length( toTarget ) };
	const glm::vec2 nToTarget{ toTarget / distance };

	const glm::vec2 movement{ nToTarget * amount };

	if( length( movement ) > distance )
	{
		SetPosition( endPos.x, endPos.y, 0.f );
		return true;
	}
	const glm::vec2 newPos{ pos + movement };
	SetPosition( newPos.x, newPos.y, 0.f );
	return false;
}
