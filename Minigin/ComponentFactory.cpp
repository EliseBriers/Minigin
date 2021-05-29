#include "MiniginPCH.h"
#include "ComponentFactory.h"

std::unique_ptr<dae::IComponent> dae::ComponentFactory::GetComponent( GameObject& gameObject, const rapidjson::Value::Object& jsonObject )
{
	const std::string className{ jsonObject["class"].GetString( ) };
	const rapidjson::Value::Object object{ jsonObject["desc"].GetObjectA( ) };
	return GetComponent( className, gameObject, object );
}

std::unique_ptr<dae::IComponent> dae::ComponentFactory::GetComponent( const std::string& name, GameObject& gameObject, const rapidjson::Value::Object& jsonObject )
{
	const auto it{ m_Map.find( "class " + name ) };
	if( it == m_Map.end( ) )
		return nullptr;

	return it->second( gameObject, jsonObject );
}
