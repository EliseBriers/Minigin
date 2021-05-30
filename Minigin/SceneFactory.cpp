#include "MiniginPCH.h"
#include "SceneFactory.h"
#include <istreamwrapper.h>
#include <fstream>
#include "Scene.h"
#include "ResourceManager.h"

dae::SceneFactory::SceneFactory( std::string dataFolder, ResourceManager& resourceManager )
	: m_DataFolder{ std::move( dataFolder ) }
	, m_ResourceManager{ resourceManager }
{
}

std::unique_ptr<dae::Scene> dae::SceneFactory::ReadScene( const std::string& fileName )
{
	using namespace rapidjson;
	// Read Json File
	const Document& doc{ m_ResourceManager.GetJsonDocument( fileName ) };
	rapidjson::Value::ConstObject obj{ doc.GetObjectA() };
	const JsonObjectWrapper rootObject{doc.GetObjectA(), fileName, m_ResourceManager};
	
	// Extract name and gameObjects
	const std::string name{ rootObject.GetString( "name" ) };
	std::vector<JsonObjectWrapper> gameObjects{ rootObject.GetObjectArray( "objects" ) };

	// Build Scene
	std::unique_ptr<Scene> pScene{ std::make_unique<Scene>( name ) };
	for( auto& gameObject : gameObjects )
	{
		pScene->Add( m_GameObjectFactory.GenerateGameObject( gameObject ) );
	}

	return pScene;
}
