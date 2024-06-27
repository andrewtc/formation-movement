#include "common.h"
#include "Formation.h"


namespace atc
{
	const Color Formation::COLORS_BY_INDEX[ Formation::COLOR_COUNT ] =
	{
		Color::createFromValues( 1.0f, 0.0f, 0.0f ),
		Color::createFromValues( 1.0f, 1.0f, 0.0f ),
		Color::createFromValues( 0.0f, 1.0f, 0.0f ),
		Color::createFromValues( 0.0f, 1.0f, 1.0f ),
		Color::createFromValues( 0.0f, 0.0f, 1.0f ),
		Color::createFromValues( 1.0f, 0.0f, 1.0f )
	};


	const float Formation::ORIGIN_POINT_DRAW_RADIUS = 0.25f;
	const float Formation::SLOT_DRAW_RADIUS = 0.5f;
	const float Formation::CENTER_OF_MASS_DRAW_RADIUS = 0.25f;
	const float Formation::FACING_ARROW_LENGTH = 0.5f;
	const float Formation::FACING_ARROW_OFFSET = 0.5f;


	Formation::Slot::Slot( const Vector& offset ) :
		unit( nullptr ), offset( offset )
	{ }


	Formation::Slot::Slot( float offsetX, float offsetY ) :
		unit( nullptr ), offset( offsetX, offsetY )
	{ }


	Formation::Formation( World* world, int index, const Point& origin, const Point& destination, FormationBehavior* behavior, Color color ) :
		m_index( index ),
		m_color( color ),
		m_wasModified( false ),
		m_origin( origin ),
		m_destination( destination ),
		m_world( world ),
		m_flowfield( nullptr )
	{
		// Calculate the facing angle for the Formation.
		Vector toDestination = ( destination - origin );
		Direction facing( Angle::EAST );

		if( toDestination.getLengthSquared() > 0.0f )
		{
			facing = Direction( toDestination );
		}

		m_facingAngle = facing;

		// Set the behavior for this Formation.
		setBehavior( behavior );

		// Create a flowfield toward the goal of the Formation.
		m_flowfield = m_world->getMap()->createFlowfield();

		Map::TileVector goalPosition = m_world->worldToTileCoords( destination );
		Flowfield::Tile goalTile = m_flowfield->getTile( goalPosition.x, goalPosition.y );
		m_flowfield->setGoalTile( goalTile );
		m_flowfield->recalculate();
	}


	Formation::~Formation()
	{
		if( hasFlowfield() )
		{
			// If this Formation has a Flowfield, destroy it.
			m_flowfield->destroy();
			m_flowfield = nullptr;
		}
	}


	void Formation::update( double elapsedTime )
	{
		// Find the movement speed of the slowest unit.
		float maximumMoveSpeed = m_units.calculateMaximumMoveSpeed();

		// Scale the movement speed based on cohesion of Units in the Formation.
		float moveSpeed = ( maximumMoveSpeed * calculateCohesion() );

		// Calculate the direction in which to move the Formation.
		Vector toGoal = ( m_destination - m_origin );

		Map* map = m_world->getMap();
		Map::TileVector tilePos = m_world->worldToTileCoords( m_origin );
		Map::Tile mapTile = map->getTile( tilePos );

		if( mapTile.isValid() && mapTile->isPassable() &&
			!m_world->traceIsPassable( m_origin, m_destination, Unit::TRACE_RADIUS ) )
		{
			// Get the best adjacency from this location.
			Flowfield::Tile flowfieldTile = m_flowfield->getTile( tilePos.x, tilePos.y );

			if( !flowfieldTile->isGoal() )
			{
				CardinalDirection bestAdjacency = flowfieldTile->getBestAdjacency();
				Map::Tile adjacent = mapTile.getAdjacentTile( bestAdjacency );

				// Move toward the adjacent tile.
				Point adjacentPos = m_world->tileToWorldCoords( adjacent.getPosition() );
				toGoal = ( adjacentPos - m_origin );
			}
		}

		float distanceToGoal = toGoal.getLength();

		if( distanceToGoal > 0 )
		{
			// Normalize the direction vector.
			Vector directionToGoal = ( toGoal / distanceToGoal );

			float displacement = std::min( moveSpeed * (float) elapsedTime, distanceToGoal );

			// Move the Formation toward the goal.
			m_origin += ( directionToGoal * displacement );

			// Set the orientation of the Formation.
			m_facingAngle = Angle( Direction( directionToGoal ) );
		}

		if( m_wasModified )
		{
			// If the Formation was modified (i.e. a Unit was added or removed),
			// recalculate the slots for each unit.
			recalculate();
			m_wasModified = false;
		}
		else
		{
			bool needsReassign = false;

			for( int i = 0; i < m_units.getUnitCount(); ++i )
			{
				// Check each unit to see if it needs to be assigned to or evicted from a slot.
				Unit* unit = m_units.getUnitByIndex( i );

				// Calculate the distance of the Unit to the Formation.
				float distanceSquaredToFormation = unit->getDistanceSquaredToFormation();

				// Calculate the assignment distance.
				float assignUnitDistanceSquared = m_behavior->getAssignUnitToSlotDistanceSquared();

				if( !unit->hasFormationSlot() )
				{
					if( distanceSquaredToFormation <= assignUnitDistanceSquared )
					{
						needsReassign = true;
					}
				}
				else
				{
					if( distanceSquaredToFormation > assignUnitDistanceSquared )
					{
						needsReassign = true;
					}
				}
			}

			if( needsReassign )
			{
				// Reassign slots, as necessary.
				reassignSlots();
			}
		}
	}


