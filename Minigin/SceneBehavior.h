#pragma once

namespace dae
{
	class Renderer;
	class UpdateInfo;
	class InitInfo;
	class Scene;
}

namespace dae
{
	class SceneBehavior
	{
	public:
		SceneBehavior( );

		virtual void Draw( Renderer& );
		virtual void FixedUpdate( const UpdateInfo& );
		virtual void Update( const UpdateInfo& );
		virtual void Init( InitInfo& );
		void SetScene( Scene* pScene );

		// Rule of 5
		virtual ~SceneBehavior( ) = default;
		SceneBehavior( const SceneBehavior& other ) = delete;
		SceneBehavior( SceneBehavior&& other ) noexcept = delete;
		SceneBehavior& operator=( const SceneBehavior& other ) = delete;
		SceneBehavior& operator=( SceneBehavior&& other ) noexcept = delete;
	protected:
		Scene& GetScene( ) const;
	private:
		Scene* m_pScene;
	};
}
