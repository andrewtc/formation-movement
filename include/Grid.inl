#ifndef ATC_GRID_INL
#define ATC_GRID_INL


#define ATC_GRID_TEMPLATE \
	template< typename tileData_t,\
	typename tileOffset_t,\
	size_t maxDimensionPowerOfTwo >

#define ATC_GRID_TILE_TEMPLATE \
	ATC_GRID_TEMPLATE \
	template< bool isConst >

#define ATC_GRID_BASIC_TILE \
	ATC_GRID::BasicTile< isConst >

#define ATC_GRID_TILE_VECTOR \
	ATC_GRID::TileVector


namespace atc
{
	inline CardinalDirection getOppositeDirection( CardinalDirection direction )
	{
		CardinalDirection result = CARDINAL_DIRECTION_NONE;

		switch( direction )
		{
		case CARDINAL_DIRECTION_EAST:
			result = CARDINAL_DIRECTION_WEST;
			break;

		case CARDINAL_DIRECTION_NORTH:
			result = CARDINAL_DIRECTION_SOUTH;
			break;

		case CARDINAL_DIRECTION_WEST:
			result = CARDINAL_DIRECTION_EAST;
			break;

		case CARDINAL_DIRECTION_SOUTH:
			result = CARDINAL_DIRECTION_NORTH;
			break;
		}

		return result;
	}


	inline CardinalDirection getCounterClockwiseDirection( CardinalDirection direction )
	{
		CardinalDirection result = CARDINAL_DIRECTION_NONE;

		switch( direction )
		{
		case CARDINAL_DIRECTION_EAST:
			result = CARDINAL_DIRECTION_NORTH;
			break;

		case CARDINAL_DIRECTION_NORTH:
			result = CARDINAL_DIRECTION_WEST;
			break;

		case CARDINAL_DIRECTION_WEST:
			result = CARDINAL_DIRECTION_SOUTH;
			break;

		case CARDINAL_DIRECTION_SOUTH:
			result = CARDINAL_DIRECTION_EAST;
			break;
		}

		return result;
	}


	// ------------------------------ TileVector ------------------------------

	ATC_GRID_TEMPLATE
	ATC_GRID_TILE_VECTOR::TileVector() :
		x( 0 ), y( 0 )
	{ }


	ATC_GRID_TEMPLATE
	ATC_GRID_TILE_VECTOR::TileVector( TileOffset x, TileOffset y ) :
		x( x ), y( y )
	{ }


	ATC_GRID_TEMPLATE
	ATC_GRID_TILE_VECTOR::~TileVector() { }


	ATC_GRID_TEMPLATE
	typename ATC_GRID::TileOffset ATC_GRID_TILE_VECTOR::getManhattanDistance( const TileVector& first, const TileVector& second )
	{
		return ( abs( first.x - second.x ) + abs( first.y - second.y ) );
	}


	ATC_GRID_TEMPLATE
	bool ATC_GRID_TILE_VECTOR::operator!=( const TileVector& other ) const
	{
		return ( x != other.x || y != other.y );
	}


	ATC_GRID_TEMPLATE
	bool ATC_GRID_TILE_VECTOR::operator==( const TileVector& other ) const
	{
		return !( operator!=( other ) );
	}


	ATC_GRID_TEMPLATE
	void ATC_GRID_TILE_VECTOR::operator+=( const TileVector& other )
	{
		x += other.x;
		y += other.y;
	}


	ATC_GRID_TEMPLATE
	void ATC_GRID_TILE_VECTOR::operator-=( const TileVector& other )
	{
		x -= other.x;
		y -= other.y;
	}


	ATC_GRID_TEMPLATE
	typename ATC_GRID::TileVector ATC_GRID_TILE_VECTOR::operator+( const TileVector& other ) const
	{
		return TileVector( x + other.x, y + other.y );
	}


	ATC_GRID_TEMPLATE
	typename ATC_GRID::TileVector ATC_GRID_TILE_VECTOR::operator-( const TileVector& other ) const
	{
		return TileVector( x - other.x, y - other.y );
	}


	// ------------------------------ BasicTile ------------------------------

	ATC_GRID_TILE_TEMPLATE
	ATC_GRID_BASIC_TILE::BasicTile() :
		m_grid( nullptr )
	{ }
	
	
	ATC_GRID_TILE_TEMPLATE
	ATC_GRID_BASIC_TILE::BasicTile( TileGridType* grid, TileOffset x, TileOffset y ) :
		m_grid( grid ), m_position( x, y )
	{ }


	ATC_GRID_TILE_TEMPLATE
	ATC_GRID_BASIC_TILE::BasicTile( TileGridType* grid, const TileVector& position ) :
		m_grid( grid ), m_position( position )
	{ }


	ATC_GRID_TILE_TEMPLATE
	ATC_GRID_BASIC_TILE::BasicTile( const BasicTile< false >& other ) :
		m_grid( other.m_grid ), m_position( other.m_position )
	{ }


