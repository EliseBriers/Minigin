#pragma once
#include <document.h>

namespace dae
{
	class ResourceManager;

	class JsonObjectWrapper
	{
	public:
		JsonObjectWrapper( const rapidjson::Value::ConstObject& obj, std::string name, ResourceManager& resourceManager );
		JsonObjectWrapper( const std::string& fileName, ResourceManager& resourceManager );
		std::string GetString( const std::string& idx ) const;
		float GetFloat( const std::string& idx ) const;
		uint32_t GetUint( const std::string& idx ) const;
		JsonObjectWrapper GetObjectWrapper( const std::string& idx ) const;
		std::vector<JsonObjectWrapper> GetObjectArray( const std::string& idx ) const;
		glm::vec2 GetVec2( const std::string& idx ) const;
	private:
		rapidjson::Value::ConstObject m_Object;
		std::string m_Name;

		ResourceManager& m_ResourceManager;
	};
}
