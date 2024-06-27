#include "common.h"
#include "Color.h"
#include "Color.inl"


namespace atc
{
	const Color Color::WHITE = Color( 255, 255, 255, 255 );
	const Color Color::BLACK = Color( 0, 0, 0, 255 );
	const Color Color::TRANSPARENT_WHITE = Color( 255, 255, 255, 0 );
	const Color Color::TRANSPARENT_BLACK = Color( 0, 0, 0, 0 );
}
