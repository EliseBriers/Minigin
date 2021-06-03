#include "GamePCH.h"
#include "EnumHelpers.h"

LevelColor EnumHelpers::StringToLevelColor( const std::string& str )
{
	if( str == "black" )
		return LevelColor::Black;
	if( str == "void" )
		return LevelColor::Void;
	if( str == "blue" )
		return LevelColor::Blue;
	if( str == "gray" )
		return LevelColor::Gray;
	if( str == "green" )
		return LevelColor::Green;
	if( str == "lime_blue" )
		return LevelColor::LimeBlue;
	if( str == "lime_orange" )
		return LevelColor::LimeOrange;
	if( str == "orange" )
		return LevelColor::Orange;
	if( str == "teal" )
		return LevelColor::Teal;
	dae::Logger::LogWarning( "EnumHelpers::StringToLevelColor > invalid string \"" + str + '"' );
	return LevelColor::Blue;
}
