#ifndef ATC_APP_H
#define ATC_APP_H

namespace atc
{
	/**
	 * Global application class.
	 */
	class App
	{
	public:
		static const Color BACKGROUND_COLOR;

		App();
		~App();

		void run();
		void quit();

		Window* getWindow() const;
		Renderer* getRenderer() const;
		World* getWorld() const;
		HUD* getHUD() const;

		void setPaused( bool isPaused );
		void togglePaused();
		bool isPaused() const;

	protected:
		void onStartup();
		void onShutdown();

		bool m_isRunning;
		bool m_isPaused;
		bool m_isQuitting;
		Window* m_window;
		Renderer* m_renderer;
		World* m_world;
		HUD* m_hud;

		friend class Window;
	};
}

#endif