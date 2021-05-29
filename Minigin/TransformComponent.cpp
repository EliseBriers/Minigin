#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "JsonObjectWrapper.h"

dae::TransformComponent::TransformComponent( GameObject& gameObject )
	: TransformComponent{ gameObject, 0.f, 0.f, 0.f }
{
}

dae::TransformComponent::TransformComponent( GameObject& gameObject, float x, float y, float z )
	: IComponent{ gameObject }
	, m_Position{ x, y, z }
{
}

dae::TransformComponent::TransformComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject )
	: IComponent{ gameObject }
	, m_Position{ jsonObject.GetFloat( "x" ), jsonObject.GetFloat( "y" ), jsonObject.GetFloat( "z" ) }
{
}

const glm::vec3& dae::TransformComponent::GetPosition( ) const
{
	return m_Position;
}

void dae::TransformComponent::SetPosition( float x, float y, float z )
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
