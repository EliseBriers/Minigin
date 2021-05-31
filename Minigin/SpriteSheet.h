#pragma once
#include "IComponent.h"
#include "Renderer.h"

namespace dae
{
	class JsonObjectWrapper;

	class SpriteSheet
	{
	public:
		SpriteSheet( const JsonObjectWrapper& jsonObject );

		void Init( const InitInfo& initInfo );
		void Draw( Renderer& renderer, const glm::vec2& pos, const glm::vec2& pivot, float scale, size_t spriteIdx );

		const Rect2D GetSpriteSource( size_t idx ) const;
	private:
		std::string m_FileName;
		std::vector<Rect2D> m_SpriteSources;
		const Texture2D* m_pTexture;

		static Rect2D ObjectToRect( const JsonObjectWrapper& jsonObject );
	};
}
