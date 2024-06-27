#include "common.h"
#include "HUD.h"

namespace atc
{
	HUD::HUD()
	{
		m_font = Font::loadFromFile( "data/images/anonymous_pro_64pt.png", 95 );
	}


	HUD::~HUD() { }


	void HUD::update( double elapsedTime )
	{
	}


	void HUD::draw( Renderer* renderer )
	{
		// Draw in device coordinates.
		renderer->resetView();

		// Start drawing.
		renderer->beginDraw();

		// Draw hello world text.
		Vector windowSize = g_app.getWindow()->getSize();
		Point helloWorldPosition;
		helloWorldPosition += ( windowSize * -0.5f );

		glColor3f( 1.0f, 1.0f, 1.0f );
		renderer->renderText( m_font, "Hello, World!", helloWorldPosition, 32.0f );

		Window* window = g_app.getWindow();
		World* world = g_app.getWorld();

		if( window->mouseButtonIsDragging( GLFW_MOUSE_BUTTON_LEFT ) )
		{
			Camera* camera = world->getCamera();

			Point dragStart = window->getMouseButtonDragStartPosition( GLFW_MOUSE_BUTTON_LEFT );
			Point dragEnd = window->getMousePosition();

			if( camera != nullptr )
			{
				renderer->drawRectangle( dragStart, dragEnd );
			}
		}

		// Stop drawing.
		renderer->endDraw();
	}
}