#include "common.h"
#include "World.h"


namespace atc
{
	const char* World::MAP_FOLDER_PATH = "data/maps/";
	const char* World::MAP_FILE_EXTENSION = ".png";


	World::World() :
		m_nextActorID( 0 ),
		m_nextFormationIndex( 0 ),
		m_camera( nullptr ),
		m_isTracing( false )
	{ }


	World::~World()
	{
		destroy();
	}


	void World::loadMap( const std::string& name )
	{
		// Build the path to the map image file.
		std::stringstream formatter;
		formatter << MAP_FOLDER_PATH << name << MAP_FILE_EXTENSION;
		std::string path = formatter.str();

		// Load the image file for the map.
		int width, height, channelCount;
		unsigned int* texels = (unsigned int*) stbi_load( path.c_str(), &width, &height, &channelCount, 4 );

		if( texels )
		{
			// Resize the map.
			m_map.resize( width, height );
			m_map.clear();

			for( int y = 0; y < height; ++y )
			{
				for( int x = 0; x < width; ++x )
				{
					// Interpret each pixel as a tile and load it based on color.
					unsigned int texel = texels[ ( ( height - y - 1 ) * width ) + x ];

					switch( texel )
					{
					// Interpret full white pixels as non-passable tiles.
					case 0xFFFFFFFF:
						m_map.getTile( x, y )->setPassable( false );
						break;

					// Interpret green pixels as friendly units.
					case 0xFF00FF00:
						spawnUnit( Point( (float) x, (float) y ) );
						break;
					}
				}
			}

			// Free the image data.
			stbi_image_free( texels );
		}
	}


	void World::init()
	{
		// Spawn a default camera and make it current.
		Camera* defaultCamera = spawnDefaultCamera();
		setCamera( defaultCamera );
	}


	void World::destroy()
	{
		removeAllActors();
		destroyRemovedActors();
	}


	void World::addActor( Actor* actor )
	{
		// Make sure the Actor is not part of another World.
		requires( actor->getWorld() == nullptr );
		requires( actor->getID() == -1 );

		// Assign the Actor a unique ID.
		Actor::ID id = m_nextActorID;
		++m_nextActorID;
		requires( getActorByID( id ) == nullptr );
		actor->m_ID = id;

		// Add the Actor to the World.
		m_actorsByID[ id ] = actor;
		actor->m_world = this;

		// Initialize the Actor.
		actor->init();
	}


	void World::killAllActors()
	{
		for( auto it = m_actorsByID.begin(); it != m_actorsByID.end(); ++it )
		{
			it->second->kill();
		}
	}


	void World::removeAllActors()
	{
		for( auto it = m_actorsByID.begin(); it != m_actorsByID.end(); ++it )
		{
			it->second->remove();
		}
	}


	void World::destroyRemovedActors()
	{
		for( auto it = m_actorsByID.begin(); it != m_actorsByID.end(); ++it )
		{
			Actor* actor = ( it->second );

			if( actor->m_needsRemoval )
			{
				// If any actor needs to be removed, add it to the removal list.
				m_actorsToRemove.push_back( actor );
			}
		}

		for( auto it = m_actorsToRemove.begin(); it != m_actorsToRemove.end(); ++it )
		{
			Actor* actor = ( *it );

			// Remove and destroy actors that need removal.
			auto itToRemove = m_actorsByID.find( actor->getID() );
			promises( itToRemove != m_actorsByID.end() );
			m_actorsByID.erase( itToRemove );

			delete actor;
		}

		// Clear the list of removed actors.
		m_actorsToRemove.clear();
	}


