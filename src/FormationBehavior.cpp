#include "common.h"
#include "FormationBehavior.h"

namespace atc
{
	// ------------------------------ FormationBehavior ------------------------------
	const float FormationBehavior::ASSIGN_UNIT_TO_SLOT_DISTANCE_PADDING = 1.0f;


	FormationBehavior::FormationBehavior() :
		m_formation( nullptr ),
		m_assignUnitToSlotDistance( 0 )
	{ }


	FormationBehavior::~FormationBehavior() { }


	// ------------------------------ BoxFormationBehavior ------------------------------

	BoxFormationBehavior::BoxFormationBehavior( float spacing ) :
		m_spacing( spacing ), m_rows( 0 ), m_columns( 0 )
	{
		// Make sure a valid spacing value was passed.
		requires( spacing > 0.0f );
	}


	BoxFormationBehavior::~BoxFormationBehavior() { }


	void BoxFormationBehavior::recalculate()
	{
		requires( m_formation );

		// Determine how many units are in the formation.
		int unitCount = m_formation->getUnitCount();

		if( unitCount > 0 )
		{
			// If there is at least one unit in the formation, determine how many columns to form.
			m_columns = (int) std::ceilf( std::sqrt( (float) unitCount ) );
			m_rows = (int) std::ceilf( (float) unitCount / m_columns );

			// Set the assignment distance based on the size of the Formation.
			setAssignUnitToSlotDistance( sqrtf( ( m_rows * m_rows ) + ( m_columns * m_columns ) ) + ASSIGN_UNIT_TO_SLOT_DISTANCE_PADDING );

			// Determine the starting offset of the formation.
			Vector offset = ( Vector( (float) ( m_columns - 1 ) * -0.5f, (float) ( m_rows - 1 ) * -0.5f ) * ( m_spacing ) );

			for( int i = 0, row = 0, column = 0; i < unitCount; ++i )
			{
				// Add slots for each unit.
				Vector slotPosition = ( offset + ( Vector( (float) column, (float) ( m_rows - row - 1 ) ) * m_spacing ) );
				m_formation->addSlot( slotPosition );

				// Lay out as many slots in a row as possible before starting another.
				++column;

				if( column >= m_columns )
				{
					column = 0;
					++row;
				}
			}
		}
	}


	void BoxFormationBehavior::reassignSlots()
	{
		std::multimap< float, Unit* > unitsByHeight;
		std::multimap< float, Unit* > unitsInRow;

		float assignDistanceSquared = getAssignUnitToSlotDistanceSquared();

		for( int y = 0, count = (int) m_formation->getUnitCount(); y < count; ++y )
		{
			Unit* unit = m_formation->getUnitByIndex( y );
			float distanceSquaredToFormation = unit->getDistanceSquaredToFormation();

			if( distanceSquaredToFormation <= assignDistanceSquared )
			{
				// Get the relative position to the Formation.
				Point relativePosition = m_formation->getRelativePosition( unit->getPosition() );

				// Sort all units in order of relative Y-value (if they are in range).
				unitsByHeight.insert( std::make_pair( -relativePosition.y, unit ) );
			}
		}

		for( int y = 0; y < m_rows; ++y )
		{
			while( unitsInRow.size() < m_columns && !unitsByHeight.empty() )
			{
				// Pop the next Unit.
				auto begin = unitsByHeight.begin();
				Unit* unit = begin->second;
				unitsByHeight.erase( begin );

				// Get the relative position to the Formation.
				Point relativePosition = m_formation->getRelativePosition( unit->getPosition() );

				// Assign units to this row in order of relative X-value.
				unitsInRow.insert( std::make_pair( relativePosition.x, unit ) );
			}

			for( int x = 0, count = unitsInRow.size(); x < count; ++x )
			{
				// Pop the next Unit.
				auto begin = unitsInRow.begin();
				Unit* unit = begin->second;
				unitsInRow.erase( begin );

				// Calculate the slot index for this Unit.
				int slotIndex = ( ( y * m_columns ) + x );

				// Assign units to slots in the row by order of X-value.
				m_formation->assignUnitToSlot( unit, slotIndex );
			}
		}
	}
}