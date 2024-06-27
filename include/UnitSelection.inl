namespace atc
{
	inline void UnitSelection::addUnit( Unit* unit )
	{
		auto it = m_unitsByID.find( unit->getID() );

		if( it == m_unitsByID.end() )
		{
			m_unitsByID[ unit->getID() ] = unit;
		}
	}


	inline void UnitSelection::addUnits( const UnitSelection& selection )
	{
		auto end = selection.m_unitsByID.end();

		for( auto it = selection.m_unitsByID.begin(); it != end; ++it )
		{
			// Add all units from the other selection to this one.
			addUnit( it->second );
		}
	}


	inline void UnitSelection::removeUnit( Unit* unit )
	{
		auto it = m_unitsByID.find( unit->getID() );

		if( it != m_unitsByID.end() )
		{
			m_unitsByID.erase( it );
		}
	}


	inline bool UnitSelection::containsUnit( Unit* unit )
	{
		return ( m_unitsByID.find( unit->getID() ) != m_unitsByID.end() );
	}


	inline void UnitSelection::clear()
	{
		m_unitsByID.clear();
	}


	inline size_t UnitSelection::getUnitCount() const
	{
		return m_unitsByID.size();
	}


	inline Unit* UnitSelection::getUnitByIndex( int index ) const
	{
		int currentIndex = 0;
		Unit* unit = nullptr;

		for( auto it = m_unitsByID.begin(); it != m_unitsByID.end(); ++it )
		{
			if( currentIndex == index )
			{
				unit = it->second;
				break;
			}

			++currentIndex;
		}

		return unit;
	}
}