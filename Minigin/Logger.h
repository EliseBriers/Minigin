#pragma once

namespace dae
{
	namespace Logger
	{
		enum class LogColor : WORD
		{
			Error = 12,
			Warning = 14,
			Info = 15
		};

		void LogInfo( const std::string& msg );
		void LogWarning( const std::string& msg );
		void LogError( const std::string& msg );
		void Log( const std::string& msg, LogColor c );
	}
}
