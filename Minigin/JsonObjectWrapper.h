#pragma once
#include <document.h>

namespace dae
{
	class JsonObjectWrapper
	{
	public:
		JsonObjectWrapper( const rapidjson::Value::Object& obj, std::string name );
		std::string GetString( const std::string& idx ) const;
		float GetFloat( const std::string& idx ) const;
		uint32_t GetUint( const std::string& idx ) const;
		JsonObjectWrapper GetObjectWrapper( const std::string& idx ) const;
		rapidjson::Value::Array GetArray( const std::string& idx ) const;
		std::vector<JsonObjectWrapper> GetObjectArray( const std::string& idx ) const;
	private:
		rapidjson::Value::Object m_Object;
		std::string m_Name;
	};
}
