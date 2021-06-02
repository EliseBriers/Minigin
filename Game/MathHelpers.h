#pragma once

namespace MathHelpers
{
	struct Circle
	{
		glm::vec2 Pos;
		float R;
	};

	bool AreCirclesOverlapping( const Circle& s0, const Circle& s1 );
	float Pow2( float f );
}
