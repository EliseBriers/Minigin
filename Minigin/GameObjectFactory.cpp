#include "MiniginPCH.h"
#include "GameObjectFactory.h"

std::unique_ptr<dae::GameObject> dae::GameObjectFactory::GenerateGameObject( const rapidjson::Value::Object& jsonObject )
{
	rapidjson::Value::Array components{ jsonObject["components"].GetArray( ) };
	const std::string name{ jsonObject["name"].GetString( ) };

	std::unique_ptr<GameObject> pGameObject{ std::make_unique<GameObject>( name ) };

	for( auto& comp : components )
	{
		pGameObject->AddComponent( m_ComponentFactory.GetComponent( *pGameObject, comp.GetObjectA( ) ) );
	}

	return pGameObject;
}
