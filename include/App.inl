namespace atc
{
	inline Window* App::getWindow() const
	{
		return m_window;
	}


	inline Renderer* App::getRenderer() const
	{
		return m_renderer;
	}


	inline World* App::getWorld() const
	{
		return m_world;
	}


	inline HUD* App::getHUD() const
	{
		return m_hud;
	}


	inline void App::setPaused( bool isPaused )
	{
		m_isPaused = isPaused;
	}


	inline void App::togglePaused()
	{
		m_isPaused = !m_isPaused;
	}


	inline bool App::isPaused() const
	{
		return m_isPaused;
	}
}