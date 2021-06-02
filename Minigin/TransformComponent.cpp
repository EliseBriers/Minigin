#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "JsonObjectWrapper.h"

dae::TransformComponent::TransformComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name )
	: IComponent{ gameObject, std::move( name ) }
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
