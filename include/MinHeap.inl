#ifndef ATC_MINHEAP_INL
#define ATC_MINHEAP_INL

namespace atc
{
	template< size_t capacity, typename key_t, typename value_t >
	FixedSizeMinHeap< capacity, key_t, value_t >::Pair::Pair()
	{ }


	template< size_t capacity, typename key_t, typename value_t >
	FixedSizeMinHeap< capacity, key_t, value_t >::Pair::Pair( const Key& key, Value& value ) :
		key( key ), value( value )
	{ }


	template< size_t capacity, typename key_t, typename value_t >
	FixedSizeMinHeap< capacity, key_t, value_t >::Pair::~Pair()
	{
		// Zero out the memory for this Pair.
		memset( this, 0, sizeof( Pair ) );
	}


	template< size_t capacity, typename key_t, typename value_t >
	bool FixedSizeMinHeap< capacity, key_t, value_t >::Pair::operator>( const Pair& other ) const
	{
		// Return whether this Pair has a greater key.
		return ( key > other.key );
	}


	template< size_t capacity, typename key_t, typename value_t >
	FixedSizeMinHeap< capacity, key_t, value_t >::FixedSizeMinHeap() :
	m_size( 0 )
	{
		// Zero out all memory.
		memset( m_pairs, 0, sizeof( m_pairs ) );
	}


	template< size_t capacity, typename key_t, typename value_t >
	FixedSizeMinHeap< capacity, key_t, value_t >::~FixedSizeMinHeap()
	{
		clear();
	}


	template< size_t capacity, typename key_t, typename value_t >
	void FixedSizeMinHeap< capacity, key_t, value_t >::insert( const Key& key, Value& value )
	{
		// Make sure we don't overflow the buffer.
		requires( m_size < ( CAPACITY - 1 ) );

		// Add the element to the end of the array.
		Node currentNode = getEndOfArray();
		( *currentNode ) = Pair( key, value );
		++m_size;

		// Re-balance the tree.
		bubbleUp( currentNode );

		promises( isValidHeap( getFirstNode() ) );
	}


	template< size_t capacity, typename key_t, typename value_t >
	void FixedSizeMinHeap< capacity, key_t, value_t >::update( const Key& key, Value& value )
	{
		Node nodeFound = nullptr;

		// Find the element in the array.
		for( size_t i = 0; i < m_size; ++i )
		{
			Node currentNode = getNode( i );

			if( currentNode->value == value )
			{
				nodeFound = currentNode;
				break;
			}
		}

		// If no matching node was found, don't do anything.
		if( nodeFound == nullptr ) return;

		// Change the value of the node in the tree.
		nodeFound->key = key;
		nodeFound->value = value;

		// Bubble the node up or down as necessary.
		heapify( nodeFound );
	}


	template< size_t capacity, typename key_t, typename value_t >
	value_t FixedSizeMinHeap< capacity, key_t, value_t >::popMinElement()
	{
		// Grab the topmost node.
		Node firstNode = getFirstNode();

		// Copy the value to pop.
		Value value = firstNode->value;

		Node endNode = getLastNode();

		if( m_size > 1 )
		{
			// Swap the element with the end of the array.
			swap( *firstNode, *endNode );
		}

		// Remove the value at the end.
		--m_size;

		// Destroy and erase the removed value.
		endNode->~Pair();

		// Re-balance the tree.
		if( m_size > 1 )
			bubbleDown( firstNode );

		promises( isValidHeap( firstNode ) );

		// Return the popped value.
		return value;
	}


	template< size_t capacity, typename key_t, typename value_t >
	value_t FixedSizeMinHeap< capacity, key_t, value_t >::peekMinElement() const
	{
		// Return the topmost value.
		return getFirstNode()->value;
	}


	template< size_t capacity, typename key_t, typename value_t >
	void FixedSizeMinHeap< capacity, key_t, value_t >::clear()
	{
		// Call destructor on all nodes.
		for( size_t i = 0; i < m_size; ++i )
		{
			getNode( i )->~Pair();
		}

		// Reset the array.
		m_size = 0;
	}


	template< size_t capacity, typename key_t, typename value_t >
	size_t FixedSizeMinHeap< capacity, key_t, value_t >::getCapacity() const
	{
		return capacity;
	}


	template< size_t capacity, typename key_t, typename value_t >
	size_t FixedSizeMinHeap< capacity, key_t, value_t >::getSize() const
	{
		return m_size;
	}


