#include "common.h"
#include "Camera.h"

namespace atc
{
	const float Camera::SCROLL_SPEED = 75.0f;


	Camera::Camera() :
		m_zoom( 1.0f )
	{

	}


	Camera::~Camera()
	{

	}


	void Camera::update( double elapsedTime )
	{
		if( isCurrent() )
		{
			Window* window = g_app.getWindow();
			Vector scrollDirection;

			if( window->isFullscreen() )
			{
				// If in fullscreen mode, scroll the camera based on mouse position.
				scrollDirection = window->getScrollDirection();
			}

			// If any arrow keys are held, scroll.
			if( window->keyIsHeld( GLFW_KEY_LEFT ) )
				scrollDirection.x -= 1.0f;
			if( window->keyIsHeld( GLFW_KEY_RIGHT ) )
				scrollDirection.x += 1.0f;
			if( window->keyIsHeld( GLFW_KEY_DOWN ) )
				scrollDirection.y -= 1.0f;
			if( window->keyIsHeld( GLFW_KEY_UP ) )
				scrollDirection.y += 1.0f;

			// Scroll the camera.
			setVelocity( Vector( scrollDirection.x, scrollDirection.y ) * SCROLL_SPEED * ( TILE_SCALE / m_zoom ) );
		}

		Actor::update( elapsedTime );
	}


	void Camera::apply()
	{
		// Center the view on the camera.
		g_app.getRenderer()->setView( m_position, m_zoom );
	}
}