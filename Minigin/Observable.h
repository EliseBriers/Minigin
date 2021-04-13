#pragma once
#include <vector>
#include <memory>
#include <functional>

namespace dae
{
	class Observable
	{
	public:
		void AddObserver( const std::function<void( )>& observer );
		void Notify( ) const;
	private:
		std::vector<std::function<void( )>> m_Observers;;
	};

	inline void Observable::AddObserver( const std::function<void( )>& observer )
	{
		m_Observers.push_back( observer );
	}

	inline void Observable::Notify( ) const
	{
		for( const std::function<void( )>& observer : m_Observers )
		{
			observer( );
		}
	}
}
