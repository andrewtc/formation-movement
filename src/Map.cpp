#include "common.h"
#include "Map.h"


namespace atc
{
	// ------------------------------ Map ------------------------------

	Map::Map() :
		m_nextFlowfieldIndex( 0 ),
		m_nextPathfindIndex( 0 )
	{
		init();
	}


	Map::Map( unsigned int width, unsigned int height, const MapTile& fillTile ) :
		Grid( width, height, fillTile ),
		m_nextFlowfieldIndex( 0 )
	{
		resize( width, height );
		init();
	}


	Map::~Map() { }


	void Map::init()
	{
		for( int i = 0; i < MAX_FLOWFIELDS; ++i )
		{
			// Initialize all flowfield map pointers.
			m_flowfields[ i ].m_map = this;
		}
	}


	void Map::update( double elapsedTime )
	{
		// Determine how many paths to handle this frame.
		size_t pathfindCount = std::min( m_pathfindRequestsByIndex.size(), (size_t) MAX_PATHFINDS_PER_FRAME );

		for( size_t i = 0; i < pathfindCount; ++i )
		{
			// Fulfill the next path request.
			findPath();
		}
	}


	int Map::requestPathForUnit( Unit* unit, const Point& destination )
	{
		requires( unit );

		// Reserve the next available Path index.
		int index = m_nextPathfindIndex;
		requires( m_pathfindRequestsByIndex.find( index ) == m_pathfindRequestsByIndex.end() );
		++m_nextPathfindIndex;

		// Create a new path request for this Unit.
		PathfindRequest request( index, unit, destination );

		// Add the request to the list of open requests.
		m_pathfindRequestsByIndex[ index ] = request;

		// Return the index of the new path.
		return index;
	}


	void Map::findPath()
	{
		if( !m_pathfindRequestsByIndex.empty() )
		{
			// If there is a path request pending, pop it from the list.
			auto it = m_pathfindRequestsByIndex.begin();
			PathfindRequest request = it->second;
			m_pathfindRequestsByIndex.erase( it );
			
			// Start a new path.
			Path path( request.unit, request.destination );

			// Make sure the destination tile is passable.
			// TODO: Give Map a reference to the World, for convenience.
			World* world = request.unit->getWorld();
			Map::Tile destinationTile = world->getMapTileAtPosition( request.destination );

			if( destinationTile.isValid() && destinationTile->isPassable() )
			{
				// Add the Unit's current tile to the open list, with a cost of zero and a direction of none.
				Map::Tile startingTile = request.unit->getCurrentTile();
				startingTile->setLastPathfindCost( 0.0f );
				startingTile->setLastPathfindDirection( CARDINAL_DIRECTION_NONE );

				startingTile->open( request.index );
				m_openList.insert( 0.0f, startingTile );

				bool pathWasFound = false;

				while( pathWasFound == false && !m_openList.isEmpty() )
				{
					// Pop the first open tile off the open list.
					Map::Tile currentTile = m_openList.popMinElement();

					// Flag the tile as closed.
					currentTile->close( request.index );
					
					if( currentTile == destinationTile )
					{
						// If the goal tile was reached, mark that the path was found.
						pathWasFound = true;

						// If the destination was reached, build the path back to the goal.
						for( Map::Tile pathTile = destinationTile;
							pathTile->getLastPathfindDirection() != CARDINAL_DIRECTION_NONE;
							pathTile = pathTile.getAdjacentTile( pathTile->getLastPathfindDirection() ) )
						{
							path.pushWaypoint( world->getMapTileWorldPosition( pathTile ) );
						}

						// Clear the open list.
						m_openList.clear();

						// Exit the loop.
						break;
					}
					else
					{
						CardinalDirection direction = CARDINAL_DIRECTION_EAST;

						for( int i = 0; i <= CARDINAL_DIRECTION_COUNT; ++i )
						{
							// Get each adjacent tile from the current one.
							direction = getCounterClockwiseDirection( direction );
							Map::Tile adjacentTile = currentTile.getAdjacentTile( direction );

							if( adjacentTile.isValid() && !adjacentTile->isClosed( request.index ) && adjacentTile->isPassable() )
							{
								// If the tile is passable, calculate the cost of each adjacent tile.
								float distanceToGoal = (float) Map::TileVector::getManhattanDistance( adjacentTile.getPosition(), destinationTile.getPosition() );
								float costToEnterTile = ( currentTile->getLastPathfindCost() + 1.0f ); // TODO: Support variable entry costs, for collision avoidance.
								float totalCost = ( distanceToGoal + costToEnterTile );

								bool tileDataNeedsUpdate = false;

								if( !adjacentTile->isOpen( request.index ) )
								{
									// Open the tile and add it to the open list.
									adjacentTile->open( request.index );
									m_openList.insert( totalCost, adjacentTile );

									// Update the tile data.
									tileDataNeedsUpdate = true;
								}
								else if( totalCost < adjacentTile->getLastPathfindCost() )
								{
									// If the tile is already open and a better cost was found, update the existing tile on the open list.
									m_openList.update( totalCost, adjacentTile );

									// Update the tile data.
									tileDataNeedsUpdate = true;
								}

								if( tileDataNeedsUpdate )
								{
									// If the tile cost to be updated, update it.
									adjacentTile->setLastPathfindCost( totalCost );

									// Keep track of the tile from which we came.
									CardinalDirection directionToPreviousTile = getOppositeDirection( direction );
									adjacentTile->setLastPathfindDirection( directionToPreviousTile );
								}
							}
						}
					}
				}
			}

			// Update the Unit's path.
			request.unit->setCurrentPath( path );
		}
	}


	Flowfield* Map::createFlowfield()
	{
		requires( m_nextFlowfieldIndex < MAX_FLOWFIELDS );

		// Get the next available flowfield and initialize it.
		Flowfield* result = &m_flowfields[ m_nextFlowfieldIndex ];
		result->reserve();

		// Resize the flowfield to the same size as the map.
		result->resize( m_width, m_height );

		for( ++m_nextFlowfieldIndex; m_nextFlowfieldIndex < MAX_FLOWFIELDS; ++m_nextFlowfieldIndex )
		{
			// Find the next free flowfield after this one.
			Flowfield* currentFlowfield = &m_flowfields[ m_nextFlowfieldIndex ];

			if( currentFlowfield->isAvailable() )
			{
				break;
			}
		}

		return result;
	}


	void Map::destroyFlowfield( Flowfield* flowfield )
	{
		// Make sure the flowfield provided is actually part of the Map.
		ptrdiff_t flowfieldIndex = ( flowfield - m_flowfields );
		requires( flowfieldIndex >= 0 && flowfieldIndex < MAX_FLOWFIELDS );

		// Destroy the flowfield.
		flowfield->release();

		if( flowfieldIndex < m_nextFlowfieldIndex )
		{
			// Keep track of the next available flowfield index.
			m_nextFlowfieldIndex = flowfieldIndex;
		}
	}
}