	void Formation::draw( Renderer* renderer )
	{
		// Draw from the center of the Formation.
		renderer->pushTransform( Vector( m_origin.x, m_origin.y ) );

		// Draw everything in the Formation color.
		renderer->setColor( m_color );

		// Draw the facing arrow.
		Direction facing = getFacingAngle();
		Vector start = ( FACING_ARROW_OFFSET * facing );
		Vector end = start + ( FACING_ARROW_LENGTH * facing );
		renderer->drawLine( start, end );

		// Draw the origin point of the Formation.
		Point north( 0.0f, ORIGIN_POINT_DRAW_RADIUS );
		Point south( 0.0f, -ORIGIN_POINT_DRAW_RADIUS );
		Point west( -ORIGIN_POINT_DRAW_RADIUS, 0.0f );
		Point east( ORIGIN_POINT_DRAW_RADIUS, 0.0f );

		renderer->drawLine( north, south );
		renderer->drawLine( east, west );

		// Draw each slot in the Formation.
		for( int i = 0, slotCount = (int) m_slots.size(); i < slotCount; ++i )
		{
			renderer->pushTransform( getSlotOffset( i ) );
			renderer->drawCircle( SLOT_DRAW_RADIUS, SLOT_DRAW_SUBDIVISIONS );
			renderer->popTransform();
		}

		renderer->popTransform();

		// Draw the center of mass.
		renderer->pushTransform( m_units.calculateCenterOfMass() );
		renderer->drawCircle( CENTER_OF_MASS_DRAW_RADIUS, CENTER_OF_MASS_DRAW_SUBDIVISIONS );
		renderer->popTransform();
	}


	void Formation::addUnit( Unit* unit )
	{
		if( !m_units.containsUnit( unit ) )
		{
			requires( !unit->hasFormation() );

			// Clear the Unit's current move orders.
			unit->clearCurrentPath();

			// Add the Unit to the list of units in this formation.
			m_units.addUnit( unit );

			// Notify the Unit that it has been assigned to this formation.
			unit->setFormation( this );

			// Mark that the formation has been modified.
			m_wasModified = true;
		}
	}


	void Formation::removeUnit( Unit* unit )
	{
		if( m_units.containsUnit( unit ) )
		{
			// Remove the unit from the list of units in this formation.
			m_units.removeUnit( unit );

			// Remove the Unit from its slot (if any).
			if( unit->hasFormationSlot() )
			{
				evictUnitFromSlot( unit->getFormationSlotIndex() );
			}

			// Notify the unit that it has been removed from this formation.
			unit->setFormation( nullptr );

			// Mark that the formation has been modified.
			m_wasModified = true;
		}
	}


	void Formation::removeAllUnits()
	{
		for( int i = m_units.getUnitCount() - 1; i >= 0; --i )
		{
			// Remove all units from the formation.
			removeUnit( m_units.getUnitByIndex( i ) );
		}
	}


	void Formation::addSlot( const Vector& offset )
	{
		m_slots.push_back( Slot( offset ) );
	}


	void Formation::addSlot( float offsetX, float offsetY )
	{
		m_slots.push_back( Slot( offsetX, offsetY ) );
	}


	void Formation::removeAllSlots()
	{
		// Evict all units from all slots.
		evictAllUnits();

		// Clear all slots.
		m_slots.clear();
	}


	int Formation::getFirstFreeSlotIndex() const
	{
		int result = -1;

		for( size_t i = 0; i < m_slots.size(); ++i )
		{
			if( m_slots[ i ].isFree() )
			{
				result = (int) i;
				break;
			}
		}

		return result;
	}


