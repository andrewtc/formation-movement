#ifndef ATC_WINDOW_H
#define ATC_WINDOW_H

namespace atc
{
	/**
	 * Class that encapsulates a GLFW window.
	 */
	class Window
	{
	public:
		static const int DEFAULT_WIDTH = 1600;
		static const int DEFAULT_HEIGHT = 900;
		static const int MIN_ZOOM_LEVEL = -4; // (base two exponent)
		static const int MAX_ZOOM_LEVEL = 4; // (base two exponent)

		Window( const char* title, bool isFullscreen = false );
		~Window();

		static const float SCROLL_REGION_OFFSET;

		void init();
		void destroy();

		void setFullscreen( bool isFullscreen );
		void toggleFullscreen();
		bool isFullscreen() const;

		void setZoomLevel( int zoomLevel );
		void modifyZoomLevel( int change );
		int getZoomLevel() const;

		void swapBuffers();

		void close();
		bool isClosing() const;
		Point getScreenPosition() const;
		Vector getSize() const;

		static void mouseEvent( GLFWwindow* windowHandle, int button, int action, int modifiers );
		void onMouseDown( int button, int modifiers );
		void onMouseUp( int button, int modifiers );
		void onMouseMove();

		static void keyEvent( GLFWwindow* windowHandle, int keyCode, int scanCode, int action, int modifiers );
		void onKeyDown( int keyCode, int modifiers );
		void onKeyUp( int keyCode, int modifiers );
		bool keyIsHeld( int key ) const;

		void updateInput();
		Point getMousePosition() const;
		Vector getScrollDirection() const;
		bool mouseButtonIsUp( int button ) const;
		bool mouseButtonIsDown( int button ) const;
		bool mouseButtonIsDragging( int button ) const;
		Point getMouseButtonDragStartPosition( int button ) const;
		Vector getMouseButtonDragVector( int button ) const;
		Point screenToDeviceCoords( const Point& screenCoords ) const;

		void toggleTrace();

	protected:
		struct MouseButtonState
		{
			MouseButtonState();

			bool isDown;
			bool isDragging;
			Point dragStartPosition;
		};

		bool m_isFullscreen;
		int m_zoomLevel;
		const char* m_title;
		GLFWwindow* m_window;
		Point m_lastMousePosition;
		MouseButtonState m_mouseButtonStates[ GLFW_MOUSE_BUTTON_LAST ];
	};
}

#endif