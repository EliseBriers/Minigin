#include "MiniginPCH.h"
#include "Logger.h"

void dae::Logger::LogInfo( const std::string& msg )
{
	Log( "Info: " + msg, LogColor::Info );
}

void dae::Logger::LogWarning( const std::string& msg )
{
	Log( "Warning: " + msg, LogColor::Warning );
}

void dae::Logger::LogError( const std::string& msg )
{
	Log( "Error: " + msg, LogColor::Error );
}

void dae::Logger::Log( const std::string& msg, LogColor c )
{
	const HANDLE hConsole{ GetStdHandle( STD_OUTPUT_HANDLE ) };
	SetConsoleTextAttribute( hConsole, static_cast<WORD>(c) );
	std::cout << msg << '\n';
	SetConsoleTextAttribute( hConsole, static_cast<WORD>(LogColor::Info) );
}
