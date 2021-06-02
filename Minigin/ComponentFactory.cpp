#include "MiniginPCH.h"
#include "ComponentFactory.h"

std::unique_ptr<dae::IComponent> dae::ComponentFactory::GetComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject )
{
	const std::string className{ jsonObject.GetString( "class" ) };
	const JsonObjectWrapper object{ jsonObject.GetObjectWrapper( "desc" ) };
	std::string name{ jsonObject.GetOptionalString( "name", "" ) };
	return GetComponent( className, gameObject, object, std::move( name ) );
}

std::unique_ptr<dae::IComponent> dae::ComponentFactory::GetComponent( const std::string& className, GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name )
{
	const auto it{ m_Map.find( "class " + className ) };
	if( it == m_Map.end( ) )
		return nullptr;

	return it->second( gameObject, jsonObject, std::move( name ) );
}
