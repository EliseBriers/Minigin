#pragma once
#include <string>
#include <memory>
#include <vector>
#include "UUID.h"

namespace dae
{
	class UpdateInfo;
	class InitInfo;
	class BaseComponent;

	class ResourceManager;
	class Renderer;
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject( const std::string& name );
		GameObject( std::string&& name );
		void AddComponent( std::unique_ptr<BaseComponent> pComponent );

		void Draw( Renderer& renderer );
		void FixedUpdate( const UpdateInfo& updateInfo );
		void PersistentFixedUpdate( const UpdateInfo& updateInfo );
		void Update( const UpdateInfo& updateInfo );
		void PersistentUpdate( const UpdateInfo& updateInfo );
		void Init( InitInfo& initInfo );
		void Deactivate( );
		void Activate( );
		void Pause( );
		void UnPause( );

		template <typename T>
		T* GetComponent( ) const;
		template <typename T>
		T* GetComponentByName( const std::string& name ) const;
		const std::string& GetName( ) const;
		const UUID<GameObject> Id;

		GameObject( ) = default;

		bool IsActive( ) const;
		bool IsPaused( ) const;

		~GameObject( );
		GameObject( const GameObject& other ) = delete;
		GameObject( GameObject&& other ) = delete;
		GameObject& operator=( const GameObject& other ) = delete;
		GameObject& operator=( GameObject&& other ) = delete;
	private:
		std::vector<std::unique_ptr<BaseComponent>> m_pComponents;
		const std::string m_Name;
		bool m_IsActive;
		bool m_IsPaused;
	};

	template <typename T>
	T* GameObject::GetComponent( ) const
	{
		for( const std::unique_ptr<BaseComponent>& pComponent : m_pComponents )
		{
			T* pReturnVal{ dynamic_cast<T*>(pComponent.get( )) };
			if( pReturnVal )
				return pReturnVal;
		}
		return nullptr;
	}

	template <typename T>
	T* GameObject::GetComponentByName( const std::string& name ) const
	{
		for( const std::unique_ptr<BaseComponent>& pComponent : m_pComponents )
		{
			T* pReturnVal{ dynamic_cast<T*>(pComponent.get( )) };
			if( pReturnVal && pReturnVal->GetName( ) == name )
				return pReturnVal;
		}
		return nullptr;
	}
}