	void World::update( double elapsedTime )
	{
		// Update the Map.
		m_map.update( elapsedTime );

		for( auto it = m_formationsByIndex.begin(); it != m_formationsByIndex.end(); ++it )
		{
			// Update all Formations.
			it->second->update( elapsedTime );
		}

		for( auto it = m_actorsByID.begin(); it != m_actorsByID.end(); ++it )
		{
			// Update all actors.
			it->second->update( elapsedTime );
		}

		for( auto first = m_actorsByID.begin(); first != m_actorsByID.end(); ++first )
		{
			Actor* firstActor = first->second;

			if( !firstActor->isCollisionEnabled() )
			{
				// Don't let Actors without collision collide with each other.
				continue;
			}

			for( auto second = first; second != m_actorsByID.end(); ++second )
			{
				Actor* secondActor = second->second;

				if( firstActor == secondActor || !secondActor->isCollisionEnabled() )
				{
					// Don't let Actors without collision collide with each other,
					// and don't let actors collide with themselves.
					continue;
				}

				// Calculate the distance between both Actors and their collision distance.
				Vector displacementCenterToCenter = ( secondActor->getPosition() - firstActor->getPosition() );
				float distanceSquared = displacementCenterToCenter.getLengthSquared();
				float combinedRadii = ( firstActor->getCollisionRadius() + secondActor->getCollisionRadius() );
				float collisionDistanceSquared = ( combinedRadii * combinedRadii );

				if( distanceSquared < collisionDistanceSquared )
				{
					float distanceCenterToCenter = displacementCenterToCenter.normalize();
					float penetrationDepth = ( combinedRadii - distanceCenterToCenter );

					Vector nudge = displacementCenterToCenter * ( penetrationDepth * 0.5f );
					nudge *= 0.5f;

					firstActor->translate( -1.0f * nudge );
					secondActor->translate( 1.0f * nudge );

					// Let each pair of Actors handle collision.
					//firstActor->onCollision( secondActor, displacement * -0.5f, collisionDistance );
					//secondActor->onCollision( firstActor, displacement * 0.5f, collisionDistance );
				}
			}
		}

		for( auto it = m_actorsByID.begin(); it != m_actorsByID.end(); ++it )
		{
			// Collide with walls.
			it->second->collideWithWalls();
		}

		// Destroy removed Actors.
		destroyRemovedActors();

		// Destroy empty formations.
		destroyEmptyFormations();
	}


	void World::draw( Renderer* renderer )
	{
		// Apply the current camera (if any).
		if( m_camera )
		{
			m_camera->apply();
		}
		else
		{
			renderer->resetView();
		}

		// Start drawing.
		renderer->beginDraw();

		// Determine the bounds of all visible tiles.
		Point minBounds = m_camera->deviceToWorldCoords( Point( renderer->getViewLeft(), renderer->getViewBottom() ) );
		Point maxBounds = m_camera->deviceToWorldCoords( Point( renderer->getViewRight(), renderer->getViewTop() ) );

		// Determine the tile bounds of all visible tiles.
		Map::TileVector minTilePos = worldToTileCoords( minBounds );
		Map::TileVector maxTilePos = worldToTileCoords( maxBounds );

		short tileLeft   = std::max< short >( minTilePos.x, 0 );
		short tileBottom = std::max< short >( minTilePos.y, 0 );
		short tileRight  = std::min< short >( maxTilePos.x, m_map.getWidth() - 1 );
		short tileTop    = std::min< short >( maxTilePos.y, m_map.getHeight() - 1 );

		// Draw the map.
		drawMap( renderer, tileLeft, tileBottom, tileRight, tileTop );

		// Outline the border of the map.
		Point bottomLeft( getLeft(), getBottom() );
		Point topRight( getRight(), getTop() );

		glColor3f( 1.0f, 1.0f, 1.0f );
		renderer->drawRectangle( bottomLeft, topRight );

		for( auto it = m_formationsByIndex.begin(); it != m_formationsByIndex.end(); ++it )
		{
			// Draw all formations.
			it->second->draw( renderer );
		}

		// Draw flowfields for all currently selected units.
		std::set< Formation* > formationsToDraw;

		for( int i = 0; i < m_unitSelection.getUnitCount(); ++i )
		{
			Formation* formation = m_unitSelection.getUnitByIndex( i )->getFormation();

			if( formation )
			{
				formationsToDraw.insert( formation );
			}
		}

		for( auto it = formationsToDraw.begin(); it != formationsToDraw.end(); ++it )
		{
			Formation* formation = ( *it );

			if( formation->hasFlowfield() )
			{
				Flowfield* flowfield = formation->getFlowfield();
				drawFlowfield( flowfield, renderer, formation->getColor(), tileLeft, tileBottom, tileRight, tileTop );
			}
		}

		for( auto it = m_actorsByID.begin(); it != m_actorsByID.end(); ++it )
		{
			// Draw all Actors.
			it->second->draw( renderer );
		}

		// Draw the current selection.
		m_unitSelection.draw( renderer );

		glBegin( GL_LINES );

		// Draw X axis.
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex2f( 0.0f, 0.0f );
		glVertex2f( AXIS_LENGTH, 0.0f );

		// Draw Y axis.
		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex2f( 0.0f, 0.0f );
		glVertex2f( 0.0f, AXIS_LENGTH );

		glEnd();

		if( m_isTracing )
		{
			// Update the trace destination.
			m_traceDestination = g_app.getWorld()->getMouseWorldPosition();

			// Trace the intercepts between both points.
			std::vector< Point > intercepts;
			traceGridIntercepts( m_traceOrigin, m_traceDestination, intercepts );

			// If the trace is clear, draw it in white.
			Color color = Color::createFromValues( 1.0f, 0.0f, 0.0f );

			if( traceIsPassable( m_traceOrigin, m_traceDestination, 0.5f ) )
			{
				// If the trace is NOT clear, draw it in red.
				color = Color::WHITE;
			}

			renderer->setColor( color );
			renderer->drawLine( m_traceOrigin, m_traceDestination );

			for( auto it = intercepts.begin(); it != intercepts.end(); ++it )
			{
				renderer->pushTransform( Vector( it->x, it->y ) );
				renderer->drawCircle( 0.1f, 4 );
				renderer->popTransform();
			}
		}

		// Stop drawing.
		renderer->endDraw();
	}


