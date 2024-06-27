#ifndef ATC_FORMATIONBEHAVIOR_H
#define ATC_FORMATIONBEHAVIOR_H

namespace atc
{
	/**
	 * Abstract base class for an object that handles assigning units
	 * to slots within a Formation.
	 */
	class FormationBehavior
	{
	public:
		static const float ASSIGN_UNIT_TO_SLOT_DISTANCE_PADDING;

		virtual ~FormationBehavior();

		/**
		 * Creates slots for each unit in the Formation and assigns each Unit to a slot.
		 */
		virtual void recalculate() = 0;

		/**
		 * 
		 */
		virtual void reassignSlots() = 0;

		Formation* getFormation() const;
		void setAssignUnitToSlotDistance( float distance );
		float getAssignUnitToSlotDistance() const;
		float getAssignUnitToSlotDistanceSquared() const;

	protected:
		FormationBehavior();

		void setFormation( Formation* formation );

		Formation* m_formation;
		float m_assignUnitToSlotDistance;

		friend class Formation;
	};


	/**
	 * Arranges units in a Formation into ranks and files.
	 */
	class BoxFormationBehavior : public FormationBehavior
	{
	public:
		BoxFormationBehavior( float spacing );
		virtual ~BoxFormationBehavior();

		virtual void recalculate();
		virtual void reassignSlots();

	protected:
		int m_rows, m_columns;
		float m_spacing;
	};
}

#endif