#ifndef ATC_VECTOR_H
#define ATC_VECTOR_H

namespace atc
{
	class Angle;
	class Direction;


	/**
	 * Simple 2D vector class.
	 */
	struct Vector
	{
		static const Vector ZERO;

		static float dot( const Vector& first, const Vector& second );
		static Vector clamp( const Vector& vector, float maxLength );
		float normalize();

		Vector();
		Vector( const Direction& direction );
		Vector( float x, float y );

		Vector operator+( const Vector& other ) const;
		Vector operator-( const Vector& other ) const;
		Vector operator*( float scalar ) const;
		Vector operator/( float scalar ) const;

		void operator+=( const Vector& other );
		void operator-=( const Vector& other );
		void operator*=( float scalar );
		void operator/=( float scalar );

		float getLength() const;
		float getLengthSquared() const;

		float x, y;
	};

	typedef Vector Point;


	/**
	 * Simple 2D direction class.
	 */
	class Direction
	{
	public:
		Direction();
		Direction( float x, float y );
		Direction( const Vector& vector );
		Direction( const Angle& facingAngle );

		Vector operator*( float scalar ) const;

		Direction getPerpendicular() const;

		static Direction rotate( const Direction& direction, const Angle& angle );
		void rotate( const Angle& angle );

		float getX() const;
		float getY() const;

	private:
		float m_x, m_y;

		friend class Angle;
		friend struct Vector;
	};
}

#endif