namespace atc
{
	// ------------------------------ MapTile ------------------------------

	inline MapTile::MapTile() :
		m_isPassable( true ),
		m_lastPathfindOpened( -1 ),
		m_lastPathfindClosed( -1 ),
		m_lastPathfindCost( 0.0f ),
		m_lastPathfindDirection( CARDINAL_DIRECTION_NONE )
	{ }


	inline MapTile::~MapTile() { }


	inline void MapTile::setPassable( bool isPassable )
	{
		m_isPassable = isPassable;
	}


	inline bool MapTile::isPassable() const
	{
		return m_isPassable;
	}


	inline void MapTile::open( int pathfindIndex )
	{
		m_lastPathfindOpened = pathfindIndex;
	}


	inline void MapTile::close( int pathfindIndex )
	{
		m_lastPathfindClosed = pathfindIndex;
	}


	inline bool MapTile::isOpen( int pathfindIndex ) const
	{
		return ( m_lastPathfindOpened == pathfindIndex );
	}


	inline bool MapTile::isClosed( int pathfindIndex ) const
	{
		return ( m_lastPathfindClosed == pathfindIndex );
	}


	inline void MapTile::setLastPathfindDirection( CardinalDirection direction )
	{
		m_lastPathfindDirection = direction;
	}


	inline CardinalDirection MapTile::getLastPathfindDirection() const
	{
		return m_lastPathfindDirection;
	}


	inline void MapTile::setLastPathfindCost( float cost )
	{
		m_lastPathfindCost = cost;
	}


	inline float MapTile::getLastPathfindCost() const
	{
		return m_lastPathfindCost;
	}


	// ------------------------------ Map ------------------------------

	inline float Map::getLeft() const
	{
		return -0.5f;
	}


	inline float Map::getRight() const
	{
		return ( getLeft() + m_width );
	}


	inline float Map::getBottom() const
	{
		return -0.5f;
	}


	inline float Map::getTop() const
	{
		return ( getBottom() + m_height );
	}
}