	template< size_t capacity, typename key_t, typename value_t >
	bool FixedSizeMinHeap< capacity, key_t, value_t >::isEmpty() const
	{
		return ( m_size == 0 );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::Node FixedSizeMinHeap< capacity, key_t, value_t >::getFirstNode()
	{
		return &( m_pairs[ 0 ] );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::ConstNode FixedSizeMinHeap< capacity, key_t, value_t >::getFirstNode() const
	{
		return &( m_pairs[ 0 ] );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::Node FixedSizeMinHeap< capacity, key_t, value_t >::getLastNode()
	{
		return &( m_pairs[ m_size - 1 ] );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::ConstNode FixedSizeMinHeap< capacity, key_t, value_t >::getLastNode() const
	{
		return &( m_pairs[ m_size - 1 ] );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::Node FixedSizeMinHeap< capacity, key_t, value_t >::getEndOfArray()
	{
		return &( m_pairs[ m_size ] );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::ConstNode FixedSizeMinHeap< capacity, key_t, value_t >::getEndOfArray() const
	{
		return &( m_pairs[ m_size ] );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::Node FixedSizeMinHeap< capacity, key_t, value_t >::getNode( int index )
	{
		return ( getFirstNode() + index );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::ConstNode FixedSizeMinHeap< capacity, key_t, value_t >::getNode( int index ) const
	{
		return ( getFirstNode() + index );
	}


	template< size_t capacity, typename key_t, typename value_t >
	size_t FixedSizeMinHeap< capacity, key_t, value_t >::getIndexOfNode( ConstNode node ) const
	{
		return ( node - getFirstNode() );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::Node FixedSizeMinHeap< capacity, key_t, value_t >::getParentOfNode( Node node )
	{
		return getNode( ( getIndexOfNode( node ) - 1 ) >> 1 );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::ConstNode FixedSizeMinHeap< capacity, key_t, value_t >::getParentOfNode( ConstNode node ) const
	{
		return getNode( ( getIndexOfNode( node ) - 1 ) >> 1 );
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::Node FixedSizeMinHeap< capacity, key_t, value_t >::getFirstChildOfNode( Node node )
	{
		return getNode( ( node - getFirstNode() ) << 1 ) + 1;
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::ConstNode FixedSizeMinHeap< capacity, key_t, value_t >::getFirstChildOfNode( ConstNode node ) const
	{
		return getNode( ( node - getFirstNode() ) << 1 ) + 1;
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::Node FixedSizeMinHeap< capacity, key_t, value_t >::getSecondChildOfNode( Node node )
	{
		return getNode( ( node - getFirstNode() ) << 1 ) + 2;
	}


	template< size_t capacity, typename key_t, typename value_t >
	typename FixedSizeMinHeap< capacity, key_t, value_t >::ConstNode FixedSizeMinHeap< capacity, key_t, value_t >::getSecondChildOfNode( ConstNode node ) const
	{
		return getNode( ( node - getFirstNode() ) << 1 ) + 2;
	}


	template< size_t capacity, typename key_t, typename value_t >
	void FixedSizeMinHeap< capacity, key_t, value_t >::swap( Pair& first, Pair& second )
	{
		// Swap the two values (using the cache-coherent swap buffer).
		m_swapPair = second;
		second = first;
		first = m_swapPair;
	}


	template< size_t capacity, typename key_t, typename value_t >
	void FixedSizeMinHeap< capacity, key_t, value_t >::heapify( Node node )
	{
		Node parentNode = getParentOfNode( node );

		if( isValidNode( parentNode ) && ( ( *parentNode ) > ( *node ) ) )
		{
			bubbleUp( node );
		}
		else
		{
			bubbleDown( node );
		}
	}


	template< size_t capacity, typename key_t, typename value_t >
	void FixedSizeMinHeap< capacity, key_t, value_t >::bubbleUp( Node node )
	{
		// Move the node as far up the tree as possible.
		while( getIndexOfNode( node ) > 0 )
		{
			Node parentNode = getParentOfNode( node );

			// Make sure the parent is farther up in the tree.
			requires( parentNode < node );

			if( ( *parentNode ) > ( *node ) )
			{
				// If the current node is less than its parent, swap the two.
				swap( *node, *parentNode );

				// Continue re-balancing from the parent node.
				node = parentNode;
			}
			else break;
		}
	}


	template< size_t capacity, typename key_t, typename value_t >
	void FixedSizeMinHeap< capacity, key_t, value_t >::bubbleDown( Node node )
	{
		// Move the node as far down the tree as necessary.
		while( isValidNode( node ) )
		{
			Node firstChildNode = getFirstChildOfNode( node );
			Node secondChildNode = getSecondChildOfNode( node );

			// Make sure the child nodes are farther down the tree.
			requires( firstChildNode > node );
			requires( secondChildNode > node );

			bool hasFirstChild = isValidNode( firstChildNode );
			bool hasSecondChild = isValidNode( secondChildNode );

			Node nodeToCheck = nullptr;

			if( !hasFirstChild )
			{
				// If this node is a leaf, stop bubbling.
				break;
			}
			else
			{
				if( hasSecondChild && ( ( *firstChildNode ) > ( *secondChildNode ) ) )
				{
					// If this node has two children and the second child is
					// smaller, check the second child.
					nodeToCheck = secondChildNode;
				}
				else
				{
					// If this node only has one child or the first child is
					// greater than the second, check the first child.
					nodeToCheck = firstChildNode;
				}

				if( ( *node ) > ( *nodeToCheck ) )
				{
					// If the chosen child node is less than its parent, swap the two.
					swap( *nodeToCheck, *node );

					// Continue re-balancing from the child node.
					node = nodeToCheck;
				}
				else break;
			}
		}
	}


	template< size_t capacity, typename key_t, typename value_t >
	bool FixedSizeMinHeap< capacity, key_t, value_t >::isValidNode( ConstNode node ) const
	{
		size_t nodeIndex = getIndexOfNode( node );
		return ( ( nodeIndex >= 0 ) && ( nodeIndex < m_size ) );
	}


	template< size_t capacity, typename key_t, typename value_t >
	bool FixedSizeMinHeap< capacity, key_t, value_t >::isValidHeap( ConstNode node ) const
	{
		// If the size of this heap is less than 2, it must be a valid heap.
		if( m_size < 2 ) return true;

		ConstNode parentNode = getParentOfNode( node );
		ConstNode firstChildNode = getFirstChildOfNode( node );
		ConstNode secondChildNode = getSecondChildOfNode( node );

		// Return whether this node is greater than or equal to its parent.
		if( isValidNode( parentNode ) && ( *parentNode > *node ) )
		{
			int parentIndex = getIndexOfNode( parentNode );
			int nodeIndex = getIndexOfNode( node );
			return false;
		}

		// Ensure that each child node is a valid heap.
		if( isValidNode( firstChildNode ) && ( !isValidHeap( firstChildNode ) ) )
			return false;
		if( isValidNode( secondChildNode ) && ( !isValidHeap( secondChildNode ) ) )
			return false;

		return true;
	}
}

#endif