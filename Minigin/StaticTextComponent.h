#pragma once
#include "BaseComponent.h"
#include <string>
#include <document.h>
#include <SDL.h>

namespace dae
{
	class JsonObjectWrapper;
	class TransformComponent;

	class StaticTextComponent final : public BaseComponent
	{
	public:
		StaticTextComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name );
		void Draw( Renderer& renderer ) override;
		void Init( const InitInfo& initInfo ) override;
		glm::vec2 GetPosition( ) const;
		glm::vec2 GetSize( ) const;

		~StaticTextComponent( ) override = default;
		StaticTextComponent( const StaticTextComponent& other ) = delete;
		StaticTextComponent( StaticTextComponent&& other ) noexcept = delete;
		StaticTextComponent& operator=( const StaticTextComponent& other ) = delete;
		StaticTextComponent& operator=( StaticTextComponent&& other ) noexcept = delete;
	private:
		SDL_Color m_Color;
		std::string m_Text;
		std::string m_FontFileName;
		Texture2D const* m_pTexture;
		TransformComponent const* m_pTransformComponent;
		uint32_t m_Size;
		glm::vec2 m_Pivot;
	};
}
