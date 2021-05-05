#pragma once
#include "SceneManager.h"
#include "memory"

namespace dae
{
	class GameObject;
	class InitInfo;

	class Scene
	{
		friend Scene& SceneManager::CreateScene( const std::string& name );
		// private Constructor tag is used to allow for private constructor combined with make_unique
		struct ConstructorTag
		{
		};

	public:
		explicit Scene( const std::string& name, const ConstructorTag& );

		void Add( std::unique_ptr<GameObject> object, InitInfo& initInfo );

		void FixedUpdate( const UpdateInfo& updateInfo );
		void Update( const UpdateInfo& updateInfo );
		void Render( Renderer& renderer ) const;

		GameObject* GetGameObject( const std::string& name ) const;

		~Scene( );
		Scene( const Scene& other ) = delete;
		Scene( Scene&& other ) = delete;
		Scene& operator=( const Scene& other ) = delete;
		Scene& operator=( Scene&& other ) = delete;
	private:

		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects;

		static unsigned int m_IdCounter;
	};
}
