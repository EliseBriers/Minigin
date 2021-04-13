#pragma once
#include <unordered_map>
#include <functional>

template <typename T, typename ... Args>
class ManagerMap
{
	using allocator_t = std::function<std::unique_ptr<T>( Args ... )>;

	struct AllocatorObject
	{
		AllocatorObject( allocator_t& allocator, Args ... args )
			: pValue{ allocator( args... ) }
		{
		}

		std::unique_ptr<T> pValue;
	};

public:
	ManagerMap( const allocator_t& allocator );
	T& GetElement( const std::string& name, Args ... args );
private:
	std::unordered_map<std::string, AllocatorObject> m_Map;
	allocator_t m_Allocator;
};

template <typename T, typename ... Args>
ManagerMap<T, Args...>::ManagerMap( const allocator_t& allocator )
	: m_Allocator{ allocator }
{
}

template <typename T, typename ... Args>
T& ManagerMap<T, Args...>::GetElement( const std::string& name, Args ... args )
{
	return *m_Map.try_emplace( name, m_Allocator, args... ).first->second.pValue;

	// I over-designed an optimization that gets rid of a single hash,
	// in a function that doesn't run in the hotloop,
	// without doing any benchmarks.
	// This is how we're supposed to optimize, right?

	// Old code to explain why AllocatorObject is needed:
	// Without it I'd have to do a find and sometimes an emplace, which will calculate the hash twice
	
	//auto it{ m_Map.find( name ) };
	//if( it == m_Map.end( ) )
	//{
	//	return *m_Map.emplace( name, m_Allocator( args... ) ).first->second;
	//}
	//return *it->second;
}
