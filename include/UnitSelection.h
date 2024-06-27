#ifndef ATC_UNITSELECTION_H
#define ATC_UNITSELECTION_H

#include <set>

namespace atc
{
	/**
	 * Encapsulates a selection of units.
	 */
	class UnitSelection : public Orderable
	{
	public:
		static const float SELECTION_CIRCLE_PADDING;

		UnitSelection();
		virtual ~UnitSelection();

		void draw( Renderer* renderer );

		void addUnit( Unit* unit );
		void addUnits( const UnitSelection& selection );
		void removeUnit( Unit* unit );
		bool containsUnit( Unit* unit );
		void clear();

		virtual void orderMoveTo( const Point& location );

		Point calculateCenterOfMass() const;
		float calculateMaximumMoveSpeed() const;

		size_t getUnitCount() const;
		Unit* getUnitByIndex( int index ) const;

	protected:
		std::map< Actor::ID, Unit* > m_unitsByID;
	};
}

#endif