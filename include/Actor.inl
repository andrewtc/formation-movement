namespace atc
{
	inline bool Actor::isAlive() const
	{
		return m_isAlive;
	}


	inline bool Actor::isDead() const
	{
		return ( !isAlive() );
	}


	inline Actor::ID Actor::getID() const
	{
		return m_ID;
	}


	inline World* Actor::getWorld() const
	{
		return m_world;
	}


	inline void Actor::setPosition( const Point& position )
	{
		m_position = position;
	}


	inline Point Actor::getPosition() const
	{
		return m_position;
	}


	inline void Actor::setVelocity( const Vector& velocity )
	{
		m_velocity = velocity;
	}


	inline Vector Actor::getVelocity() const
	{
		return m_velocity;
	}


	inline bool Actor::isAtLocation( const Point& location ) const
	{
		return ( m_position - location ).getLengthSquared() <= POSITION_TOLERANCE_SQUARED;
	}


	inline bool Actor::isOverlappingLocation( const Point& location ) const
	{
		return ( m_position - location ).getLengthSquared() <= ( m_collisionRadius * m_collisionRadius );
	}


	inline void Actor::setCollisionRadius( float collisionRadius )
	{
		m_collisionRadius = collisionRadius;
	}


	inline float Actor::getCollisionRadius() const
	{
		return m_collisionRadius;
	}


	inline void Actor::setCollisionEnabled( bool collisionEnabled )
	{
		m_isCollisionEnabled = collisionEnabled;
	}


	inline bool Actor::isCollisionEnabled() const
	{
		return m_isCollisionEnabled;
	}
}