	Point Formation::getSlotOffset( int index ) const
	{
		// Make sure the slot index is valid.
		requires( isValidSlotIndex( index ) );

		// Get the slot at the specified index.
		const Slot& slot = m_slots[ index ];

		// Rotate the offset by the facing angle.
		Direction facing = getFacing();
		Direction perpendicular = facing.getPerpendicular();
		Vector rotatedOffset = ( ( slot.offset.x * perpendicular ) + ( slot.offset.y * facing ) );

		// Return the transformed slot offset.
		return rotatedOffset;
	}


	Point Formation::getSlotWorldLocation( int index ) const
	{
		// Return the slot location transformed by the Formation origin.
		return ( m_origin + getSlotOffset( index ) );
	}


	Point Formation::getRelativePosition( const Point& worldPosition ) const
	{
		// Get the displacement from the origin point of the Formation to the position.
		Point displacement = ( worldPosition - m_origin );

		// Get the facing and perpendicular vectors for the formation.
		Direction facing = getFacing();
		Direction perpendicular = facing.getPerpendicular();

		// Project the position into a coordinate system relative to the Formation's
		// origin point and orientation.
		return Point( Vector::dot( displacement, perpendicular ), Vector::dot( displacement, facing ) );
	}


	float Formation::calculateCohesion() const
	{
		// Removing this because it doesn't look too good. :(
		// TODO: Make this optional.
		return 1.0f;

		float cohesion = 0.0f;
		size_t passableSlotCount = 0;
		size_t cohesiveUnitCount = 0;

		if( getUnitCount() > 0 )
		{
			for( int slotIndex = 0; slotIndex < m_slots.size(); ++slotIndex )
			{
				if( slotIsTaken( slotIndex ) )
				{
					// Get the slot position of the Unit.
					Point slotWorldLocation = getSlotWorldLocation( slotIndex );
					Map::ConstTile tile = m_world->getMapTileAtPosition( slotWorldLocation );

					if( tile.isValid() && tile->isPassable() )
					{
						// Only count slots over passable tiles toward the cohesion total.
						++passableSlotCount;
					}
				}
			}

			for( int index = 0; index < m_units.getUnitCount(); ++index )
			{
				// Calculate a cohesion value based on the distance squared of each Unit
				// from its Formation slot.
				Unit* unit = m_units.getUnitByIndex( index );

				if( unit->hasFormationSlot() )
				{
					// Determine how far the Unit is from its slot.
					float distanceToSlot = unit->getDistanceToSlot();

					// Get the location of the Unit's slot.
					Point slotLocation = getSlotWorldLocation( unit->getFormationSlotIndex() );

					if( unit->isAtLocation( slotLocation ) )
					{
						// If the Unit is in formation (i.e. at the slot location), add to the
						// total number of cohesive Units.
						++cohesiveUnitCount;
					}
				}
			}

			if( passableSlotCount > 0 )
			{
				// Use only the minimum cohesion for all units.
				cohesion = ( (float) cohesiveUnitCount / passableSlotCount );
			}
			else
			{
				cohesion = 1.0f;
			}
		}

		return ( cohesion * cohesion );
	}


	void Formation::recalculate()
	{
		// Clear all slots in the Formation.
		removeAllSlots();

		if( m_behavior )
		{
			// Recalculate all slots for this Formation.
			m_behavior->recalculate();
		}
	}


	void Formation::reassignSlots()
	{
		evictAllUnits();

		if( m_behavior )
		{
			m_behavior->reassignSlots();
		}
	}


	void Formation::assignUnitToSlot( Unit* unit, int index )
	{
		requires( unit );
		requires( containsUnit( unit ) );
		requires( isValidSlotIndex( index ) );

		if( m_slots[ index ].isTaken() )
		{
			// If the slot is already taken, evict the previous unit.
			evictUnitFromSlot( index );
		}

		if( unit->hasFormationSlot() )
		{
			// If this unit has already been assigned to a slot, evict it.
			evictUnitFromSlot( unit->getFormationSlotIndex() );
		}

		// Assign the unit to the requested slot.
		m_slots[ index ].unit = unit;
		unit->setFormationSlotIndex( index );
		unit->onAssignedToSlot();
	}


	void Formation::evictUnitFromSlot( int index )
	{
		requires( isValidSlotIndex( index ) );

		// Evict the unit from the specified slot.
		Slot& slot = m_slots[ index ];
		Unit* unit = slot.unit;
		slot.unit = nullptr;

		if( unit )
		{
			unit->setFormationSlotIndex( -1 );
			unit->onEvictedFromSlot();
		}
	}


	void Formation::evictAllUnits()
	{
		for( size_t i = 0; i < m_slots.size(); ++i )
		{
			evictUnitFromSlot( (int) i );
		}
	}
}