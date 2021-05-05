#include "MiniginPCH.h"
#include "ThreadWrapper.h"

dae::ThreadWrapper::ThreadWrapper( std::function<void( )> func )
	: m_Thread{ func }
{
}

dae::ThreadWrapper::~ThreadWrapper( )
{
	m_Thread.join( );
}
