#include "MiniginPCH.h"
#include "JsonObjectWrapper.h"
#include "Logger.h"

dae::JsonObjectWrapper::JsonObjectWrapper( const rapidjson::Value::Object& obj, std::string name )
	: m_Object{ obj }
	, m_Name{ std::move( name ) }
{
}

std::string dae::JsonObjectWrapper::GetString( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogWarning( "No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		return "";
	}

	if( !m_Object[idx.c_str( )].IsString( ) )
	{
		Logger::LogWarning( "Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain a string" );
		return "";
	}

	return { m_Object[idx.c_str( )].GetString( ) };
}

float dae::JsonObjectWrapper::GetFloat( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogWarning( "No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		return 0.f;
	}

	if( !m_Object[idx.c_str( )].IsFloat( ) )
	{
		Logger::LogWarning( "Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain a float" );
	}

	return m_Object[idx.c_str( )].GetFloat( );
}

uint32_t dae::JsonObjectWrapper::GetUint( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogWarning( "No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		return 0u;
	}

	if( !m_Object[idx.c_str( )].IsUint( ) )
	{
		Logger::LogWarning( "Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain a uint" );
	}

	return m_Object[idx.c_str( )].GetUint( );
}

dae::JsonObjectWrapper dae::JsonObjectWrapper::GetObjectWrapper( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogError( "No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		throw std::exception{ "Invalid Index" };
	}

	if( !m_Object[idx.c_str( )].IsObject( ) )
	{
		Logger::LogError( "Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain an object" );
		throw std::exception{ "Invalid Jason Object" };
	}

	return { m_Object[idx.c_str( )].GetObjectA( ), m_Name + '.' + idx };
}

rapidjson::Value::Array dae::JsonObjectWrapper::GetArray( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogError( "No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		throw std::exception{ "Invalid Index" };
	}

	if( !m_Object[idx.c_str( )].IsArray( ) )
	{
		Logger::LogError( "Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain an array" );
		throw std::exception{ "Invalid Jason Array" };
	}

	return m_Object[idx.c_str( )].GetArray( );
}

std::vector<dae::JsonObjectWrapper> dae::JsonObjectWrapper::GetObjectArray( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogWarning( "No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		return { };
	}

	if( !m_Object[idx.c_str( )].IsArray( ) )
	{
		Logger::LogWarning( "Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain an array" );
		return { };
	}

	auto vec{ m_Object[idx.c_str( )].GetArray( ) };
	const std::string arrayName{ m_Name + '.' + idx };
	size_t i{ };
	std::vector<JsonObjectWrapper> objects{ };
	for( auto& obj : vec )
	{
		const std::string name{ arrayName + '[' + std::to_string( ++i ) + ']' };
		if( !obj.IsObject( ) )
		{
			Logger::LogWarning( "Invalid object found in array \"" + arrayName + '"' );
			continue;
		}
		objects.emplace_back( obj.GetObjectA( ), name );
	}

	return objects;
}