	void World::drawMap( Renderer* renderer, short tileLeft, short tileBottom, short tileRight, short tileTop )
	{
		for( unsigned int y = tileBottom; y <= tileTop; ++y )
		{
			for( unsigned int x = tileLeft; x <= tileRight; ++x )
			{
				// Get the position of each tile.
				Map::Tile tile = m_map.getTile( x, y );

				Point bottomLeft = Point( (float) x - 0.5f, (float) y - 0.5f );
				Point topRight = Point( (float) x + 0.5f, (float) y + 0.5f );

				if( tile->isPassable() )
				{
					glColor3f( 0.1f, 0.15f, 0.25f );
					renderer->drawRectangle( bottomLeft, topRight );
				}
				else
				{
					glColor3f( 1.0f, 1.0f, 1.0f );
					renderer->fillRectangle( bottomLeft, topRight );
				}
			}
		}
	}


	void World::drawFlowfield( Flowfield* flowfield, Renderer* renderer, Color color, short tileLeft, short tileBottom, short tileRight, short tileTop )
	{
		requires( flowfield );
		requires( renderer );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		short width = flowfield->getWidth();
		short height = flowfield->getHeight();

		float alphaScale = ( 255.0f / ( width + height ) );

		for( unsigned int y = tileBottom; y <= tileTop; ++y )
		{
			for( unsigned int x = tileLeft; x <= tileRight; ++x )
			{
				// Get the tile to draw.
				Flowfield::Tile tile = flowfield->getTile( x, y );

				// Adjust the opacity of the color based on cost.
				Color tileColor = color;
				tileColor.alpha = (unsigned char) std::min( tile->getDistanceToGoal() * alphaScale, 255.0f );

				// Draw the tile.
				renderer->setColor( tileColor );

				Point center = Point( (float) x, (float) y );
				Point topLeft( x - 0.5f, y - 0.5f );
				Point bottomRight = ( topLeft + Vector( 1.0f, 1.0f ) );

				renderer->fillRectangle( topLeft, bottomRight );

				Color detailColor( color );
				detailColor.alpha = 255;
				renderer->setColor( detailColor );

				if( tile->isGoal() )
				{
					// If this is the goal, render a goal marker.
					renderer->pushTransform( Vector( (float) x, (float) y ) );

					renderer->drawCircle( 0.45f, 12 );
					renderer->drawCircle( 0.35f, 12 );
					renderer->drawCircle( 0.25f, 12 );
					renderer->drawCircle( 0.15f, 12 );

					renderer->popTransform();
				}

				CardinalDirection bestAdjacentTileDirection = tile->getAdjacency( 0 );

				if( bestAdjacentTileDirection != CARDINAL_DIRECTION_NONE )
				{
					Point endpoint( center );

					switch( bestAdjacentTileDirection )
					{
					case CARDINAL_DIRECTION_EAST:
						endpoint.x += 0.5f;
						break;

					case CARDINAL_DIRECTION_NORTH:
						endpoint.y -= 0.5f;
						break;

					case CARDINAL_DIRECTION_WEST:
						endpoint.x -= 0.5f;
						break;

					case CARDINAL_DIRECTION_SOUTH:
						endpoint.y += 0.5f;
						break;
					}

					renderer->pushTransform( Vector( (float) x, (float) y ) );
					renderer->fillCircle( 0.1f, 4 );
					renderer->popTransform();

					renderer->drawLine( center, endpoint );
				}
			}
		}

		glDisable( GL_BLEND );
	}


