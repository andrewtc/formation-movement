#include "common.h"
#include "Unit.h"

namespace atc
{
	const Color Unit::DEFAULT_COLOR = Color::createFromValues( 0.8f, 0.8f, 0.8f );
	const Color Unit::ARROW_COLOR = Color::createFromValues( 0.0f, 1.0f, 1.0f );
	const Color Unit::TARGET_COLOR = Color::createFromValues( 0.0f, 1.0f, 1.0f );
	const float Unit::COLLISION_RADIUS = 0.5f;
	const float Unit::COLLISION_IMPULSE = 10.0f;
	const float Unit::MOVEMENT_SPEED = 6.0f;
	const float Unit::TRACE_RADIUS = ( COLLISION_RADIUS - 0.05f );
	const float Unit::APPROACHING_FORMATION_BOOST_AMOUNT = 0.5f;


	Unit::Unit() :
		Actor( true ), // Enable collision.
		m_formation( nullptr ),
		m_formationSlotIndex( -1 ),
		m_currentPathRequestIndex( -1 )
	{
		setCollisionRadius( COLLISION_RADIUS );
	}


	Unit::~Unit()
	{
		if( hasFormation() )
		{
			// If this unit was part of a formation, remove it.
			m_formation->removeUnit( this );
		}
	}


	void Unit::onSpawn( const Point& position )
	{
		// Move toward the spawn location of the unit.
		setPosition( position );
		setTargetLocation( position );
	}


	void Unit::update( double elapsedTime )
	{
		if( hasFormation() )
		{
			if( hasFormationSlot() )
			{
				// If this unit has a formation slot, get the location of the slot.
				Point slotLocation = m_formation->getSlotWorldLocation( m_formationSlotIndex );
				Map::Tile slotTile = getWorld()->getMapTileAtPosition( slotLocation );

				if( canMoveDirectlyTo( slotLocation ) )
				{
					// If we're not at our intended slot and can move directly to the slot, move there.
					setTargetLocation( slotLocation );
				}
				else
				{
					// Otherwise, use the flowfield.
					updateTargetLocation();
				}
			}
			else
			{
				// Over several frames, find the farthest point on the Flowfield the Unit can
				// reach in a straight shot and head toward that location.
				updateTargetLocation();
			}
		}

		if( !isAtLocation( m_targetLocation ) )
		{
			// Determine whether the location is passable.
			Map::Tile tile = getCurrentTile();

			if( tile.isValid() && tile->isPassable() )
			{
				// Determine the direction in which to move.
				Vector toTargetLocation = ( m_targetLocation - m_position );

				if( toTargetLocation.getLengthSquared() > 0.0f )
				{
					// Normalize the movement direction.
					Direction moveDirection = Direction( toTargetLocation );
					float speed = MOVEMENT_SPEED;

					if( isApproachingFormation() )
					{
						// Give the Unit a speed boost if trying to get into formation.
						speed += ( MOVEMENT_SPEED * APPROACHING_FORMATION_BOOST_AMOUNT );
					}

					// Turn and move toward the target location.
					m_velocity = ( moveDirection * speed );
					m_facingAngle = Angle( moveDirection );
				}
				else
				{
					m_velocity = Vector::ZERO;
				}
			}
		}
		else
		{
			m_position = ( m_position * 0.5f ) + ( m_targetLocation * 0.5f );
		}

		// Update position.
		Actor::update( elapsedTime );

		// Collide with walls.
		//collideWithWalls();

		// Reset velocity.
		m_velocity = Vector::ZERO;
	}


	void Unit::updateTargetLocation()
	{
		// Get the current flowfield tile.
		Flowfield* flowfield = m_formation->getFlowfield();
		Flowfield::Tile currentFlowfieldTile = getWorld()->getFlowfieldTileAtPosition( flowfield, m_position );

		// Over several frames, trace out to the farthest tile that can be reached in a straight line.
		Flowfield::Tile currentTargetTile = getWorld()->getFlowfieldTileAtPosition( flowfield, m_targetLocation );

		if( ( currentTargetTile == currentFlowfieldTile ) || !canMoveDirectlyTo( m_targetLocation ) )
		{
			// Otherwise, start over at the best tile adjacent to this Unit's current Flowfield tile.
			currentTargetTile = currentFlowfieldTile.getAdjacentTile( currentFlowfieldTile->getBestAdjacency() );
		}

		for( int i = 0; i < MAX_OPTIMIZATION_STEPS && !currentTargetTile->isGoal() &&
			 Flowfield::TileVector::getManhattanDistance( currentFlowfieldTile.getPosition(), currentTargetTile.getPosition() ) <= MAX_SHORTCUT_LOOKAHEAD;
			 ++i )
		{
			// Determine the next tile.
			CardinalDirection nextTileDirection = currentTargetTile->getBestAdjacency();

			if( nextTileDirection != CARDINAL_DIRECTION_NONE )
			{
				Flowfield::Tile nextTile = currentTargetTile.getAdjacentTile( nextTileDirection );
				Point nextTilePosition = getWorld()->getFlowfieldTileWorldPosition( nextTile );

				if( canMoveDirectlyTo( nextTilePosition ) )
				{
					// If there is a straight-line path to the next tile, set it as the destination.
					currentTargetTile = nextTile;
				}
				else
				{
					break;
				}
			}
		}

		// Go toward the target tile.
		setTargetLocation( getWorld()->getFlowfieldTileWorldPosition( currentTargetTile ) );
	}