	ATC_GRID_TILE_TEMPLATE
	ATC_GRID_BASIC_TILE::~BasicTile() { }


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID_BASIC_TILE::TileGridType* ATC_GRID_BASIC_TILE::getGrid() const
	{
		return m_grid;
	}


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID_BASIC_TILE::TileDataType& ATC_GRID_BASIC_TILE::getData() const
	{
		requires( isValid() );
		size_t index = ( m_position.x + ( m_position.y * GridType::MAX_WIDTH ) );
		return m_grid->m_tiles[ index ];
	}


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID_BASIC_TILE::TileDataType& ATC_GRID_BASIC_TILE::operator*() const
	{
		return getData();
	}


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID_BASIC_TILE::TileDataType* ATC_GRID_BASIC_TILE::operator->() const
	{
		return &( getData() );
	}


	ATC_GRID_TILE_TEMPLATE
	template< bool isOtherConst >
	bool ATC_GRID_BASIC_TILE::operator==( const BasicTile< isOtherConst >& other ) const
	{
		return !( operator!=( other ) );
	}


	ATC_GRID_TILE_TEMPLATE
	template< bool isOtherConst >
	bool ATC_GRID_BASIC_TILE::operator!=( const BasicTile< isOtherConst >& other ) const
	{
		requires( m_grid == other.m_grid );
		return ( m_position != other.m_position );
	}


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID_BASIC_TILE::TileType ATC_GRID_BASIC_TILE::getRelativeTile( TileOffset offsetX, TileOffset offsetY ) const
	{
		return m_grid->getTile( getX() + offsetX, getY() + offsetY );
	}


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID_BASIC_TILE::TileType ATC_GRID_BASIC_TILE::getRelativeTile( const TileVector& offset ) const
	{
		return getRelativeTile( offset.x, offset.y );
	}


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID_BASIC_TILE::TileType ATC_GRID_BASIC_TILE::getAdjacentTile( CardinalDirection direction ) const
	{
		return getRelativeTile( GridType::getDirectionVector( direction ) );
	}


	ATC_GRID_TILE_TEMPLATE
	typename const ATC_GRID::TileVector& ATC_GRID_BASIC_TILE::getPosition() const
	{
		return m_position;
	}


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID::TileOffset ATC_GRID_BASIC_TILE::getX() const
	{
		return m_position.x;
	}


	ATC_GRID_TILE_TEMPLATE
	typename ATC_GRID::TileOffset ATC_GRID_BASIC_TILE::getY() const
	{
		return m_position.y;
	}


	ATC_GRID_TILE_TEMPLATE
	bool ATC_GRID_BASIC_TILE::isValid() const
	{
		return ( m_grid->contains( m_position ) );
	}


	// ------------------------------ Grid ------------------------------

	ATC_GRID_TEMPLATE
	ATC_GRID::Grid() :
		m_width( 0u ),
		m_height( 0u )
	{ }


	ATC_GRID_TEMPLATE
	ATC_GRID::Grid( unsigned int width, unsigned int height, const TileData& fillTile ) :
		m_width( width ),
		m_height( height )
	{
		// Fill the grid with the fill tile.
		clear( fillTile );
	}


	ATC_GRID_TEMPLATE
	ATC_GRID::~Grid() { }


	ATC_GRID_TEMPLATE
	typename ATC_GRID::TileVector ATC_GRID::getDirectionVector( CardinalDirection direction )
	{
		TileVector result( 0, 0 );

		switch( direction )
		{
		case CARDINAL_DIRECTION_EAST:
			result.x += 1;
			break;

		case CARDINAL_DIRECTION_NORTH:
			result.y -= 1;
			break;

		case CARDINAL_DIRECTION_WEST:
			result.x -= 1;
			break;

		case CARDINAL_DIRECTION_SOUTH:
			result.y += 1;
			break;
		}

		return result;
	}


	ATC_GRID_TEMPLATE
	void ATC_GRID::resize( size_t width, size_t height )
	{
		requires( width <= MAX_WIDTH && height <= MAX_HEIGHT );
		m_width = width;
		m_height = height;
	}


	ATC_GRID_TEMPLATE
	void ATC_GRID::clear( const TileData& fillTile )
	{
		std::fill_n( m_tiles, MAX_TILES, fillTile );
	}


	ATC_GRID_TEMPLATE
	bool ATC_GRID::contains( const TileVector& position ) const
	{
		return ( ( position.x >= 0 && position.x < (TileOffset) m_width ) &&
				 ( position.y >= 0 && position.y < (TileOffset) m_height ) );
	}


	ATC_GRID_TEMPLATE
	typename ATC_GRID::Tile ATC_GRID::getTile( TileOffset x, TileOffset y )
	{
		return Tile( this, x, y );
	}


	ATC_GRID_TEMPLATE
	typename ATC_GRID::ConstTile ATC_GRID::getTile( TileOffset x, TileOffset y ) const
	{
		return ConstTile( this, x, y );
	}


	ATC_GRID_TEMPLATE
	typename ATC_GRID::Tile ATC_GRID::getTile( const TileVector& position )
	{
		return getTile( position.x, position.y );
	}


	ATC_GRID_TEMPLATE
	typename ATC_GRID::ConstTile ATC_GRID::getTile( const TileVector& position ) const
	{
		return getTile( position.x, position.y );
	}


	ATC_GRID_TEMPLATE
	size_t ATC_GRID::getWidth() const
	{
		return m_width;
	}


	ATC_GRID_TEMPLATE
	size_t ATC_GRID::getHeight() const
	{
		return m_height;
	}
}

#endif