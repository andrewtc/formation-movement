#include "common.h"
#include "Window.h"

namespace atc
{
	const float Window::SCROLL_REGION_OFFSET = 50.0;


	Window::Window( const char* title, bool isFullscreen ) :
		m_window( nullptr ),
		m_title( title ),
		m_isFullscreen( isFullscreen ),
		m_zoomLevel( 0 )
	{ }


	Window::~Window()
	{
		destroy();
	}


	void Window::init()
	{
		// Destroy any previous window handle.
		destroy();

		int width = DEFAULT_WIDTH;
		int height = DEFAULT_HEIGHT;
		GLFWmonitor* monitor = NULL;

		if( m_isFullscreen )
		{
			// If fullscreen mode is set, show the window in the primary monitor.
			monitor = glfwGetPrimaryMonitor();

			// Use the size of the primary monitor as the window size.
			const GLFWvidmode* videoMode = glfwGetVideoMode( monitor );
			width = videoMode->width;
			height = videoMode->height;
		}

		// Open a new window.
		glfwWindowHint( GLFW_RESIZABLE, 0 );
		m_window = glfwCreateWindow( width, height, m_title, monitor, NULL );
		glfwSetWindowUserPointer( m_window, this );
		glfwMakeContextCurrent( m_window );

		// Set input callbacks.
		glfwSetMouseButtonCallback( m_window, mouseEvent );
		glfwSetKeyCallback( m_window, keyEvent );

		// Set the size of the View.
		Renderer* renderer = g_app.getRenderer();
		requires( renderer );
		renderer->setViewSize( Vector( width, height ) );
	}


	void Window::destroy()
	{
		if( m_window )
		{
			glfwDestroyWindow( m_window );
			m_window = nullptr;
		}
	}


	void Window::setFullscreen( bool isFullscreen )
	{
		bool wasFullscreen = m_isFullscreen;
		m_isFullscreen = isFullscreen;

		if( wasFullscreen != isFullscreen )
		{
			// If the fullscreen mode changed, reinitialize the window.
			init();
		}
	}


	void Window::swapBuffers()
	{
		glfwSwapBuffers( m_window );
	}


	void Window::updateInput()
	{
		Point mousePosition = getMousePosition();

		if( ( mousePosition - m_lastMousePosition ).getLengthSquared() != 0.0f )
		{
			onMouseMove();
			m_lastMousePosition = mousePosition;
		}
	}


	void Window::onMouseDown( int button, int modifiers )
	{
		// TODO: Use event handling to move this code elsewhere.

		// Register the mouse button press.
		m_mouseButtonStates[ button ].isDown = true;

		if( button == GLFW_MOUSE_BUTTON_RIGHT )
		{
			// Get the current unit selection.
			World* world = g_app.getWorld();
			UnitSelection& selection = world->getUnitSelection();
			Point mouseLocation = world->getMouseWorldPosition();

			if( world->getMapTileAtPosition( mouseLocation ).isValid() )
			{
				// Issue a move order to the current selection of units.
				selection.orderMoveTo( mouseLocation );
			}
		}
	}


	void Window::onMouseUp( int button, int modifiers )
	{
		// TODO: Use event handling to move this code elsewhere.

		MouseButtonState& mouseButtonState = m_mouseButtonStates[ button ];

		if( button == GLFW_MOUSE_BUTTON_LEFT )
		{
			// Get the current unit selection.
			World* world = g_app.getWorld();
			UnitSelection& selection = world->getUnitSelection();

			// Get the Unit underneath the mouse world coordinates (if any).
			Point mouseLocation = world->getMouseWorldPosition();
			Unit* unitUnderMouse = world->getUnitAtLocation( mouseLocation );
			bool unitUnderMouseWasSelected = false;

			if( unitUnderMouse != nullptr )
			{
				// If there is a unit under the mouse, keep track of whether it was selected before the event.
				unitUnderMouseWasSelected = selection.containsUnit( unitUnderMouse );
			}

			if( !( modifiers & GLFW_MOD_SHIFT ) )
			{
				// Unless the shift key is held, clear the current selection.
				selection.clear();
			}

			if( mouseButtonState.isDragging )
			{
				Camera* camera = world->getCamera();

				if( camera )
				{
					Point mouseDragStartLocation = camera->deviceToWorldCoords( mouseButtonState.dragStartPosition );
					Point mouseDragEndLocation = camera->deviceToWorldCoords( getMousePosition() );
					UnitSelection unitsInArea;

					// If the left mouse was dragging, select all units in the area provided.
					world->getAllUnitsInArea( mouseDragStartLocation, mouseDragEndLocation, unitsInArea );
					selection.addUnits( unitsInArea );
					//world->setUnitSelection( unitsInArea );
				}
			}
			else if( unitUnderMouse != nullptr )
			{
				// If this is a click, get the Unit's current formation (if any).
				Formation* formation = unitUnderMouse->getFormation();

				if( formation && unitUnderMouseWasSelected )
				{
					// If the unit is in a formation and was already selected, select all units in
					// the same formation.
					for( int i = 0; i < formation->getUnitCount(); ++i )
					{
						selection.addUnit( formation->getUnitByIndex( i ) );
					}
				}
				else
				{
					// If the unit was NOT already selected, add the unit to the current selection.
					selection.addUnit( unitUnderMouse );
				}
			}
		}

		// Register that the mouse is no longer down.
		mouseButtonState.isDown = false;
		mouseButtonState.isDragging = false;
	}


	void Window::onMouseMove()
	{
		for( int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i )
		{
			MouseButtonState& mouseButtonState = m_mouseButtonStates[ i ];

			if( mouseButtonIsDown( i ) && !mouseButtonState.isDragging )
			{
				// If a mouse button is currently held and is not already dragging,
				// start dragging for that mouse button.
				mouseButtonState.isDragging = true;
				mouseButtonState.dragStartPosition = getMousePosition();
			}
		}
	}


	void Window::onKeyDown( int keyCode, int modifiers )
	{
		switch( keyCode )
		{
		case GLFW_KEY_ESCAPE:
			close();
			break;

		case GLFW_KEY_F1:
			toggleFullscreen();
			break;

		case GLFW_KEY_EQUAL:
			modifyZoomLevel( 1 );
			break;

		case GLFW_KEY_MINUS:
			modifyZoomLevel( -1 );
			break;

		case GLFW_KEY_0:
			setZoomLevel( 0 );
			break;

		case GLFW_KEY_T:
			toggleTrace();
			break;

		case GLFW_KEY_P:
			g_app.togglePaused();
			break;
		}
	}


	void Window::onKeyUp( int keyCode, int modifiers )
	{
		// TODO
	}


	void Window::toggleTrace()
	{
		World* world = g_app.getWorld();

		if( !world->isTracing() )
		{
			// Start tracing.
			world->startTrace( world->getMouseWorldPosition() );
		}
		else
		{
			// End the trace.
			world->endTrace();
		}
	}
}