	Camera* World::spawnDefaultCamera()
	{
		// Create a new camera in the middle of the map.
		Camera* camera = new Camera();
		camera->setPosition( Point( m_map.getWidth() * 0.5f, m_map.getHeight() * 0.5f ) );
		camera->setZoom( TILE_SCALE );

		// Add the camera to the world.
		addActor( camera );

		return camera;
	}


	Unit* World::spawnUnit( const Point& location )
	{
		// Create a new Unit and add it to the world.
		Unit* unit = new Unit();
		unit->onSpawn( location );

		addActor( unit );

		return unit;
	}


	Point World::getMouseWorldPosition() const
	{
		requires( m_camera );
		return m_camera->deviceToWorldCoords( g_app.getWindow()->getMousePosition() );
	}


	Unit* World::getUnitAtLocation( const Point& location ) const
	{
		Unit* unit = nullptr;

		for( auto it = m_actorsByID.begin(); it != m_actorsByID.end(); ++it )
		{
			Actor* actor = it->second;

			if( actor->isOverlappingLocation( location ) )
			{
				unit = dynamic_cast< Unit* >( actor );

				if( unit != nullptr )
				{
					break;
				}
			}
		}

		return unit;
	}


	void World::getAllUnitsInArea( const Point& firstCorner, const Point& secondCorner, UnitSelection& result )
	{
		result.clear();

		float minX = std::min( firstCorner.x, secondCorner.x );
		float maxX = std::max( firstCorner.x, secondCorner.x );
		float minY = std::min( firstCorner.y, secondCorner.y );
		float maxY = std::max( firstCorner.y, secondCorner.y );

		for( auto it = m_actorsByID.begin(); it != m_actorsByID.end(); ++it )
		{
			Actor* actor = it->second;
			Point position = actor->getPosition();

			if( position.x >= minX && position.x <= maxX &&
				position.y >= minY && position.y <= maxY )
			{
				Unit* unit = dynamic_cast< Unit* >( actor );

				if( unit )
				{
					result.addUnit( unit );
				}
			}
		}
	}


	Formation* World::createFormation( const Point& origin, const Point& destination, FormationBehavior* behavior )
	{
		// Create a new formation with the next available index.
		Color color = Formation::getColorByIndex( m_nextFormationIndex );
		Formation* formation = new Formation( this, m_nextFormationIndex, origin, destination, behavior, color );

		// Keep track of the formation by its index.
		m_formationsByIndex[ m_nextFormationIndex ] = formation;

		while( m_formationsByIndex.find( m_nextFormationIndex ) != m_formationsByIndex.end() )
		{
			// Increment the next formation index.
			++m_nextFormationIndex;
		}

		return formation;
	}


	void World::destroyEmptyFormations()
	{
		std::vector< Formation* > formationsToDestroy;

		for( auto it = m_formationsByIndex.begin(); it != m_formationsByIndex.end(); ++it )
		{
			Formation* formation = it->second;

			if( formation->getUnitCount() <= 0 )
			{
				// If a formation is too empty, mark it for destruction.
				formationsToDestroy.push_back( formation );
			}
		}

		for( auto it = formationsToDestroy.begin(); it != formationsToDestroy.end(); ++it )
		{
			Formation* formation = *it;

			// Remove all units from the formation and destroy it.
			formation->removeAllUnits();

			int index = formation->getIndex();
			m_formationsByIndex.erase( index );

			if( index < m_nextFormationIndex )
			{
				m_nextFormationIndex = index;
			}

			delete formation;
		}
	}


