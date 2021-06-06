#pragma once
#include "GameObject.h"

namespace dae
{
	class JsonObjectWrapper;
	class InitInfo;
	class UpdateInfo;
	class Renderer;

	class BaseComponent
	{
	public:
		BaseComponent( GameObject& gameObject, std::string name );

		virtual void FixedUpdate( const UpdateInfo& );
		virtual void PersistentFixedUpdate( const UpdateInfo& );
		virtual void Update( const UpdateInfo& );
		virtual void PersistentUpdate( const UpdateInfo& );
		virtual void Draw( Renderer& );
		virtual void Init( const InitInfo& );
		virtual void Deactivate( );
		virtual void Activate( );
		virtual void Pause( );
		virtual void UnPause( );

		GameObject& GetGameObject( ) const;

		const std::string& GetName( ) const;
		const UUID<BaseComponent> Id;

		virtual ~BaseComponent( ) = default;
		BaseComponent( const BaseComponent& other ) = delete;
		BaseComponent( BaseComponent&& other ) noexcept = delete;
		BaseComponent& operator=( const BaseComponent& other ) = delete;
		BaseComponent& operator=( BaseComponent&& other ) noexcept = delete;
	protected:
		std::reference_wrapper<GameObject> m_GameObject;
		std::string m_Name;
	};
}
