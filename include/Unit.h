#ifndef ATC_UNIT_H
#define ATC_UNIT_H

namespace atc
{
	/**
	 * Basic controllable Actor.
	 */
	class Unit : public Actor, public Orderable
	{
	public:
		static const Color DEFAULT_COLOR;
		static const Color ARROW_COLOR;
		static const Color TARGET_COLOR;
		static const int COLLISION_RADIUS_SUBDIVISIONS = 12;
		static const float COLLISION_RADIUS;
		static const float COLLISION_IMPULSE;
		static const float MOVEMENT_SPEED;
		static const float TRACE_RADIUS;
		static const int MAX_OPTIMIZATION_STEPS = 2;
		static const int MAX_SHORTCUT_LOOKAHEAD = 16;
		static const float APPROACHING_FORMATION_BOOST_AMOUNT;

		Unit();
		virtual ~Unit();

		virtual void update( double elapsedTime );
		virtual void draw( Renderer* renderer );
		virtual void onCollision( Actor* other, const Vector& displacement, float collisionDistance );

		void setTargetLocation( const Point& target );
		Point getTargetLocation() const;
		virtual void orderMoveTo( const Point& destination );
		bool canMoveDirectlyTo( const Point& destination );
		bool isApproachingFormation() const;

		void setFacingAngle( Angle facingAngle );
		Angle getFacingAngle() const;

		void clearCurrentPath();
		void setCurrentPath( const Path& path );
		Path getCurrentPath() const;

		bool hasFormation() const;
		Formation* getFormation() const;
		bool hasFormationSlot() const;
		int getFormationSlotIndex() const;
		float getDistanceSquaredToFormation() const;
		float getDistanceToFormation() const;
		float getDistanceSquaredToSlot() const;
		float getDistanceToSlot() const;

		Map::TileVector getTilePosition() const;
		Map::Tile getCurrentTile() const;

	protected:
		virtual void onSpawn( const Point& position );

		void setFormation( Formation* formation );
		void setFormationSlotIndex( int slotIndex );
		void onAssignedToSlot();
		void onEvictedFromSlot();

		void updateTargetLocation();
		void collideWithWalls();

		int m_formationSlotIndex;
		int m_currentPathRequestIndex;
		Formation* m_formation;
		Angle m_facingAngle;
		Path m_currentPath;
		Point m_targetLocation;

		friend class World;
		friend class Formation;
	};
}

#endif