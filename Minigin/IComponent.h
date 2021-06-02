#pragma once
#include "GameObject.h"

namespace dae
{
	class InitInfo;
	class UpdateInfo;
	class Renderer;

	class IComponent
	{
	public:
		IComponent( GameObject& gameObject );

		virtual void FixedUpdate( const UpdateInfo& );
		virtual void Update( const UpdateInfo& );
		virtual void Draw( Renderer& );
		virtual void Init( const InitInfo& );

		GameObject& GetGameObject( ) const;

		virtual ~IComponent( ) = default;
		IComponent( const IComponent& other ) = delete;
		IComponent( IComponent&& other ) noexcept = delete;
		IComponent& operator=( const IComponent& other ) = delete;
		IComponent& operator=( IComponent&& other ) noexcept = delete;
	protected:
		std::reference_wrapper<GameObject> m_GameObject;
	};
}
