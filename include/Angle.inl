namespace atc
{
	// ------------------------------ Rotation Inline Methods ------------------------------

	inline Rotation Rotation::createFromRadians( float radians )
	{
		return Rotation( radians );
	}


	inline Rotation Rotation::createFromDegrees( float degrees )
	{
		return Rotation( Angle::DEGREE_TO_RADIAN_RATIO * degrees );
	}


	inline Rotation::Rotation() :
		m_radians( 0.0f )
	{ }


	inline Rotation::Rotation( float radians ) :
		m_radians( radians )
	{ }


	inline float Rotation::toRadians() const
	{
		return m_radians;
	}


	inline float Rotation::toDegrees() const
	{
		return ( Angle::RADIAN_TO_DEGREE_RATIO * m_radians );
	}


	inline Rotation Rotation::operator+( Rotation other ) const
	{
		return Rotation( m_radians + other.m_radians );
	}


	inline Rotation Rotation::operator-( Rotation other ) const
	{
		return Rotation( m_radians + other.m_radians );
	}


	inline Rotation Rotation::operator*( float scalar ) const
	{
		return Rotation( m_radians * scalar );
	}


	inline Rotation Rotation::operator/( float scalar ) const
	{
		return Rotation( m_radians / scalar );
	}


	inline void Rotation::operator+=( Rotation other )
	{
		m_radians += other.m_radians;
	}


	inline void Rotation::operator-=( Rotation other )
	{
		m_radians -= other.m_radians;
	}


	// ------------------------------ Angle Inline Methods ------------------------------

	inline Angle Angle::createFromRadians( float radians )
	{
		return Angle( radians );
	}


	inline Angle Angle::createFromDegrees( float degrees )
	{
		return Angle( DEGREE_TO_RADIAN_RATIO * degrees );
	}


	inline Angle::Angle() :
		m_radians( 0.0f )
	{ }


	inline Angle::Angle( float radians ) :
		m_radians( normalize( radians ) )
	{ }


	inline Angle::Angle( Rotation rotation ) :
		m_radians( Math::wrap( rotation.toRadians(), 0.0f, TWO_PI ) )
	{ }


	inline Angle::Angle( Direction direction ) :
		m_radians( atan2f( direction.m_y, direction.m_x ) )
	{ }


	inline float Angle::normalize( float radians )
	{
		return Math::wrap( radians, 0.0f, TWO_PI );
	}


	inline float Angle::toRadians() const
	{
		return m_radians;
	}


	inline float Angle::toDegrees() const
	{
		return ( RADIAN_TO_DEGREE_RATIO * m_radians );
	}
	

	inline Angle Angle::operator+( Rotation rotation ) const
	{
		return Angle( m_radians + rotation.toRadians() );
	}


	inline Angle Angle::operator-( Rotation rotation ) const
	{
		return Angle( m_radians - rotation.toRadians() );
	}


	inline void Angle::operator+=( Rotation rotation )
	{
		m_radians = normalize( m_radians + rotation.toRadians() );
	}


	inline void Angle::operator-=( Rotation rotation )
	{
		m_radians = normalize( m_radians - rotation.toRadians() );
	}


	// ------------------------------ Common Operators ------------------------------

	inline Rotation operator*( float scalar, Rotation rotation )
	{
		return ( rotation * scalar );
	}
}