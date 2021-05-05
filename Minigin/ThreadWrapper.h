#pragma once
#include <thread>
#include <functional>

namespace dae
{
	class ThreadWrapper
	{
	public:
		ThreadWrapper( std::function<void( )> func );

		~ThreadWrapper( );
		ThreadWrapper( const ThreadWrapper& other ) = delete;
		ThreadWrapper( ThreadWrapper&& other ) noexcept = delete;
		ThreadWrapper& operator=( const ThreadWrapper& other ) = delete;
		ThreadWrapper& operator=( ThreadWrapper&& other ) noexcept = delete;
	private:
		std::thread m_Thread;
	};
}
