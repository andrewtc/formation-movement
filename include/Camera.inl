namespace atc
{
	inline Point Camera::deviceToWorldCoords( const Point& deviceCoords ) const
	{
		// Determine where the point is relative to the center of the window (in pixels).
		return Point( ( deviceCoords.x / m_zoom ) + m_position.x,
					  ( deviceCoords.y / m_zoom ) + m_position.y );
	}


	inline void Camera::setZoom( float zoom )
	{
		m_zoom = zoom;
	}


	inline float Camera::getZoom() const
	{
		return m_zoom;
	}


	inline bool Camera::isCurrent() const
	{
		return ( this == getWorld()->getCamera() );
	}
}