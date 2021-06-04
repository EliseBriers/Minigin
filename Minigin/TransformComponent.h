#pragma once
#include "IComponent.h"
// #pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
// #pragma warning(pop)

namespace dae
{
	class JsonObjectWrapper;

	class TransformComponent : public IComponent
	{
	public:
		TransformComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name );
		const glm::vec3& GetPosition( ) const;
		float GetScale( ) const;

		void SetPosition( float x, float y, float z );
		void SetScale( float scale );
		bool MoveTo( const glm::vec2& endPos, float amount );
	private:
		glm::vec3 m_Position;
		float m_Scale;
	};
}
