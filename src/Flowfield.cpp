#include "common.h"
#include "Flowfield.h"

namespace atc
{
	Flowfield::Flowfield() :
		m_map( nullptr ),
		m_isReserved( false ),
		m_goalTile( getTile( 0, 0 ) )
	{ }


	Flowfield::~Flowfield() { }


	void Flowfield::recalculate()
	{
		// Clear any existing data.
		clear();

		// Add goal location.
		m_goalTile->setGoal( true );
		m_goalTile->setClosed( true );
		m_tilesToEvaluate.insert( 0, m_goalTile );

		while( m_tilesToEvaluate.getSize() > 0 )
		{
			// Pop the tile with the minimum goal distance and evaluate it.
			Tile tile = m_tilesToEvaluate.popMinElement();
			CardinalDirection direction = CARDINAL_DIRECTION_EAST;

			for( int i = 0; i < CARDINAL_DIRECTION_COUNT; ++i )
			{
				// Evaluate the adjacent tile.
				evaluateTile( tile, direction );

				// Go to the next tile direction to evaluate.
				direction = getCounterClockwiseDirection( direction );
			}
		}
	}


	void Flowfield::evaluateTile( Tile tile, CardinalDirection direction )
	{
		// Get the next adjacent tile from this one.
		Tile adjacentTile = tile.getAdjacentTile( direction );

		if( adjacentTile.isValid() )
		{
			// If this node hasn't already been visited, get the Map tile for the adjacent tile.
			TileVector position = adjacentTile.getPosition();
			Map::Tile mapTile = m_map->getTile( position.x, position.y );

			if( mapTile->isPassable() )
			{
				// Add adjacency information for this tile.
				adjacentTile->addAdjacency( getOppositeDirection( direction ) );

				if( !adjacentTile->isClosed() )
				{
					// Close the tile.
					adjacentTile->setClosed( true );

					// Calculate the distance to the goal.
					unsigned int adjacentDistanceToGoal = ( tile->getDistanceToGoal() + 1 );
					adjacentTile->setDistanceToGoal( adjacentDistanceToGoal );

					// Add the tile to the list of tiles to be evaluated.
					m_tilesToEvaluate.insert( adjacentDistanceToGoal, adjacentTile );
				}
			}
		}
	}


	void Flowfield::destroy()
	{
		// Tell the map to free this Flowfield.
		m_map->destroyFlowfield( this );
	}


	void Flowfield::setGoalTile( const Tile& tile )
	{
		m_goalTile = tile;
	}


	Flowfield::Tile Flowfield::getGoalTile()
	{
		return m_goalTile;
	}


	Flowfield::ConstTile Flowfield::getGoalTile() const
	{
		return m_goalTile;
	}
}