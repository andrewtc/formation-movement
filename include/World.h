#ifndef ATC_WORLD_H
#define ATC_WORLD_H

namespace atc
{
	/**
	 * Manages, updates, and draws Actors.
	 */
	class World
	{
	public:
		static const char* MAP_FOLDER_PATH;
		static const char* MAP_FILE_EXTENSION;

		World();
		~World();

		void loadMap( const std::string& name );
		void init();
		void destroy();

		void addActor( Actor* actor );
		Actor* getActorByID( Actor::ID id ) const;
		void killAllActors();
		void removeAllActors();

		void update( double elapsedTime );
		void draw( Renderer* renderer );

		Camera* spawnDefaultCamera();
		Unit* spawnUnit( const Point& location );

		void setCamera( Camera* camera );
		Camera* getCamera() const;
		Point getMouseWorldPosition() const;
		UnitSelection& getUnitSelection();
		void setUnitSelection( const UnitSelection& selection );
		const UnitSelection& getUnitSelection() const;
		Unit* getUnitAtLocation( const Point& location ) const;
		void getAllUnitsInArea( const Point& firstCorner, const Point& secondCorner, UnitSelection& result );

		Formation* createFormation( const Point& origin, const Point& destination, FormationBehavior* behavior );

		Map* getMap();
		const Map* getMap() const;
		float getLeft() const;
		float getRight() const;
		float getBottom() const;
		float getTop() const;

		Map::TileVector worldToTileCoords( const Point& worldCoords ) const;
		Point tileToWorldCoords( const Map::TileVector& tileCoords ) const;
		Point tileToWorldCoords( const Flowfield::TileVector& tileCoords ) const;
		static Direction cardinalDirectionToWorldDirection( CardinalDirection direction );

		Map::Tile getMapTileAtPosition( const Point& worldCoords );
		Map::ConstTile getMapTileAtPosition( const Point& worldCoords ) const;
		Map::Tile getMapTileAtPosition( float x, float y );
		Map::ConstTile getMapTileAtPosition( float x, float y ) const;
		Point getMapTileWorldPosition( Map::ConstTile tile ) const;

		Flowfield::Tile getFlowfieldTileAtPosition( Flowfield* flowfield, const Point& worldCoords ) const;
		Flowfield::ConstTile getFlowfieldTileAtPosition( const Flowfield* flowfield, const Point& worldCoords ) const;
		Flowfield::Tile getFlowfieldTileAtPosition( Flowfield* flowfield, float x, float y ) const;
		Flowfield::ConstTile getFlowfieldTileAtPosition( const Flowfield* flowfield, float x, float y ) const;
		Point getFlowfieldTileWorldPosition( Flowfield::ConstTile tile ) const;

		static bool findTimeToIntercept( float origin, float intercept, float velocity, float& result );
		static bool findTimeToInterceptGrid( float origin, float velocity, float& result );
		static void traceGridIntercepts( const Point& origin, const Point& destination, std::vector< Point >& result );

		bool areaIsPassable( const Point& position, float radius ) const;
		bool traceIsPassable( const Point& origin, const Point& destination, float radius );

		void startTrace( const Point& origin );
		void endTrace();
		bool isTracing() const;

	protected:
		void drawMap( Renderer* renderer, short tileLeft, short tileBottom, short tileRight, short tileTop );
		void drawFlowfield( Flowfield* flowfield, Renderer* renderer, Color color, short tileLeft, short tileBottom, short tileRight, short tileTop );

		void destroyRemovedActors();
		void destroyEmptyFormations();

		Actor::ID m_nextActorID;
		int m_nextFormationIndex;
		Camera* m_camera;
		bool m_isTracing;
		Point m_traceOrigin;
		Point m_traceDestination;
		std::map< Actor::ID, Actor* > m_actorsByID;
		std::vector< Actor* > m_actorsToRemove;
		std::map< int, Formation* > m_formationsByIndex;
		UnitSelection m_unitSelection;
		Map m_map;
	};
}

#endif