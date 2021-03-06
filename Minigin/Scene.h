#pragma once
#include "SceneManager.h"
#include "memory"

namespace dae
{
	class GameObject;
	class InitInfo;
	class SceneBehavior;

	class Scene
	{
	public:
		explicit Scene( const std::string& name );

		void Add( std::unique_ptr<GameObject> object );

		void FixedUpdate( const UpdateInfo& updateInfo );
		void Update( const UpdateInfo& updateInfo );
		void Render( Renderer& renderer ) const;
		void Init( InitInfo& initInfo );

		const std::string& GetName( ) const;
		GameObject* GetGameObject( const std::string& name ) const;
		SceneManager* GetSceneManager( ) const;
		void SetSceneManager( SceneManager* pSceneManager );
		void SetBehavior( std::unique_ptr<SceneBehavior> pBehavior );
		template <typename T>
		T* GetSceneBehaviorAs( ) const;

		~Scene( );
		Scene( const Scene& other ) = delete;
		Scene( Scene&& other ) = delete;
		Scene& operator=( const Scene& other ) = delete;
		Scene& operator=( Scene&& other ) = delete;
	private:
		SceneManager* m_pSceneManager;
		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects;
		std::vector<std::unique_ptr<GameObject>> m_UninitializedObjects;
		std::unique_ptr<SceneBehavior> m_pSceneBehavior;
		bool m_InitializedBehavior;

		void MoveActiveGameObjects( );
	};

	template <typename T>
	T* Scene::GetSceneBehaviorAs( ) const
	{
		return dynamic_cast<T*>(m_pSceneBehavior.get( ));
	}
}
