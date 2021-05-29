#pragma once
#include "IComponent.h"
// #pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <document.h>
// #pragma warning(pop)

namespace dae
{
	class JsonObjectWrapper;

	class TransformComponent : public IComponent
	{
	public:
		TransformComponent( GameObject& gameObject );
		TransformComponent( GameObject& gameObject, float x, float y, float z );
		TransformComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject );
		const glm::vec3& GetPosition( ) const;

		void SetPosition( float x, float y, float z );
	private:
		glm::vec3 m_Position;
	};
}
