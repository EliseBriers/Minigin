#pragma once
#include "ManagerMap.h"
#include "IComponent.h"
#include <document.h>
#include "functional"

namespace dae
{
	class IComponent;

	class ComponentFactory
	{
		using allocator_t = std::function<std::unique_ptr<IComponent>( GameObject&, const rapidjson::Value::Object& )>;
		using map_t = std::unordered_map<std::string, allocator_t>;
	public:
		template <typename T>
		void RegisterComponent( );
		std::unique_ptr<IComponent> GetComponent( GameObject& gameObject, const rapidjson::Value::Object& jsonObject );
	private:
		std::unique_ptr<IComponent> GetComponent( const std::string& name, GameObject& gameObject, const rapidjson::Value::Object& jsonObject );
		map_t m_Map;

		template <typename T>
		static std::unique_ptr<IComponent> DefaultAllocator( GameObject& gameObject, const rapidjson::Value::Object& jsonObject );
	};

	template <typename T>
	void ComponentFactory::RegisterComponent( )
	{
		const std::string name{ typeid(T).name( ) };
		m_Map[name] = DefaultAllocator<T>;
	}

	template <typename T>
	std::unique_ptr<IComponent> ComponentFactory::DefaultAllocator( GameObject& gameObject, const rapidjson::Value::Object& jsonObject )
	{
		return std::make_unique<T>( gameObject, jsonObject );
	}
}
