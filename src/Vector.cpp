#include "common.h"
#include "Vector.h"

using namespace atc;


const Vector Vector::ZERO( 0.0f, 0.0f );


float Vector::dot( const Vector& first, const Vector& second )
{
	return ( ( first.x * second.x ) + ( first.y * second.y ) );
}


Vector Vector::clamp( const Vector& vector, float maxLength )
{
	requires( maxLength >= 0.0f );

	Vector result = vector;

	// Get the length of the vector.
	float length = vector.getLength();

	if( length > maxLength )
	{
		result = ( ( vector / length ) * maxLength );
	}

	return result;
}


float Vector::normalize()
{
	float lengthSquared = getLengthSquared();

	if( lengthSquared > 0.0f )
	{
		float length = sqrt( lengthSquared );
		float invLength = ( 1.0f / length );
		x *= invLength;
		y *= invLength;

		return length;
	}

	return 0.0f;
}


Direction::Direction( const Angle& facingAngle ) :
	m_x( cosf( facingAngle.toRadians() ) ),
	m_y( sinf( facingAngle.toRadians() ) )
{ }


Direction Direction::rotate( const Direction& direction, const Angle& angle )
{
	float radians = angle.toRadians();
	return Direction( ( direction.m_x * sinf( radians ) ) + ( direction.m_y * cosf( radians ) ),
					  ( direction.m_y * cosf( radians ) ) - ( direction.m_x * sinf( radians ) ) );
}


void Direction::rotate( const Angle& angle )
{
	float radians = angle.toRadians();
	m_x = ( m_x * sinf( radians ) ) + ( m_y * cosf( radians ) );
	m_y = ( m_y * cosf( radians ) ) - ( m_x * sinf( radians ) );
}