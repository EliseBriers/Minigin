#pragma once
#include "SceneManager.h"
#include "memory"

namespace dae
{
	class GameObject;
	class InitInfo;

	class Scene
	{
	public:
		explicit Scene( const std::string& name );

		void Add( std::unique_ptr<GameObject> object );

		void FixedUpdate( const UpdateInfo& updateInfo );
		void Update( const UpdateInfo& updateInfo );
		void Render( Renderer& renderer ) const;
		void InitGameObjects( InitInfo& initInfo );

		GameObject* GetGameObject( const std::string& name ) const;

		~Scene( );
		const std::string& GetName( ) const;
		Scene( const Scene& other ) = delete;
		Scene( Scene&& other ) = delete;
		Scene& operator=( const Scene& other ) = delete;
		Scene& operator=( Scene&& other ) = delete;
	private:
		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects;
		std::vector<std::unique_ptr<GameObject>> m_UninitializedObjects;
	};
}
