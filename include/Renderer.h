#ifndef ATC_RENDERER_H
#define ATC_RENDERER_H

namespace atc
{
	/**
	 * Manages drawing context.
	 */
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void init();

		void beginDraw();
		void endDraw();
		void clear( Color color );
		void pushTransform( const Vector& translation, const Rotation& rotation = Rotation::NO_REVOLUTION );
		void popTransform();

		void setColor( Color color );

		void drawLine( const Point& start, const Point& end );
		void drawCircle( float radius, int subdivisions );
		void drawRectangle( const Point& start, const Point& end );
		void fillCircle( float radius, int subdivisions );
		void fillRectangle( const Point& start, const Point& end );
		void renderText( Font* font, const std::string& text, const Point& position, float size );

		void setView( const Point& origin, float zoom );
		void setViewSize( const Vector& viewSize );
		Vector getViewSize() const;
		float getViewTop() const;
		float getViewLeft() const;
		float getViewBottom() const;
		float getViewRight() const;
		void resetView();

	protected:
		bool m_needsViewUpdate;
		Point m_viewOrigin;
		Vector m_viewSize;
		float m_zoom;
	};
}

#endif