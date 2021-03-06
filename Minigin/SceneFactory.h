#pragma once
#include "GameObjectFactory.h"

namespace dae
{
	class Scene;

	class SceneFactory
	{
	public:
		SceneFactory( std::string dataFolder, ResourceManager& resourceManager);
		std::unique_ptr<Scene> ReadScene( const std::string& fileName );
		template <typename T>
		void RegisterComponent( );
	private:
		std::string m_DataFolder;
		GameObjectFactory m_GameObjectFactory;
		ResourceManager& m_ResourceManager;
	};

	template <typename T>
	void SceneFactory::RegisterComponent( )
	{
		return m_GameObjectFactory.RegisterComponent<T>( );
	}
}
