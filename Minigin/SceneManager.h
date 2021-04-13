#pragma once

namespace dae
{
	class Renderer;
	class Scene;

	class SceneManager final
	{
	public:
		SceneManager( ) = default;
		Scene& CreateScene( const std::string& name );

		void FixedUpdate( float dt );
		void Update( float dt );
		void Render( Renderer& renderer );

		~SceneManager( );
	private:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
