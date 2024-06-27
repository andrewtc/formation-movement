#ifndef ATC_FORMATION_H
#define ATC_FORMATION_H

namespace atc
{
	class Flowfield;

	/**
	 * Manages movement AI for a group of units moving in formation.
	 */
	class Formation
	{
	public:
		static Color getColorByIndex( int index );

		void update( double elapsedTime );
		void draw( Renderer* renderer );

		void setBehavior( FormationBehavior* behavior );
		FormationBehavior* getBehavior() const;

		void addUnit( Unit* unit );
		void removeUnit( Unit* unit );
		void removeAllUnits();
		bool containsUnit( Unit* unit );

		void addSlot( const Vector& offset );
		void addSlot( float offsetX, float offsetY );
		void removeAllSlots();
		bool isValidSlotIndex( int index ) const;
		bool slotIsFree( int index ) const;
		bool slotIsTaken( int index ) const;
		int getFirstFreeSlotIndex() const;
		Point getSlotOffset( int index ) const;
		Point getSlotWorldLocation( int index ) const;
		Point getRelativePosition( const Point& worldPosition ) const;
		float calculateCohesion() const;
		float getAssignUnitToSlotDistance() const;
		float getAssignUnitToSlotDistanceSquared() const;

		void assignUnitToSlot( Unit* unit, int index );
		void evictUnitFromSlot( int index );
		void evictAllUnits();

		int getIndex() const;
		Color getColor() const;
		size_t getUnitCount() const;
		Unit* getUnitByIndex( int index ) const;
		Point getOrigin() const;
		Angle getFacingAngle() const;
		Direction getFacing() const;
		Flowfield* getFlowfield() const;
		bool hasFlowfield() const;

	protected:
		static const size_t COLOR_COUNT = 6;
		static const Color COLORS_BY_INDEX[ COLOR_COUNT ];

		static const float ORIGIN_POINT_DRAW_RADIUS;
		static const float SLOT_DRAW_RADIUS;
		static const int SLOT_DRAW_SUBDIVISIONS = 12;

		static const float CENTER_OF_MASS_DRAW_RADIUS;
		static const int CENTER_OF_MASS_DRAW_SUBDIVISIONS = 4;

		static const float FACING_ARROW_LENGTH;
		static const float FACING_ARROW_OFFSET;

		/**
		 * Represents a planned position for a Unit within the formation.
		 */
		struct Slot
		{
			Slot( const Vector& offset = Vector::ZERO );
			Slot( float offsetX, float offsetY );

			bool isFree() const;
			bool isTaken() const;

			Unit* unit;
			Vector offset;
		};

		Formation( World* world, int index, const Point& origin, const Point& destination, FormationBehavior* behavior, Color color );
		~Formation();

		void recalculate();
		void reassignSlots();

		bool m_wasModified;
		int m_index;
		World* m_world;
		FormationBehavior* m_behavior;
		Flowfield* m_flowfield;
		Color m_color;
		Point m_origin;
		Point m_destination;
		Angle m_facingAngle;
		UnitSelection m_units;
		std::vector< Slot > m_slots;

		friend class World;
		friend class FormationBehavior;
	};
}

#endif