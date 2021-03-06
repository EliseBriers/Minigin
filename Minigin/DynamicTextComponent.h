#pragma once
#include "BaseComponent.h"
#include <vector>
#include <document.h>
#include <SDL.h>

namespace dae
{
	class JsonObjectWrapper;
	class TransformComponent;

	class DynamicTextComponent final : public BaseComponent
	{
	public:
		DynamicTextComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name );

		void SetText( const std::string& text );
		void EmplaceText( std::string&& text );
		void Draw( Renderer& renderer ) override;
		void Init( const InitInfo& initInfo ) override;

		~DynamicTextComponent( ) override = default;
		DynamicTextComponent( const DynamicTextComponent& other ) = delete;
		DynamicTextComponent( DynamicTextComponent&& other ) noexcept = delete;
		DynamicTextComponent& operator=( const DynamicTextComponent& other ) = delete;
		DynamicTextComponent& operator=( DynamicTextComponent&& other ) noexcept = delete;
	private:
		std::vector<std::reference_wrapper<const Texture2D>> m_pTextures;
		std::string m_FontFileName;
		std::string m_Text;
		uint32_t m_FontSize;
		float m_Spacing;
		TransformComponent* m_pTransform;
		char m_Start;
		uint8_t m_CharCount;
		glm::vec2 m_Size;
		glm::vec2 m_Pivot;
		SDL_Color m_Color;

		void UpdateSize( );
		glm::vec2 GetDrawPos( ) const;
	};
}
