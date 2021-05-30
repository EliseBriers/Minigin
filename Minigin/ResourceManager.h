#pragma once
#include "ManagerMap.h"
#include "Texture2D.h"
#include "Font.h"
#include <document.h>

namespace dae
{
	class Renderer;

	class ResourceManager final
	{
		using font_info_t = std::pair<std::string, uint32_t>;
	public:
		struct DataPaths
		{
			std::string TextureDataPath;
			std::string FontDataPath;
			std::string SceneDataPath;
		};

		ResourceManager(DataPaths dataPaths);
		void Init(  );
		const Texture2D& GetTexture( const std::string& file, Renderer& renderer );
		Font& LoadFont( const std::string& file, uint32_t size );
		const Texture2D& LoadText( const std::string& text, const std::string& fileName, uint32_t size, Renderer& renderer );
		const rapidjson::Document& GetJsonDocument( const std::string& fileName );
		rapidjson::Value::ConstObject GetJsonConstObject( const std::string& fileName );
	private:
		DataPaths m_DataPaths;
		ManagerMap<Texture2D, const std::string&, Renderer&> m_Textures;
		ManagerMap<Font, const std::string&, uint32_t> m_Fonts;
		ManagerMap<Texture2D, const std::string&, Renderer&, const Font&> m_TextTextures;
		ManagerMap<rapidjson::Document, const std::string&> m_JsonDocuments;

		static std::unique_ptr<Texture2D> TextureAllocator( const std::string& file, Renderer& renderer );
		static std::unique_ptr<Font> FontAllocator( const std::string& file, uint32_t size );
		static std::unique_ptr<Texture2D> TextAllocator( const std::string& text, Renderer& renderer, const Font& font );
		static std::unique_ptr<rapidjson::Document> JsonDocumentAllocator( const std::string& fileName );
	};
}
