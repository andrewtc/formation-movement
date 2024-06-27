namespace atc
{
	inline void Path::clear()
	{
		m_waypoints.clear();
	}


	inline void Path::pushWaypoint( const Point& waypoint )
	{
		m_waypoints.push_back( waypoint );
	}


	inline Point Path::popNextWaypoint()
	{
		Point result = m_waypoints.back();
		m_waypoints.pop_back();
		return result;
	}


	inline Point Path::getWaypoint( int index ) const
	{
		int i = ( m_waypoints.size() - index - 1 );
		return m_waypoints[ i ];
	}


	inline Point Path::getNextWaypoint() const
	{
		return m_waypoints.back();
	}


	inline Point Path::getDestination() const
	{
		return m_waypoints.front();
	}


	inline bool Path::isValid() const
	{
		return ( m_waypoints.size() > 0 );
	}


	inline Unit* Path::getUnit() const
	{
		return m_unit;
	}
}