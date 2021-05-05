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
		Scene& CreateScene( const std::string& name );

		void FixedUpdate( const UpdateInfo& updateInfo );
		void Update( const UpdateInfo& updateInfo );
		void Render( Renderer& renderer );

		~SceneManager( );
	private:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
