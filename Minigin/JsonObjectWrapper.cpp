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

std::string dae::JsonObjectWrapper::GetFullVarName( const std::string& idx ) const
{
	return m_Name + '.' + idx;
}

std::string dae::JsonObjectWrapper::GetString( const std::string& idx ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		WarnKeyNotFound( __func__, idx );
		return "";
	}

	if( !m_Object[idx.c_str( )].IsString( ) )
	{
		WarnInvalidObject( __func__, idx, "a string" );
		return "";
	}

	return { m_Object[idx.c_str( )].GetString( ) };
}

float dae::JsonObjectWrapper::GetFloat( const std::string& idx ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		WarnKeyNotFound( __func__, idx );
		return 0.f;
	}

	if( !m_Object[idx.c_str( )].IsNumber( ) )
	{
		WarnInvalidObject( __func__, idx, "a float" );
		return 0.f;
	}

	return m_Object[idx.c_str( )].GetFloat( );
}

float dae::JsonObjectWrapper::GetOptionalFloat( const std::string& idx, float defaultVal ) const
{
	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		return defaultVal;
	}

	if( !m_Object[idx.c_str( )].IsNumber( ) )
	{
		return defaultVal;
	}

	return m_Object[idx.c_str( )].GetFloat( );
}

uint32_t dae::JsonObjectWrapper::GetUint( const std::string& idx ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		WarnKeyNotFound( __func__, idx );
		return 0u;
	}

	if( !m_Object[idx.c_str( )].IsUint( ) )
	{
		WarnInvalidObject( __func__, idx, "a uint" );
		return 0u;
	}

	return m_Object[idx.c_str( )].GetUint( );
}

dae::JsonObjectWrapper dae::JsonObjectWrapper::GetObjectWrapper( const std::string& idx ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		ErrorKeyNotFound( __func__, idx );
		throw std::exception{ "Invalid Index" };
	}

	if( m_Object[idx.c_str( )].IsString( ) )
	{
		return { m_Object[idx.c_str( )].GetString( ), m_ResourceManager };
	}

	if( !m_Object[idx.c_str( )].IsObject( ) )
	{
		ErrorInvalidObject( __func__, idx, "an object or string" );
		throw std::exception{ "Invalid Jason Object" };
	}

	return { m_Object[idx.c_str( )].GetObjectA( ), m_Name + '.' + idx, m_ResourceManager };
}

std::vector<dae::JsonObjectWrapper> dae::JsonObjectWrapper::GetObjectArray( const std::string& idx ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	if( !m_Object.HasMember( idx.c_str( ) ) )
	{
		WarnKeyNotFound( __func__, idx );
		return { };
	}

	if( !m_Object[idx.c_str( )].IsArray( ) )
	{
		WarnInvalidObject( __func__, idx, "an array" );
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

void dae::JsonObjectWrapper::WarnInvalidObject( const std::string& currentFunction, const std::string& idx, const std::string& objectType ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	Logger::LogWarning( "dae::JsonObjectWrapper::" + currentFunction + " > " + fullVarName + " does not contain " + objectType );
}

void dae::JsonObjectWrapper::ErrorInvalidObject( const std::string& currentFunction, const std::string& idx, const std::string& objectType ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	Logger::LogError( "dae::JsonObjectWrapper::" + currentFunction + " > " + fullVarName + " does not contain " + objectType );
}

void dae::JsonObjectWrapper::WarnKeyNotFound( const std::string& currentFunction, const std::string& idx ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	Logger::LogWarning( "dae::JsonObjectWrapper::" + currentFunction + " > " + fullVarName + " not found" );
}

void dae::JsonObjectWrapper::ErrorKeyNotFound( const std::string& currentFunction, const std::string& idx ) const
{
	const std::string fullVarName{ GetFullVarName( idx ) };

	Logger::LogError( "dae::JsonObjectWrapper::" + currentFunction + " > " + fullVarName + " not found" );
}
