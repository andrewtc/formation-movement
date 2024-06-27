namespace atc
{
	// ------------------------------ Formation ------------------------------

	inline Color Formation::getColorByIndex( int index )
	{
		return COLORS_BY_INDEX[ index % COLOR_COUNT ];
	}


	inline void Formation::setBehavior( FormationBehavior* behavior )
	{
		// Make sure a Behavior was specified.
		requires( behavior );

		// Set the behavior.
		m_behavior = behavior;

		// Let the behavior know that it belongs to this Formation.
		m_behavior->setFormation( this );

		// Recalculate all slots.
		m_wasModified = true;
	}


	inline FormationBehavior* Formation::getBehavior() const
	{
		return m_behavior;
	}


	inline bool Formation::containsUnit( Unit* unit )
	{
		return m_units.containsUnit( unit );
	}


	inline bool Formation::isValidSlotIndex( int index ) const
	{
		return ( index >= 0 && index < (int) m_slots.size() );
	}


	inline bool Formation::slotIsFree( int index ) const
	{
		requires( isValidSlotIndex( index ) );
		return m_slots[ index ].isFree();
	}


	inline bool Formation::slotIsTaken( int index ) const
	{
		requires( isValidSlotIndex( index ) );
		return m_slots[ index ].isTaken();
	}


	inline float Formation::getAssignUnitToSlotDistance() const
	{
		return sqrtf( getAssignUnitToSlotDistanceSquared() );
	}


	inline float Formation::getAssignUnitToSlotDistanceSquared() const
	{
		float result = 0.0f;

		if( m_behavior )
		{
			result = m_behavior->getAssignUnitToSlotDistanceSquared();
		}

		return result;
	}


	inline int Formation::getIndex() const
	{
		return m_index;
	}


	inline Color Formation::getColor() const
	{
		return m_color;
	}


	inline size_t Formation::getUnitCount() const
	{
		return m_units.getUnitCount();
	}


	inline Unit* Formation::getUnitByIndex( int index ) const
	{
		return m_units.getUnitByIndex( index );
	}


	inline Point Formation::getOrigin() const
	{
		return m_origin;
	}


	inline Angle Formation::getFacingAngle() const
	{
		return m_facingAngle;
	}


	inline Direction Formation::getFacing() const
	{
		return Direction( m_facingAngle );
	}


	inline Flowfield* Formation::getFlowfield() const
	{
		return m_flowfield;
	}


	inline bool Formation::hasFlowfield() const
	{
		return ( m_flowfield != nullptr );
	}


	// ------------------------------ Slot ------------------------------

	inline bool Formation::Slot::isFree() const
	{
		return ( unit == nullptr );
	}


	inline bool Formation::Slot::isTaken() const
	{
		return !( isFree() );
	}
}