#ifndef ATC_ACTOR_H
#define ATC_ACTOR_H

namespace atc
{
	/**
	 * Basic game object class.
	 */
	class Actor
	{
	public:
		typedef int ID;

		static const float POSITION_TOLERANCE_SQUARED;

		Actor( bool collisionEnabled = false );
		virtual ~Actor();

		void init();
		void kill();
		void remove();

		virtual void update( double elapsedTime );
		virtual void draw( Renderer* renderer );
		virtual void onCollision( Actor* other, const Vector& displacement, float collisionDistance );
		virtual void collideWithWalls() {}

		void translate( const Vector& translation );

		bool isAlive() const;
		bool isDead() const;
		ID getID() const;
		World* getWorld() const;

		void setPosition( const Point& position );
		Point getPosition() const;
		void setVelocity( const Vector& velocity );
		Vector getVelocity() const;
		bool isAtLocation( const Point& location ) const;
		bool isOverlappingLocation( const Point& location ) const;

		void setCollisionRadius( float collisionRadius );
		float getCollisionRadius() const;

		void setCollisionEnabled( bool collisionEnabled );
		bool isCollisionEnabled() const;

	private:
		bool m_isCollisionEnabled;
		bool m_isAlive;
		bool m_needsRemoval;
		ID m_ID;
		World* m_world;

	protected:
		float m_collisionRadius;
		Point m_position;
		Vector m_velocity;

		friend class World;
	};
}

#endif