namespace atc
{
	inline void FormationBehavior::setFormation( Formation* formation )
	{
		m_formation = formation;
	}


	inline Formation* FormationBehavior::getFormation() const
	{
		return m_formation;
	}


	inline void FormationBehavior::setAssignUnitToSlotDistance( float distance )
	{
		m_assignUnitToSlotDistance = std::max( distance, 0.0f );
	}


	inline float FormationBehavior::getAssignUnitToSlotDistance() const
	{
		return m_assignUnitToSlotDistance;
	}


	inline float FormationBehavior::getAssignUnitToSlotDistanceSquared() const
	{
		return ( m_assignUnitToSlotDistance * m_assignUnitToSlotDistance );
	}
}