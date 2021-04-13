#pragma once

namespace dae
{
	class InitInfo;
	class IComponent;

	class ResourceManager;
	class Renderer;
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject( const std::string& name );
		GameObject( std::string&& name );
		void AddComponent( std::unique_ptr<IComponent> pComponent );

		void Draw( Renderer& renderer );
		void FixedUpdate( float dt );
		void Update( float dt );
		void Init( InitInfo& initInfo );

		template <typename T>
		T* GetComponent( );
		const std::string& GetName( ) const;

		GameObject( ) = default;
		~GameObject( );
		GameObject( const GameObject& other ) = delete;
		GameObject( GameObject&& other ) = delete;
		GameObject& operator=( const GameObject& other ) = delete;
		GameObject& operator=( GameObject&& other ) = delete;

	private:
		std::vector<std::unique_ptr<IComponent>> m_pComponents;
		std::string m_Name;
	};

	template <typename T>
	T* GameObject::GetComponent( )
	{
		for( const std::unique_ptr<IComponent>& pComponent : m_pComponents )
		{
			T* pReturnVal{ dynamic_cast<T*>(pComponent.get( )) };
			if( pReturnVal )
				return pReturnVal;
		}
		return nullptr;
	}
}
