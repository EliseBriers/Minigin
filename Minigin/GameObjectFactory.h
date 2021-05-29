#pragma once
#include "ComponentFactory.h"

namespace dae
{
	class GameObjectFactory
	{
	public:
		std::unique_ptr<GameObject> GenerateGameObject( const rapidjson::Value::Object& jsonObject );
		template <typename T>
		void RegisterComponent( );
	private:
		ComponentFactory m_ComponentFactory;
	};

	template <typename T>
	void GameObjectFactory::RegisterComponent( )
	{
		m_ComponentFactory.RegisterComponent<T>( );
	}
}
