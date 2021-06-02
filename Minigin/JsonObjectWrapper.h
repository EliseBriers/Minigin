#pragma once
#include <document.h>
#include "Renderer.h"
#include <iosfwd>
#include <vector>
#include <functional>

namespace dae
{
	class ResourceManager;

	class JsonObjectWrapper
	{
	public:
		JsonObjectWrapper( const rapidjson::Value::ConstObject& obj, std::string name, ResourceManager& resourceManager );
		JsonObjectWrapper( const std::string& fileName, ResourceManager& resourceManager );
		std::string GetFullVarName( const std::string& idx ) const;
		std::string GetString( const std::string& idx ) const;
		std::string GetOptionalString( const std::string& idx, const std::string& defaultVal ) const;
		float GetFloat( const std::string& idx ) const;
		float GetOptionalFloat( const std::string& idx, float defaultVal ) const;
		uint32_t GetUint( const std::string& idx ) const;
		JsonObjectWrapper GetObjectWrapper( const std::string& idx ) const;
		std::vector<JsonObjectWrapper> GetObjectArray( const std::string& idx ) const;
		glm::vec2 GetVec2( const std::string& idx ) const;

		template <typename T>
		std::vector<T> GetVector( const std::string& idx, std::function<T( const JsonObjectWrapper& )> func ) const;
	private:
		void WarnKeyNotFound( const std::string& currentFunction, const std::string& idx ) const;
		void ErrorKeyNotFound( const std::string& currentFunction, const std::string& idx ) const;
		void WarnInvalidObject( const std::string& currentFunction, const std::string& idx, const std::string& objectType ) const;
		void ErrorInvalidObject( const std::string& currentFunction, const std::string& idx, const std::string& objectType ) const;

		rapidjson::Value::ConstObject m_Object;
		std::string m_Name;

		ResourceManager& m_ResourceManager;
	};

	template <typename T>
	std::vector<T> JsonObjectWrapper::GetVector( const std::string& idx, std::function<T( const JsonObjectWrapper& )> func ) const
	{
		std::vector<JsonObjectWrapper> jsonObjects{ GetObjectArray( idx ) };
		std::vector<T> values;

		for( const JsonObjectWrapper& jsonObj : jsonObjects )
		{
			values.push_back( func( jsonObj ) );
		}

		return values;
	}
}
