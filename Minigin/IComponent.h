#pragma once
#include "GameObject.h"


namespace dae
{
	class InitInfo;
	class Renderer;

	class IComponent
	{
	public:
		IComponent( GameObject& gameObject );

		virtual void FixedUpdate( float );
		virtual void Update( float );
		virtual void Draw( Renderer& );
		virtual void Init( const InitInfo& );

		virtual ~IComponent( ) = default;
		IComponent( const IComponent& other ) = delete;
		IComponent( IComponent&& other ) noexcept = delete;
		IComponent& operator=( const IComponent& other ) = delete;
		IComponent& operator=( IComponent&& other ) noexcept = delete;
	protected:
		std::reference_wrapper<GameObject> m_GameObject;
	};
}
