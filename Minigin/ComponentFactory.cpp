#include "MiniginPCH.h"
#include "ComponentFactory.h"

std::unique_ptr<dae::IComponent> dae::ComponentFactory::GetComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject )
{
	const std::string className{ jsonObject.GetString( "class" ) };
	const JsonObjectWrapper object{ jsonObject.GetObjectWrapper("desc") };
	return GetComponent( className, gameObject, object );
}

std::unique_ptr<dae::IComponent> dae::ComponentFactory::GetComponent( const std::string& name, GameObject& gameObject, const JsonObjectWrapper& jsonObject )
{
	const auto it{ m_Map.find( "class " + name ) };
	if( it == m_Map.end( ) )
		return nullptr;

	return it->second( gameObject, jsonObject );
}