	void World::traceGridIntercepts( const Point& origin, const Point& destination, std::vector< Point >& result )
	{
		// Clear the result vector.
		result.clear();

		// Determine the vector of travel.
		Vector displacement = ( destination - origin );

		if( displacement.getLengthSquared() > 0.0f )
		{
			// If the points are separate, calculate the distance to reach the destination.
			float distance = displacement.getLength();

			// Determine the direction of motion.
			Vector direction = ( displacement / distance );

			// Find the time to reach the closest grid line in either direction (if any).
			float timeToInterceptX, timeToInterceptY;

			bool hasInterceptX = findTimeToInterceptGrid( origin.x, direction.x, timeToInterceptX );
			bool hasInterceptY = findTimeToInterceptGrid( origin.y, direction.y, timeToInterceptY );

			bool interceptWasFound = ( hasInterceptX || hasInterceptY );
			promises( interceptWasFound );

			// Find the time in each direction to cross a full grid square.
			float timeToCrossTileX = fabsf( 1.0f / direction.x );
			float timeToCrossTileY = fabsf( 1.0f / direction.y );

			while( true )
			{
				// Determine the time to the closest intercept.
				float timeToClosestIntercept = std::min( timeToInterceptX, timeToInterceptY );
				promises( timeToClosestIntercept > 0.0f );

				if( timeToClosestIntercept >= distance )
				{
					// If we've reached the destination already, stop tracing intercepts.
					// (NOTE: The time to the destination is simply the distance between the origin and destination.)
					break;
				}

				// Add a new intercept to the result vector.
				result.push_back( origin + ( direction * timeToClosestIntercept ) );

				// If either intercept was reached, increment it.
				bool goToNextInterceptX = ( timeToInterceptX <= timeToInterceptY );
				bool goToNextInterceptY = ( timeToInterceptY <= timeToInterceptX );

				if( goToNextInterceptX )
				{
					timeToInterceptX += timeToCrossTileX;
				}

				if( goToNextInterceptY )
				{
					timeToInterceptY += timeToCrossTileY;
				}
			}
		}
	}


	bool World::findTimeToIntercept( float origin, float intercept, float velocity, float& result )
	{
		// Determine whether an intercept exists.
		bool interceptExists = ( velocity != 0.0f );

		if( interceptExists )
		{
			// Determine the time to reach the intercept in question.
			result = ( ( intercept - origin ) / velocity );
			interceptExists = ( result > 0.0f );
		}

		if( !interceptExists )
		{
			// If a valid intercept doesn't exist, return positive infinity.
			result = std::numeric_limits< float >::infinity();
		}

		promises( result > 0.0f );

		return interceptExists;
	}


	bool World::findTimeToInterceptGrid( float origin, float velocity, float& result )
	{
		// Compensate for the fact that tiles are centered on their position.
		// TODO: Don't assume that each grid tile is one unit wide.
		origin -= 0.5f;

		// Determine the closest intercept.
		// TODO: Don't assume that each grid tile is one unit wide.
		float intercept = ( velocity > 0.0f ? floorf( origin + 1.0f ) : ceilf( origin - 1.0f ) );

		// Determine the time to reach the intercept in question.
		return findTimeToIntercept( origin, intercept, velocity, result );
	}


	bool World::areaIsPassable( const Point& position, float radius ) const
	{
		requires( radius >= 0.0f );

		bool result = true;

		// Get the bounds of the area to check.
		Vector radiusOffset( radius, radius );
		Map::TileVector minBounds = worldToTileCoords( position - radiusOffset );
		Map::TileVector maxBounds = worldToTileCoords( position + radiusOffset );

		for( short y = minBounds.y; y <= maxBounds.y; ++y )
		{
			for( short x = minBounds.x; x <= maxBounds.x; ++x )
			{
				Map::ConstTile tile = m_map.getTile( x, y );

				if( !( tile.isValid() && tile->isPassable() ) )
				{
					// If any tile is not passable or valid, return false.
					result = false;
					break;
				}
			}
		}

		return result;
	}


	bool World::traceIsPassable( const Point& origin, const Point& destination, float radius )
	{
		// Make sure the start and end points are both passable.
		bool result = ( areaIsPassable( origin, radius ) && areaIsPassable( destination, radius ) );

		if( result )
		{
			// Trace the intercepts between both points.
			std::vector< Point > intercepts;
			traceGridIntercepts( origin, destination, intercepts );

			for( auto it = intercepts.begin(); it != intercepts.end(); ++it )
			{
				if( !areaIsPassable( *it, radius ) )
				{
					// If any intercept along the path is not passable, return false.
					// Otherwise, return true.
					result = false;
					break;
				}
			}
		}

		return result;
	}
}