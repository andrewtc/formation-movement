#ifndef ATC_FLOWFIELD_INL
#define ATC_FLOWFIELD_INL

namespace atc
{
	// ------------------------------ FlowData ------------------------------

	inline FlowData::FlowData() :
		m_flags( FLAG_NONE ),
		m_adjacencyCount( 0 ),
		m_distanceToGoal( 0 )
	{ }


	inline FlowData::~FlowData() { }


	inline void FlowData::setGoal( bool isGoal )
	{
		if( isGoal )
		{
			m_flags |= FLAG_IS_GOAL;
		}
		else
		{
			m_flags &= ~FLAG_IS_GOAL;
		}
	}


	inline bool FlowData::isGoal() const
	{
		return ( m_flags & FLAG_IS_GOAL ) > 0;
	}


	inline void FlowData::setClosed( bool isClosed )
	{
		if( isClosed )
		{
			m_flags |= FLAG_IS_CLOSED;
		}
		else
		{
			m_flags &= ~FLAG_IS_CLOSED;
		}
	}


	inline bool FlowData::isClosed() const
	{
		return ( m_flags & FLAG_IS_CLOSED ) > 0;
	}


	inline void FlowData::clearAdjacencies()
	{
		m_adjacencyCount = 0;
	}


	inline size_t FlowData::getAdjacencyCount() const
	{
		return m_adjacencyCount;
	}


	inline void FlowData::addAdjacency( CardinalDirection direction )
	{
		// We should never add an adjacency twice.
		promises( !hasAdjacency( direction ) );

		// Add the adjacency to the list of adjacencies.
		m_adjacencies[ (size_t) m_adjacencyCount ] = (unsigned char) direction;
		++m_adjacencyCount;
	}


	inline CardinalDirection FlowData::getAdjacency( size_t index ) const
	{
		CardinalDirection result = CARDINAL_DIRECTION_NONE;

		if( index < (size_t) m_adjacencyCount )
		{
			result = (CardinalDirection) m_adjacencies[ index ];
		}

		return result;
	}


	inline CardinalDirection FlowData::getBestAdjacency() const
	{
		// Return the first (best) direction from this tile.
		return getAdjacency( 0 );
	}


	inline bool FlowData::hasAdjacency( CardinalDirection direction ) const
	{
		bool result = false;

		for( size_t i = 0; i < (size_t) m_adjacencyCount; ++i )
		{
			if( (CardinalDirection) m_adjacencies[ i ] == direction )
			{
				result = true;
				break;
			}
		}

		return result;
	}


	inline void FlowData::setDistanceToGoal( unsigned int distance )
	{
		m_distanceToGoal = distance;
	}


	inline unsigned int FlowData::getDistanceToGoal() const
	{
		return m_distanceToGoal;
	}


	// ------------------------------ Flowfield ------------------------------

	inline void Flowfield::reserve()
	{
		m_isReserved = true;
	}


	inline void Flowfield::release()
	{
		m_isReserved = false;
	}


	inline bool Flowfield::isReserved() const
	{
		return m_isReserved;
	}


	inline bool Flowfield::isAvailable() const
	{
		return !m_isReserved;
	}
}

#endif