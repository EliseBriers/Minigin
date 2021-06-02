#pragma once
#include "IComponent.h"
#include <string>
#include <document.h>

namespace dae
{
	class JsonObjectWrapper;
	class TransformComponent;

	class StaticTextComponent : public IComponent
	{
	public:
		StaticTextComponent(GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name);
		void Draw( Renderer& renderer ) override;
		void Init( const InitInfo& initInfo ) override;

		~StaticTextComponent( ) override = default;
		StaticTextComponent( const StaticTextComponent& other ) = delete;
		StaticTextComponent( StaticTextComponent&& other ) noexcept = delete;
		StaticTextComponent& operator=( const StaticTextComponent& other ) = delete;
		StaticTextComponent& operator=( StaticTextComponent&& other ) noexcept = delete;
	private:
		std::string m_Text;
		std::string m_FontFileName;
		Texture2D const* m_pTexture;
		TransformComponent const* m_pTransformComponent;
		uint32_t m_Size;
		glm::vec2 m_Pivot;
	};
}