	void Unit::collideWithWalls()
	{
		Vector radiusOffset = Vector( m_collisionRadius, m_collisionRadius );

		Point minBounds = ( m_position - radiusOffset );
		Point maxBounds = ( m_position + radiusOffset );

		Map::TileVector minTile = getWorld()->worldToTileCoords( minBounds );
		Map::TileVector maxTile = getWorld()->worldToTileCoords( maxBounds );

		for( Map::TileOffset y = minTile.y; y <= maxTile.y; ++y )
		{
			for( Map::TileOffset x = minTile.x; x <= maxTile.x; ++x )
			{
				// Get each tile with which the Unit could be colliding.
				Map* map = getWorld()->getMap();
				Map::Tile tile = map->getTile( x, y );

				if( !tile.isValid() || !tile->isPassable() )
				{
					// If the tile is not passable, get the vector distance to it.
					Point tilePos( x, y );
					Vector displacement = ( m_position - tilePos );
					Vector absoluteDisplacement( std::fabs( displacement.x ), std::fabs( displacement.y ) );

					// Calculate the target distance from the tile that the Unit should be.
					float targetDistance = ( m_collisionRadius + 0.5f );

					// Move the unit the shortest distance possible to resolve the collision.
					if( absoluteDisplacement.x >= absoluteDisplacement.y )
					{
						float sign = ( displacement.x >= 0.0f ? 1.0f : -1.0f );
						m_position.x = ( tilePos.x + ( sign * targetDistance ) );
					}
					else
					{
						float sign = ( displacement.y >= 0.0f ? 1.0f : -1.0f );
						m_position.y = ( tilePos.y + ( sign * targetDistance ) );
					}
				}
			}
		}
	}


	void Unit::draw( Renderer* renderer )
	{
		// Draw the Unit's current path.
		//m_currentPath.draw( renderer );

		// Determine unit color.
		Color unitColor = ( m_formation ? m_formation->getColor() : DEFAULT_COLOR );

		// Draw the Unit's current target.
		renderer->setColor( TARGET_COLOR );
		renderer->drawLine( m_position, m_targetLocation );

		// Draw unit.
		renderer->pushTransform( Vector( m_position.x, m_position.y ), Rotation::createFromRadians( m_facingAngle.toRadians() ) );

		Point arrowIntersection( COLLISION_RADIUS, 0.0f );
		Point arrowTip( COLLISION_RADIUS * 1.5f, 0.0f );

		renderer->setColor( unitColor );
		renderer->fillCircle( m_collisionRadius, COLLISION_RADIUS_SUBDIVISIONS );

		renderer->setColor( Color::BLACK );
		renderer->drawLine( Point::ZERO, arrowIntersection );

		renderer->setColor( unitColor );
		renderer->drawLine( arrowIntersection, arrowTip );

		renderer->popTransform();
	}


	void Unit::onCollision( Actor* other, const Vector& displacement, float collisionDistance )
	{
		// Get the amount that both actors are overlapping on each axis (i.e. as a vector).
		float distance = displacement.getLength();
		Vector direction( 1.0f, 0.0f );
		float overlap = 1.0f;

		if( distance > 0.0f )
		{
			direction = ( displacement / distance );
			overlap = 1.0f - ( distance / collisionDistance );
		}

		// Apply an impulse based on distance.
		translate( direction * overlap * 1.0f );
	}


	void Unit::orderMoveTo( const Point& destination )
	{
		// TODO: Pathfinding
		//Path path( this, destination );
		//setCurrentPath( path );
		setTargetLocation( m_position );
	}


	void Unit::setFormation( Formation* formation )
	{
		if( formation != m_formation )
		{
			// Set the new formation for this unit.
			m_formation = formation;

			// Reset the slot index for this unit.
			m_formationSlotIndex = -1;
		}
	}


	void Unit::onAssignedToSlot()
	{
		requires( hasFormation() );
		requires( hasFormationSlot() );

		// If this unit doesn't have a current path and hasn't requested one,
		// request a path for this unit.
		//m_currentPathRequestIndex = getWorld()->getMap()->requestPathForUnit( this, m_formation->getSlotWorldLocation( m_formationSlotIndex ) );
	}


	void Unit::onEvictedFromSlot()
	{

	}
}