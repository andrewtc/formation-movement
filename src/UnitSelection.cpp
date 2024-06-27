#include "common.h"
#include "UnitSelection.h"

namespace atc
{
	const float UnitSelection::SELECTION_CIRCLE_PADDING = 0.25f;


	UnitSelection::UnitSelection()
	{

	}


	UnitSelection::~UnitSelection()
	{

	}


	void UnitSelection::draw( Renderer* renderer )
	{
		glColor3f( 0.0f, 1.0f, 1.0f );

		for( auto it = m_unitsByID.begin(); it != m_unitsByID.end(); ++it )
		{
			Unit* unit = it->second;
			Point position = unit->getPosition();

			renderer->pushTransform( Vector( position.x, position.y ) );
			renderer->drawCircle( unit->getCollisionRadius() + SELECTION_CIRCLE_PADDING, Unit::COLLISION_RADIUS_SUBDIVISIONS );
			renderer->popTransform();
		}
	}


	void UnitSelection::orderMoveTo( const Point& destination )
	{
		Formation* formation = nullptr;

		if( getUnitCount() >= 1 )
		{
			// If there are two or more units in this selection, create a new Formation at the center of mass.
			// TODO: Get specific FormationBehavior type from current formation settings.
			Point centerOfMass = calculateCenterOfMass();
			formation = g_app.getWorld()->createFormation( centerOfMass, destination, new BoxFormationBehavior( 1.0f ) );
		}

		for( auto it = m_unitsByID.begin(); it != m_unitsByID.end(); ++it )
		{
			Unit* unit = it->second;

			if( unit->hasFormation() )
			{
				// Remove each unit from its current formation (if any).
				unit->getFormation()->removeUnit( unit );
			}

			if( formation != nullptr )
			{
				// If there is a new formation for these units, add each unit to it.
				formation->addUnit( unit );
			}

			// Let the Unit handle the order.
			unit->orderMoveTo( destination );
		}
	}


	Point UnitSelection::calculateCenterOfMass() const
	{
		Point sumOfPositions = Point::ZERO;

		for( auto it = m_unitsByID.begin(); it != m_unitsByID.end(); ++it )
		{
			// Sum the positions of all Units in the selection.
			Unit* unit = it->second;
			sumOfPositions += unit->getPosition();
		}

		// Divide the resultant vector by the number of Units to get the center of mass.
		return( sumOfPositions / (float) getUnitCount() );
	}


	float UnitSelection::calculateMaximumMoveSpeed() const
	{
		// TODO: Find the speed of the slowest Unit in the selection.
		return Unit::MOVEMENT_SPEED;
	}
}