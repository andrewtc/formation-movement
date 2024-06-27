namespace atc
{
	// ------------------------------ Vector Inline Methods ------------------------------

	inline Vector::Vector() :
		x( 0.0f ), y( 0.0f )
	{ }


	inline Vector::Vector( const Direction& direction ) :
		x( direction.m_x ), y( direction.m_y )
	{ }


	inline Vector::Vector( float x, float y ) :
		x( x ), y( y )
	{ }


	inline Vector Vector::operator+( const Vector& other ) const
	{
		return Vector( x + other.x, y + other.y );
	}


	inline Vector Vector::operator-( const Vector& other ) const
	{
		return Vector( x - other.x, y - other.y );
	}


	inline Vector Vector::operator*( float scalar ) const
	{
		return Vector( x * scalar, y * scalar );
	}


	inline Vector Vector::operator/( float scalar ) const
	{
		return Vector( x / scalar, y / scalar );
	}


	inline void Vector::operator+=( const Vector& other )
	{
		x += other.x;
		y += other.y;
	}


	inline void Vector::operator-=( const Vector& other )
	{
		x -= other.x;
		y -= other.y;
	}


	inline void Vector::operator*=( float scalar )
	{
		x *= scalar;
		y *= scalar;
	}


	inline void Vector::operator/=( float scalar )
	{
		x /= scalar;
		y /= scalar;
	}


	inline float Vector::getLength() const
	{
		return std::sqrtf( getLengthSquared() );
	}


	inline float Vector::getLengthSquared() const
	{
		return ( ( x * x ) + ( y * y ) );
	}


	// ------------------------------ Direction Inline Methods ------------------------------

	inline Direction::Direction() :
		m_x( 1.0f ), m_y( 0.0f )
	{ }


	inline Direction::Direction( float x, float y )
	{
		float length = std::sqrtf( ( x * x ) + ( y * y ) );
		requires( length > 0.0f );

		m_x = ( x / length );
		m_y = ( y / length );
	}


	inline Direction::Direction( const Vector& vector )
	{
		float length = vector.getLength();
		requires( length > 0.0f );

		m_x = ( vector.x / length );
		m_y = ( vector.y / length );
	}


	inline Vector Direction::operator*( float scalar ) const
	{
		return Vector( m_x * scalar, m_y * scalar );
	}


	inline Direction Direction::getPerpendicular() const
	{
		return Direction( m_y, -m_x );
	}


	inline float Direction::getX() const
	{
		return m_x;
	}


	inline float Direction::getY() const
	{
		return m_y;
	}


	// ------------------------------ Common Operators ------------------------------

	inline Vector operator*( float scalar, const Vector& vector )
	{
		return ( vector * scalar );
	}


	inline Vector operator*( float scalar, const Direction& direction )
	{
		return ( direction * scalar );
	}
}