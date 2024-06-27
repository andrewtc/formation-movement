namespace atc
{
	inline Actor* World::getActorByID( Actor::ID id ) const
	{
		Actor* result = nullptr;
		auto it = m_actorsByID.find( id );

		if( it != m_actorsByID.end() )
		{
			result = ( it->second );
		}

		return result;
	}


	inline void World::setCamera( Camera* camera )
	{
		// Make sure the camera is valid and already in the World.
		requires( camera );
		requires( camera->getWorld() == this );

		// Set the camera and apply its projection matrix.
		m_camera = camera;
	}


	inline Camera* World::getCamera() const
	{
		return m_camera;
	}


	inline UnitSelection& World::getUnitSelection()
	{
		return m_unitSelection;
	}


	inline void World::setUnitSelection( const UnitSelection& selection )
	{
		m_unitSelection = selection;
	}


	inline const UnitSelection& World::getUnitSelection() const
	{
		return m_unitSelection;
	}


	inline Map* World::getMap()
	{
		return &m_map;
	}


	inline const Map* World::getMap() const
	{
		return &m_map;
	}


	inline float World::getLeft() const
	{
		return m_map.getLeft();
	}


	inline float World::getRight() const
	{
		return m_map.getRight();
	}


	inline float World::getBottom() const
	{
		return m_map.getBottom();
	}


	inline float World::getTop() const
	{
		return m_map.getTop();
	}


	inline Map::TileVector World::worldToTileCoords( const Point& worldCoords ) const
	{
		// Get the lower bounds of the World.
		float left = getLeft();
		float bottom = getBottom();

		// Get the tile position.
		// TODO: Don't assume that one world unit corresponds to one tile.
		Map::TileVector result( (unsigned int) ( worldCoords.x - left ),
								(unsigned int) ( worldCoords.y - bottom ) );
		return result;
	}


	inline Point World::tileToWorldCoords( const Map::TileVector& tileCoords ) const
	{
		// Get the world position.
		// TODO: Don't assume that one world unit corresponds to one tile.
		return Point( (float) tileCoords.x, (float) tileCoords.y );
	}


	inline Point World::tileToWorldCoords( const Flowfield::TileVector& tileCoords ) const
	{
		// Get the world position.
		// TODO: Don't assume that one world unit corresponds to one tile.
		return Point( (float) tileCoords.x, (float) tileCoords.y );
	}


	inline Map::Tile World::getMapTileAtPosition( const Point& worldCoords )
	{
		return m_map.getTile( worldToTileCoords( worldCoords ) );
	}


	inline Map::ConstTile World::getMapTileAtPosition( const Point& worldCoords ) const
	{
		return m_map.getTile( worldToTileCoords( worldCoords ) );
	}


	inline Map::Tile World::getMapTileAtPosition( float x, float y )
	{
		return getMapTileAtPosition( Point( x, y ) );
	}


	inline Map::ConstTile World::getMapTileAtPosition( float x, float y ) const
	{
		return getMapTileAtPosition( Point( x, y ) );
	}


	inline Point World::getMapTileWorldPosition( Map::ConstTile tile ) const
	{
		return tileToWorldCoords( tile.getPosition() );
	}


	inline Flowfield::Tile World::getFlowfieldTileAtPosition( Flowfield* flowfield, const Point& worldCoords ) const
	{
		Map::TileVector tileCoords = worldToTileCoords( worldCoords );
		return flowfield->getTile( tileCoords.x, tileCoords.y );
	}


	inline Flowfield::ConstTile World::getFlowfieldTileAtPosition( const Flowfield* flowfield, const Point& worldCoords ) const
	{
		Map::TileVector tileCoords = worldToTileCoords( worldCoords );
		return flowfield->getTile( tileCoords.x, tileCoords.y );
	}


	inline Flowfield::Tile World::getFlowfieldTileAtPosition( Flowfield* flowfield, float x, float y ) const
	{
		return getFlowfieldTileAtPosition( flowfield, Point( x, y ) );
	}


	inline Flowfield::ConstTile World::getFlowfieldTileAtPosition( const Flowfield* flowfield, float x, float y ) const
	{
		return getFlowfieldTileAtPosition( flowfield, Point( x, y ) );
	}


	inline Point World::getFlowfieldTileWorldPosition( Flowfield::ConstTile tile ) const
	{
		return tileToWorldCoords( tile.getPosition() );
	}


	inline Direction World::cardinalDirectionToWorldDirection( CardinalDirection direction )
	{
		Direction result;

		switch( direction )
		{
		case CARDINAL_DIRECTION_EAST:
			result = Direction( Angle::EAST );
			break;

		case CARDINAL_DIRECTION_NORTH:
			result = Direction( Angle::NORTH );
			break;

		case CARDINAL_DIRECTION_SOUTH:
			result = Direction( Angle::SOUTH );
			break;

		case CARDINAL_DIRECTION_WEST:
			result = Direction( Angle::WEST );
			break;
		}

		return result;
	}


	inline void World::startTrace( const Point& origin )
	{
		m_isTracing = true;
		m_traceOrigin = origin;
		m_traceDestination = origin;
	}


	inline void World::endTrace()
	{
		m_isTracing = false;
	}


	inline bool World::isTracing() const
	{
		return m_isTracing;
	}
}