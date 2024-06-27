#include "common.h"

namespace atc
{
	const float Actor::POSITION_TOLERANCE_SQUARED = 0.01f;


	Actor::Actor( bool collisionEnabled ) :
		m_isCollisionEnabled( collisionEnabled ),
		m_isAlive( false ),
		m_needsRemoval( false ),
		m_ID( -1 ),
		m_collisionRadius( 0.0f ),
		m_world( nullptr )
	{ }


	Actor::~Actor()
	{

	}


	void Actor::init()
	{
		m_isAlive = true;
	}


	void Actor::kill()
	{
		m_isAlive = false;
	}


	void Actor::remove()
	{
		m_needsRemoval = true;
	}


	void Actor::update( double elapsedTime )
	{
		// Move the unit by its velocity each frame.
		m_position += ( m_velocity * (float) elapsedTime );

		// Clamp position to the bounds of the world.
		float worldLeft   = m_world->getLeft();
		float worldRight  = m_world->getRight();
		float worldBottom = m_world->getBottom();
		float worldTop    = m_world->getTop();

		m_position.x = Math::clamp( m_position.x, worldLeft, worldRight );
		m_position.y = Math::clamp( m_position.y, worldBottom, worldTop );
	}


	void Actor::draw( Renderer* renderer ) { }


	void Actor::onCollision( Actor* other, const Vector& displacement, float collisionDistance )
	{
		// By default, do nothing.
	}


	void Actor::translate( const Vector& translation )
	{
		m_position += translation;
	}
}