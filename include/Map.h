#ifndef ATC_MAP_H
#define ATC_MAP_H


namespace atc
{
	class MapTile
	{
	public:
		MapTile();
		~MapTile();

		void setPassable( bool isPassable );
		bool isPassable() const;

		void open( int pathfindIndex );
		void close( int pathfindIndex );

		bool isOpen( int pathfindIndex ) const;
		bool isClosed( int pathfindIndex ) const;

		void setLastPathfindDirection( CardinalDirection direction );
		CardinalDirection getLastPathfindDirection() const;

		void setLastPathfindCost( float cost );
		float getLastPathfindCost() const;

	protected:
		bool m_isPassable;
		int m_lastPathfindOpened;
		int m_lastPathfindClosed;
		CardinalDirection m_lastPathfindDirection;
		float m_lastPathfindCost;
	};


	/**
	 * Represents the map of the game world.
	 */
	class Map : public Grid< MapTile, short, 10 >
	{
	public:
		static const int MAX_FLOWFIELDS = 16;
		static const int MAX_PATHFINDS_PER_FRAME = 1;

		Map();
		Map( unsigned int width, unsigned int height, const MapTile& fillTile = MapTile() );
		~Map();

		void update( double elapsedTime );

		int requestPathForUnit( Unit* unit, const Point& destination );
		void cancelPathRequest( int pathIndex );

		Flowfield* createFlowfield();
		void destroyFlowfield( Flowfield* flowfield );

		float getLeft() const;
		float getRight() const;
		float getBottom() const;
		float getTop() const;

	private:
		struct PathfindRequest
		{
			PathfindRequest() { }
			PathfindRequest( int index, Unit* unit, const Point& destination ) :
				index( index ), unit( unit ), destination( destination )
			{ }

			int index;
			Unit* unit;
			Point destination;
		};

		void init();
		void findPath();

		int m_nextPathfindIndex;
		int m_nextFlowfieldIndex;
		std::map< int, PathfindRequest > m_pathfindRequestsByIndex;
		FixedSizeMinHeap< 1024, float, Tile > m_openList;
		MapTile m_tiles[ MAX_TILES ];
		Flowfield m_flowfields[ MAX_FLOWFIELDS ];
	};
}

#endif