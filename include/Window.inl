namespace atc
{
	inline void Window::toggleFullscreen()
	{
		setFullscreen( !m_isFullscreen );
	}


	inline bool Window::isFullscreen() const
	{
		return m_isFullscreen;
	}


	inline void Window::setZoomLevel( int zoomLevel )
	{
		// Clamp the zoom level.
		m_zoomLevel = Math::clamp( zoomLevel, MIN_ZOOM_LEVEL, MAX_ZOOM_LEVEL );

		// Set the zoom of the current camera to the new zoom level.
		g_app.getWorld()->getCamera()->setZoom( TILE_SCALE * powf( 2.0f, (float) m_zoomLevel ) );
	}


	inline void Window::modifyZoomLevel( int changeInZoomLevel )
	{
		setZoomLevel( m_zoomLevel + changeInZoomLevel );
	}


	inline int Window::getZoomLevel() const
	{
		return m_zoomLevel;
	}


	inline void Window::close()
	{
		// Flag that the window should close.
		glfwSetWindowShouldClose( m_window, 1 );
	}


	inline bool Window::isClosing() const
	{
		return ( glfwWindowShouldClose( m_window ) != 0 );
	}


	inline Point Window::getScreenPosition() const
	{
		int x, y;
		glfwGetWindowPos( m_window, &x, &y );
		return Point( (float) x, (float) y );
	}


	inline Vector Window::getSize() const
	{
		int width, height;
		glfwGetWindowSize( m_window, &width, &height );
		return Vector( (float) width, (float) height );
	}


	inline void Window::mouseEvent( GLFWwindow* windowHandle, int button, int action, int modifiers )
	{
		// Unpack the Window pointer tied to this window handle.
		Window* window = reinterpret_cast< Window* >( glfwGetWindowUserPointer( windowHandle ) );

		if( action == GLFW_PRESS )
		{
			window->onMouseDown( button, modifiers );
		}
		else
		{
			window->onMouseUp( button, modifiers );
		}
	}


	inline Point Window::getMousePosition() const
	{
		double mouseX, mouseY;
		glfwGetCursorPos( m_window, &mouseX, &mouseY );
		return screenToDeviceCoords( Point( (float) mouseX, (float) mouseY ) );
	}


	inline Vector Window::getScrollDirection() const
	{
		Vector result = Vector::ZERO;

		Vector windowSize = getSize();
		Point windowTopRight( windowSize.x * 0.5f, windowSize.y * 0.5f );
		Point windowBottomLeft = ( windowTopRight - windowSize );
		Point mousePosition = getMousePosition();

		float scrollRegionLeft   = ( windowBottomLeft.x + SCROLL_REGION_OFFSET );
		float scrollRegionRight  = ( windowTopRight.x - SCROLL_REGION_OFFSET );
		float scrollRegionBottom = ( windowBottomLeft.y + SCROLL_REGION_OFFSET );
		float scrollRegionTop    = ( windowTopRight.y - SCROLL_REGION_OFFSET );

		if( mousePosition.x <= scrollRegionLeft )
			result.x -= 1.0f;
		if( mousePosition.x >= scrollRegionRight )
			result.x += 1.0f;
		if( mousePosition.y <= scrollRegionBottom )
			result.y -= 1.0f;
		if( mousePosition.y >= scrollRegionTop )
			result.y += 1.0f;

		return result;
	}


	inline bool Window::mouseButtonIsUp(int button ) const
	{
		return !mouseButtonIsDown( button );
	}


	inline bool Window::mouseButtonIsDown(int button ) const
	{
		return m_mouseButtonStates[ button ].isDown == true;
	}


	inline bool Window::mouseButtonIsDragging(int button ) const
	{
		return m_mouseButtonStates[ button ].isDragging;
	}


	inline Point Window::getMouseButtonDragStartPosition(int button ) const
	{
		return m_mouseButtonStates[ button ].dragStartPosition;
	}


	inline Point Window::screenToDeviceCoords( const Point& screenCoords ) const
	{
		Vector windowHalfSize = ( getSize() * 0.5f );
		return Point( screenCoords.x - windowHalfSize.x, ( screenCoords.y - windowHalfSize.y ) * -1.0f );
	}


	inline Window::MouseButtonState::MouseButtonState() :
		isDragging( false )
	{ }


	inline void Window::keyEvent( GLFWwindow* windowHandle, int keyCode, int scanCode, int action, int modifiers )
	{
		// Unpack the Window pointer tied to this window handle.
		Window* window = reinterpret_cast< Window* >( glfwGetWindowUserPointer( windowHandle ) );

		if( action == GLFW_PRESS )
		{
			window->onKeyDown( keyCode, modifiers );
		}
		else
		{
			window->onKeyUp( keyCode, modifiers );
		}
	}


	inline bool Window::keyIsHeld( int key ) const
	{
		return glfwGetKey( m_window, key ) != 0;
	}
}