#pragma once
#include <vector>
#include <functional>

namespace dae
{
	template <typename T>
	class ObservableVariable
	{
	public:
		using observer_t = std::function<void( const T& )>;
		ObservableVariable( const T& var );
		ObservableVariable( );
		void AddObserver( observer_t observer );

		void Set( const T& var );
		const T& Get( ) const;
		bool Equals( const T& var ) const;
	private:
		void Notify( );

		std::vector<observer_t> m_Observers;
		T m_Var;
	};

	template <typename T>
	ObservableVariable<T>::ObservableVariable( const T& var )
		: m_Var{ var }
	{
	}

	template <typename T>
	ObservableVariable<T>::ObservableVariable( )
		: m_Var{ }
	{
	}

	template <typename T>
	void ObservableVariable<T>::AddObserver( observer_t observer )
	{
		m_Observers.emplace_back( std::move( observer ) );
	}

	template <typename T>
	void ObservableVariable<T>::Notify( )
	{
		for( observer_t& observer : m_Observers )
		{
			observer( m_Var );
		}
	}

	template <typename T>
	void ObservableVariable<T>::Set( const T& var )
	{
		const bool changed{ var != m_Var };
		m_Var = var;
		if( changed )
			Notify( );
	}

	template <typename T>
	const T& ObservableVariable<T>::Get( ) const
	{
		return m_Var;
	}

	template <typename T>
	bool ObservableVariable<T>::Equals( const T& var ) const
	{
		return var == m_Var;
	}
}
