#ifndef ATC_CAMERA_H
#define ATC_CAMERA_H

namespace atc
{
	/**
	 * Actor in the world from which the scene is viewed.
	 */
	class Camera : public Actor
	{
	public:
		static const float SCROLL_SPEED;

		Camera();
		virtual ~Camera();

		virtual void update( double elapsedTime );

		void apply();

		Vector getWorldSize() const;
		Point getWorldTopLeft() const;
		Point getWorldBottomRight() const;
		Point deviceToWorldCoords( const Point& screenCoords ) const;
		Point worldToScreenCoords( const Point& worldCoords ) const;

		void setZoom( float zoom );
		float getZoom() const;

		bool isCurrent() const;

	protected:
		float m_zoom;
	};
}

#endif