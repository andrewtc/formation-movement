#include "common.h"
#include "Angle.h"

#include <cmath>

namespace atc
{
	const float Angle::PI = (float) ( 4.0 * atan( 1.0 ) );
	const float Angle::PI_OVER_2 = ( PI / 2.0f );
	const float Angle::PI_OVER_4 = ( PI / 4.0f );
	const float Angle::TWO_PI = ( 2.0f * PI );

	const float Angle::DEGREE_TO_RADIAN_RATIO = ( Angle::PI / 180.0f );
	const float Angle::RADIAN_TO_DEGREE_RATIO = ( 180.0f / Angle::PI );

	const Angle Angle::EAST( 0.0f );
	const Angle Angle::NORTH( PI_OVER_2 );
	const Angle Angle::WEST( PI );
	const Angle Angle::SOUTH( PI + PI_OVER_2 );

	const Rotation Rotation::NO_REVOLUTION( 0.0f );
	const Rotation Rotation::QUARTER_REVOLUTION( Angle::PI_OVER_2 );
	const Rotation Rotation::HALF_REVOLUTION( Angle::PI );
	const Rotation Rotation::FULL_REVOLUTION( Angle::TWO_PI );
}