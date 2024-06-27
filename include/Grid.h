#ifndef ATC_GRID_H
#define ATC_GRID_H


#define ATC_GRID \
	Grid< tileData_t,\
		  tileOffset_t,\
		  maxDimensionPowerOfTwo >


namespace atc
{
	/**
	 * Represents a grid direction (i.e. north, south, east, or west).
	 */
	enum CardinalDirection
	{
		CARDINAL_DIRECTION_NONE,
		CARDINAL_DIRECTION_EAST,
		CARDINAL_DIRECTION_NORTH,
		CARDINAL_DIRECTION_WEST,
		CARDINAL_DIRECTION_SOUTH
	};

	const size_t CARDINAL_DIRECTION_COUNT = 4;


	CardinalDirection getOppositeDirection( CardinalDirection direction );
	CardinalDirection getCounterClockwiseDirection( CardinalDirection direction );


	/**
	 * Data structure that compactly stores a grid of square tiles and
	 * provides random access to tiles by row and column.
	 */
	template< typename tileData_t,
			  typename tileOffset_t = short,
			  size_t maxDimensionPowerOfTwo = 8 >
	class Grid
	{
		static_assert( std::is_integral< tileOffset_t >::value && std::is_signed< tileOffset_t >::value, "Tile offset type must be a signed integral number." );
		static_assert( ( sizeof( tileOffset_t ) * CHAR_BIT ) > maxDimensionPowerOfTwo, "Tile offset type is not large enough to index all grid tiles." );
		static_assert( ( sizeof( tileOffset_t ) <= sizeof( size_t ) ), "Tile offset type cannot be larger than size_t type." );

	public:
		static const size_t MAX_DIMENSION_POWER_OF_TWO = maxDimensionPowerOfTwo;
		static const size_t MAX_WIDTH = ( 1u << MAX_DIMENSION_POWER_OF_TWO );
		static const size_t MAX_HEIGHT = ( 1u << MAX_DIMENSION_POWER_OF_TWO );
		static const size_t MAX_TILES = ( MAX_WIDTH * MAX_HEIGHT );

		typedef ATC_GRID GridType;

		typedef tileOffset_t TileOffset;
		typedef tileData_t TileData;

		/**
		 * Represents a 2D vector offset in tile coordinates.
		 */
		struct TileVector
		{
			TileVector();
			TileVector( TileOffset x, TileOffset y );
			~TileVector();

			static TileOffset getManhattanDistance( const TileVector& first, const TileVector& second );

			bool operator==( const TileVector& other ) const;
			bool operator!=( const TileVector& other ) const;
			void operator+=( const TileVector& other );
			void operator-=( const TileVector& other );
			TileVector operator+( const TileVector& other ) const;
			TileVector operator-( const TileVector& other ) const;

			TileOffset x;
			TileOffset y;
		};

		/**
		 * Represents an element of data stored within a Grid.
		 */
		template< bool isConst >
		class BasicTile
		{
		public:
			typedef BasicTile< isConst > TileType;
			typedef typename std::conditional< isConst, const GridType, GridType >::type TileGridType;
			typedef typename std::conditional< isConst, const TileData, TileData >::type TileDataType;

			BasicTile();
			BasicTile( TileGridType* grid, TileOffset x, TileOffset y );
			BasicTile( TileGridType* grid, const TileVector& position );
			BasicTile( const BasicTile< false >& other );
			~BasicTile();

			TileGridType* getGrid() const;
			TileDataType& getData() const;
			TileDataType& operator*() const;
			TileDataType* operator->() const;

			template< bool isOtherConst >
			bool operator==( const BasicTile< isOtherConst >& other ) const;

			template< bool isOtherConst >
			bool operator!=( const BasicTile< isOtherConst >& other ) const;

			TileType getRelativeTile( TileOffset offsetX, TileOffset offsetY ) const;
			TileType getRelativeTile( const TileVector& offset ) const;
			TileType getAdjacentTile( CardinalDirection direction ) const;
			const TileVector& getPosition() const;
			TileOffset getX() const;
			TileOffset getY() const;
			bool isValid() const;

		protected:
			TileGridType* m_grid;
			TileVector m_position;

			friend class BasicTile;
		};

		typedef BasicTile< false > Tile;
		typedef BasicTile< true > ConstTile;

		Grid();
		Grid( unsigned int width, unsigned int height,
			  const TileData& fillTile = TileData() );
		~Grid();

		static TileVector getDirectionVector( CardinalDirection direction );

		void resize( size_t width, size_t height );
		void clear( const TileData& fillTile = TileData() );
		bool contains( const TileVector& position ) const;
		Tile getTile( TileOffset x, TileOffset y );
		ConstTile getTile( TileOffset x, TileOffset y ) const;
		Tile getTile( const TileVector& position );
		ConstTile getTile( const TileVector& position ) const;

		size_t getWidth() const;
		size_t getHeight() const;

	protected:
		size_t m_width;
		size_t m_height;
		TileData m_tiles[ MAX_TILES ];
	};
}

#endif