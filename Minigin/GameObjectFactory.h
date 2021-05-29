#pragma once
#include "ComponentFactory.h"

namespace dae
{
	class GameObjectFactory
	{
	public:
		std::unique_ptr<GameObject> GenerateGameObject( const JsonObjectWrapper& jsonObject );
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
