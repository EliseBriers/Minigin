#pragma once
#include "Enums.h"

namespace EnumHelpers
{
	LevelColor StringToLevelColor(const std::string& str);
	HopperType StringToHopperType(const std::string& str);
	size_t GetHopperSpriteIndex( HopperType type, SpriteDirection direction, SpriteState state );
}
