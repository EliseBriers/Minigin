#include "GamePCH.h"
#include "MathHelpers.h"
#include <glm/glm.hpp>

bool MathHelpers::AreCirclesOverlapping( const Circle& s0, const Circle& s1 )
{
	const float maxDistance{ s0.R + s1.R };
	const float distance{ glm::distance( s0.Pos, s1.Pos ) };

	return distance < maxDistance;
}

float MathHelpers::Pow2( float f )
{
	return f * f;
}
