#include "MiniginPCH.h"
#include "SceneFactory.h"
#include <istreamwrapper.h>
#include <fstream>
#include "Scene.h"

dae::SceneFactory::SceneFactory( std::string dataFolder )
	: m_DataFolder{ std::move( dataFolder ) }
{
}

std::unique_ptr<dae::Scene> dae::SceneFactory::ReadScene( const std::string& fileName )
{
	using namespace rapidjson;
	// Read Json File
	const std::string fileString{ m_DataFolder + fileName };
	std::ifstream testJsonFile{ fileString };
	IStreamWrapper streamWrapper{ testJsonFile };
	Document doc{ };
	doc.ParseStream( streamWrapper );
	assert( doc.IsObject() );
	Value::Object rootObject{ doc.GetObjectA( ) };
	
	// Extract name and gameObjects
	const std::string name{ rootObject["name"].GetString( ) };
	Value::Array gameObjects{ rootObject["objects"].GetArray( ) };

	// Build Scene
	std::unique_ptr<Scene> pScene{ std::make_unique<Scene>( name ) };
	for( auto& gameObject : gameObjects )
	{
		pScene->Add( m_GameObjectFactory.GenerateGameObject( gameObject.GetObjectA( ) ) );
	}

	return pScene;
}
