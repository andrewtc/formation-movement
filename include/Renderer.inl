namespace atc
{
	inline void Renderer::setColor( Color color )
	{
		glColor4ubv( (GLubyte*) color.getData() );
	}


	inline Vector Renderer::getViewSize() const
	{
		return m_viewSize;
	}


	inline float Renderer::getViewTop() const
	{
		return ( 0.5f * m_viewSize.y );
	}


	inline float Renderer::getViewLeft() const
	{
		return ( -0.5f * m_viewSize.x );
	}


	inline float Renderer::getViewBottom() const
	{
		return ( -0.5f * m_viewSize.y );
	}


	inline float Renderer::getViewRight() const
	{
		return ( 0.5f * m_viewSize.x );
	}


	inline void Renderer::resetView()
	{
		setView( Point::ZERO, 1.0f );
	}
}