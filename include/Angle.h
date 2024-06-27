#ifndef ATC_ANGLE_H
#define ATC_ANGLE_H

#include "Math.h"

#include "Vector.h"

namespace atc
{
	/**
	 * Represents some amount of rotation about the Z axis.
	 */
	class Rotation
	{
	public:
		static const Rotation NO_REVOLUTION;
		static const Rotation QUARTER_REVOLUTION;
		static const Rotation HALF_REVOLUTION;
		static const Rotation FULL_REVOLUTION;

		static Rotation createFromRadians( float radians );
		static Rotation createFromDegrees( float degrees );

		Rotation();

		float toRadians() const;
		float toDegrees() const;

		Rotation operator+( Rotation other ) const;
		Rotation operator-( Rotation other ) const;
		Rotation operator*( float scalar ) const;
		Rotation operator/( float scalar ) const;

		void operator+=( Rotation other );
		void operator-=( Rotation other );

	private:
		Rotation( float radians );

		float m_radians;
	};


	/**
	 * Helper class for dealing with angles.
	 */
	class Angle
	{
	public:
		Angle( Rotation rotation );
		Angle( Direction direction );

		static const float PI;
		static const float PI_OVER_2;
		static const float PI_OVER_4;
		static const float TWO_PI;

		static const float DEGREE_TO_RADIAN_RATIO;
		static const float RADIAN_TO_DEGREE_RATIO;

		static const Angle EAST;
		static const Angle NORTH;
		static const Angle WEST;
		static const Angle SOUTH;

		static Angle createFromRadians( float radians );
		static Angle createFromDegrees( float degrees );

		Angle();

		float toRadians() const;
		float toDegrees() const;

		Angle operator+( Rotation rotation ) const;
		Angle operator-( Rotation rotation ) const;
		Rotation operator-( Angle other ) const;

		void operator+=( Rotation rotation );
		void operator-=( Rotation rotation );

	private:
		static float normalize( float radians );

		Angle( float radians );

		float m_radians;
	};
}

#endif