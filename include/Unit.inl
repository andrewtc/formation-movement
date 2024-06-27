namespace atc
{
	inline void Unit::setFacingAngle( Angle facingAngle )
	{
		m_facingAngle = facingAngle;
	}


	inline Angle Unit::getFacingAngle() const
	{
		return m_facingAngle;
	}


	inline void Unit::clearCurrentPath()
	{
		m_currentPath.clear();
	}


	inline void Unit::setTargetLocation( const Point& location )
	{
		m_targetLocation = location;
	}


	inline Point Unit::getTargetLocation() const
	{
		return m_targetLocation;
	}


	inline bool Unit::canMoveDirectlyTo( const Point& destination )
	{
		// Trace from the Unit to the destination and return whether the trace is passable.
		return getWorld()->traceIsPassable( m_position, destination, TRACE_RADIUS );
	}


	inline bool Unit::isApproachingFormation() const
	{
		bool result = false;

		if( hasFormation() && hasFormationSlot() )
		{
			// Return true if the Unit is within a certain range of the Formation.
			result = ( getDistanceSquaredToFormation() <= m_formation->getAssignUnitToSlotDistanceSquared() );
		}

		return result;
	}


	inline void Unit::setCurrentPath( const Path& path )
	{
		requires( path.isValid() );
		m_currentPath = path;
	}


	inline Path Unit::getCurrentPath() const
	{
		return m_currentPath;
	}


	inline bool Unit::hasFormation() const
	{
		return ( m_formation != nullptr );
	}


	inline Formation* Unit::getFormation() const
	{
		return m_formation;
	}


	inline bool Unit::hasFormationSlot() const
	{
		return ( m_formationSlotIndex >= 0 );
	}


	inline int Unit::getFormationSlotIndex() const
	{
		return m_formationSlotIndex;
	}


	inline float Unit::getDistanceSquaredToFormation() const
	{
		requires( hasFormation() );

		// If this Unit has a slot, get the vector to it.
		Vector toFormation = ( m_position - m_formation->getOrigin() );

		// Find the distance squared to the slot.
		return toFormation.getLengthSquared();
	}


	inline float Unit::getDistanceToFormation() const
	{
		return sqrtf( getDistanceSquaredToFormation() );
	}


	inline float Unit::getDistanceSquaredToSlot() const
	{
		requires( hasFormation() && hasFormationSlot() );

		// If this Unit has a slot, get the vector to it.
		Vector toSlot = ( m_position - m_formation->getSlotWorldLocation( m_formationSlotIndex ) );

		// Find the distance squared to the slot.
		return toSlot.getLengthSquared();
	}


	inline float Unit::getDistanceToSlot() const
	{
		return sqrtf( getDistanceSquaredToSlot() );
	}


	inline void Unit::setFormationSlotIndex( int slotIndex )
	{
		m_formationSlotIndex = slotIndex;
	}


	inline Map::TileVector Unit::getTilePosition() const
	{
		return getWorld()->worldToTileCoords( m_position );
	}


	inline Map::Tile Unit::getCurrentTile() const
	{
		return getWorld()->getMapTileAtPosition( m_position );
	}
}