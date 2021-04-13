#pragma once
#include "IComponent.h"
#include "Texture2D.h"
#include "TransformComponent.h"

namespace dae
{
	class TextureComponent : public IComponent
	{
	public:
		TextureComponent( const std::string& fileName, GameObject& gameObject );
		void Draw( Renderer& renderer ) override;
		void Init( const InitInfo& ) override;

		~TextureComponent( ) override = default;
		TextureComponent( const TextureComponent& other ) = delete;
		TextureComponent( TextureComponent&& other ) noexcept = delete;
		TextureComponent& operator=( const TextureComponent& other ) = delete;
		TextureComponent& operator=( TextureComponent&& other ) noexcept = delete;
	private:
		std::string m_FileName;
		Texture2D const* m_pTexture;
		TransformComponent* m_pTransformComponent;
	};
}
