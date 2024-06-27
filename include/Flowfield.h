#ifndef ATC_FLOWFIELD_H
#define ATC_FLOWFIELD_H

namespace atc
{
	class Map;

	/**
	 * Represents the collected information about a tile from a
	 * flowfield pathfinding search.
	 */
	class FlowData
	{
	public:
		enum Flags
		{
			FLAG_NONE = 0,
			FLAG_IS_GOAL = 1,
			FLAG_IS_CLOSED = 2
		};

		FlowData();
		~FlowData();

		void setGoal( bool isGoal );
		bool isGoal() const;
		void setClosed( bool isClosed );
		bool isClosed() const;

		void clearAdjacencies();
		size_t getAdjacencyCount() const;
		void addAdjacency( CardinalDirection direction );
		CardinalDirection getAdjacency( size_t index ) const;
		CardinalDirection getBestAdjacency() const;
		bool hasAdjacency( CardinalDirection direction ) const;

		void setDistanceToGoal( unsigned int distance );
		unsigned int getDistanceToGoal() const;

	protected:
		unsigned char m_flags;
		unsigned char m_adjacencyCount;
		unsigned char m_adjacencies[ CARDINAL_DIRECTION_COUNT ]; // (Really a CardinalDirection)
		unsigned int m_distanceToGoal;
	};


	/**
	 * Stores the results of a flow field pathfinding search.
	 */
	class Flowfield : public Grid< FlowData, short, 10 >
	{
	public:
		void recalculate();
		void destroy();

		void setGoalTile( const Tile& tile );
		Tile getGoalTile();
		ConstTile getGoalTile() const;

	protected:
		Flowfield();
		~Flowfield();

		void evaluateTile( Tile tile, CardinalDirection direction );

		void reserve();
		void release();
		bool isReserved() const;
		bool isAvailable() const;

		bool m_isReserved;
		Map* m_map;
		Tile m_goalTile;
		FixedSizeMinHeap< MAX_TILES, unsigned int, Tile > m_tilesToEvaluate;

		friend class Map;
	};
}

#endif