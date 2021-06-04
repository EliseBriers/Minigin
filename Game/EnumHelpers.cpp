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

HopperType EnumHelpers::StringToHopperType( const std::string& str )
{
	if( str == "qbert_1" )
		return HopperType::Qbert1;
	if( str == "qbert_2" )
		return HopperType::Qbert2;
	if( str == "coily" )
		return HopperType::Coily;
	if( str == "ugg" )
		return HopperType::Ugg;
	if( str == "wrongway" )
		return HopperType::Wrongway;
	if( str == "slick" )
		return HopperType::Slick;
	if( str == "sam" )
		return HopperType::Sam;
	dae::Logger::LogWarning( "EnumHelpers::StringToHopperType > invalid string \"" + str + '"' );
	return HopperType::Qbert1;
}

size_t EnumHelpers::GetHopperSpriteIndex( HopperType type, SpriteDirection direction, SpriteState state )
{
	// What index of this hopper are we on?
	const size_t localIndex{ static_cast<size_t>(direction) * 2u + static_cast<size_t>(state) };
	const size_t hopperSize{ 8u };
	// Where does this hopper sprite start?
	const size_t hopperStartIndex{ static_cast<size_t>(type) * hopperSize };
	return hopperStartIndex + localIndex;
}

MoveDirection EnumHelpers::GetRandomMoveDirection( RandomBounceDirection randomBounceDirection )
{
	const int rnd{ std::rand( ) % 2 };
	switch( randomBounceDirection )
	{
	case RandomBounceDirection::Down:
		return rnd == 0 ? MoveDirection::Down : MoveDirection::Right;
	case RandomBounceDirection::UpLeft:
		return rnd == 0 ? MoveDirection::Down : MoveDirection::Left;
	case RandomBounceDirection::UpRight:
		return rnd == 0 ? MoveDirection::Up : MoveDirection::Right;
	}
	dae::Logger::LogWarning( "EnumHelpers::GetRandomMoveDirection > invalid randomBounceDirection" );
	return MoveDirection::Down;
}
