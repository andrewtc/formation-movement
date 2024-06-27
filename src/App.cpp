#include "common.h"
#include "App.h"

namespace atc
{
	const Color App::BACKGROUND_COLOR = Color::createFromValues( 0.0f, 0.1f, 0.2f );


	App::App() :
		m_window( nullptr ),
		m_renderer( nullptr ),
		m_world( nullptr ),
		m_hud( nullptr ),
		m_isRunning( false ),
		m_isPaused( false ),
		m_isQuitting( false )
	{ }


	App::~App() { }


	void App::onStartup()
	{
		m_isRunning = true;

		// Initialize GLFW.
		int success = glfwInit();
		promises( success );

		// Create Renderer.
		m_renderer = new Renderer();
		m_renderer->init();

		// Create window.
		m_window = new Window( "RTS Formation Movement" );
		m_window->init();

		// Set up the World and load a default map.
		m_world = new World();
		m_world->loadMap( "04" );
		m_world->init();

		// Set up the HUD.
		m_hud = new HUD();
	}


	void App::run()
	{
		requires( !m_isRunning );

		// Start up the app.
		onStartup();

		double lastFrameTime = glfwGetTime();
		double accumulatedFrameTime = 0.0f;

		while( !m_window->isClosing() && !m_isQuitting )
		{
			double currentFrameTime = glfwGetTime();

			if( currentFrameTime != lastFrameTime )
			{
				accumulatedFrameTime = std::min( accumulatedFrameTime + ( currentFrameTime - lastFrameTime ), MAX_ACCUMULATED_FRAME_TIME );
				lastFrameTime = currentFrameTime;
			}

			while( accumulatedFrameTime >= TARGET_FRAME_TIME )
			{
				// Do as many updates as necessary to exhaust the accumulated frame time.
				accumulatedFrameTime -= TARGET_FRAME_TIME;

				if( !m_isPaused )
				{
					m_world->update( TARGET_FRAME_TIME );
				}

				m_hud->update( TARGET_FRAME_TIME );
			}

			// Clear the screen.
			m_renderer->clear( BACKGROUND_COLOR );

			// Draw the world.
			m_world->draw( m_renderer );
			m_hud->draw( m_renderer );

			m_window->swapBuffers();
			glfwPollEvents();
			
			// Update input.
			m_window->updateInput();
		}

		// Shut down the app.
		onShutdown();
	}


	void App::quit()
	{
		m_isQuitting = true;
	}


	void App::onShutdown()
	{
		// Delete all subsystems.
		delete m_world;
		delete m_renderer;
		delete m_window;

		// Shut down GLFW.
		glfwTerminate();

		m_isRunning = false;
		m_isQuitting = false;
	}
}