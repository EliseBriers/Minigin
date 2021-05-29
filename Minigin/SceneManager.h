#pragma once

namespace dae
{
	class UpdateInfo;
	class Renderer;
	class Scene;

	class SceneManager final
	{
	public:
		SceneManager( ) = default;
		void AddScene( std::unique_ptr<Scene> pScene );
		void AddSceneAndMakeActive( std::unique_ptr<Scene> pScene );
		Scene* GetSceneByName( const std::string& sceneName );
		void SetActiveScene( const std::string& sceneName );

		void FixedUpdate( const UpdateInfo& updateInfo ) const;
		void Update( const UpdateInfo& updateInfo ) const;
		void Render( Renderer& renderer ) const;

		Scene* GetActiveScene( ) const;

		// Rule of 5
		~SceneManager( );
		SceneManager( const SceneManager& other ) = delete;
		SceneManager( SceneManager&& other ) noexcept = delete;
		SceneManager& operator=( const SceneManager& other ) = delete;
		SceneManager& operator=( SceneManager&& other ) noexcept = delete;
	private:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
		Scene* m_pActiveScene;
	};
}
