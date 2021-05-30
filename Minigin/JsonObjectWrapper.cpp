#include "MiniginPCH.h"
#include "JsonObjectWrapper.h"
#include "Logger.h"
#include <istreamwrapper.h>
#include <fstream>
#include "ResourceManager.h"

dae::JsonObjectWrapper::JsonObjectWrapper( const rapidjson::Value::ConstObject& obj, std::string name, ResourceManager& resourceManager )
	: m_Object{ obj }
	, m_Name{ std::move( name ) }
	, m_ResourceManager{ resourceManager }
{
}

dae::JsonObjectWrapper::JsonObjectWrapper( const std::string& fileName, ResourceManager& resourceManager )
	: JsonObjectWrapper{ resourceManager.GetJsonConstObject( fileName ), fileName, resourceManager }
{
}

std::string dae::JsonObjectWrapper::GetString( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogWarning( "dae::JsonObjectWrapper::GetString > No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		return "";
	}

	if( !m_Object[idx.c_str( )].IsString( ) )
	{
		Logger::LogWarning( "dae::JsonObjectWrapper::GetString > Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain a string" );
		return "";
	}

	return { m_Object[idx.c_str( )].GetString( ) };
}

float dae::JsonObjectWrapper::GetFloat( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogWarning( "dae::JsonObjectWrapper::GetFloat > No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		return 0.f;
	}

	if( !m_Object[idx.c_str( )].IsNumber( ) )
	{
		Logger::LogWarning( "dae::JsonObjectWrapper::GetFloat > Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain a float" );
		return 0.f;
	}

	return m_Object[idx.c_str( )].GetFloat( );
}

uint32_t dae::JsonObjectWrapper::GetUint( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogWarning( "dae::JsonObjectWrapper::GetUint > No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		return 0u;
	}

	if( !m_Object[idx.c_str( )].IsUint( ) )
	{
		Logger::LogWarning( "dae::JsonObjectWrapper::GetUint > Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain a uint" );
	}

	return m_Object[idx.c_str( )].GetUint( );
}

dae::JsonObjectWrapper dae::JsonObjectWrapper::GetObjectWrapper( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogError( "dae::JsonObjectWrapper::GetObjectWrapper > No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		throw std::exception{ "Invalid Index" };
	}

	if( m_Object[idx.c_str( )].IsString( ) )
	{
		return { m_Object[idx.c_str( )].GetString( ), m_ResourceManager };
	}

	if( !m_Object[idx.c_str( )].IsObject( ) )
	{
		Logger::LogError( "dae::JsonObjectWrapper::GetObjectWrapper > Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain an object" );
		throw std::exception{ "Invalid Jason Object" };
	}

	return { m_Object[idx.c_str( )].GetObjectA( ), m_Name + '.' + idx, m_ResourceManager };
}

std::vector<dae::JsonObjectWrapper> dae::JsonObjectWrapper::GetObjectArray( const std::string& idx ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		Logger::LogWarning( "dae::JsonObjectWrapper::GetObjectArray > No key \"" + idx + "\" found in Jason object \"" + m_Name + "\"" );
		return { };
	}

	if( !m_Object[idx.c_str( )].IsArray( ) )
	{
		Logger::LogWarning( "dae::JsonObjectWrapper::GetObjectArray > Key \"" + idx + "\" in Jason object \"" + m_Name + "\" does not contain an array" );
		return { };
	}

	auto vec{ m_Object[idx.c_str( )].GetArray( ) };
	const std::string arrayName{ m_Name + '.' + idx };
	size_t i{ };
	std::vector<JsonObjectWrapper> objects{ };
	for( auto& obj : vec )
	{
		const std::string name{ arrayName + '[' + std::to_string( i++ ) + ']' };
		if( obj.IsString( ) )
		{
			objects.emplace_back( std::string{ obj.GetString( ) }, m_ResourceManager );
		}
		else if( obj.IsObject( ) )
		{
			objects.emplace_back( obj.GetObjectA( ), name, m_ResourceManager );
		}
		else
		{
			Logger::LogWarning( "Invalid object found in array \"" + arrayName + '"' );
		}
	}

	return objects;
}

glm::vec2 dae::JsonObjectWrapper::GetVec2( const std::string& idx ) const
{
	const JsonObjectWrapper vec2Json{ GetObjectWrapper( idx ) };
	const float x{ vec2Json.GetFloat( "x" ) };
	const float y{ vec2Json.GetFloat( "y" ) };
	return { x, y };
}
