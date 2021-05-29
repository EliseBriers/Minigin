#include "MiniginPCH.h"
#include "GameObjectFactory.h"

std::unique_ptr<dae::GameObject> dae::GameObjectFactory::GenerateGameObject( const JsonObjectWrapper& jsonObject )
{
	std::vector<JsonObjectWrapper> components{ jsonObject.GetObjectArray( "components" ) };
	const std::string name{ jsonObject.GetString( "name" ) };

	std::unique_ptr<GameObject> pGameObject{ std::make_unique<GameObject>( name ) };

	for( auto& comp : components )
	{
		pGameObject->AddComponent( m_ComponentFactory.GetComponent( *pGameObject, comp ) );
	}

	return pGameObject;
}
