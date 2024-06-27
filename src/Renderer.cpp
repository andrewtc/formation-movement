#include "common.h"
#include "Renderer.h"

namespace atc
{
	Renderer::Renderer() :
		m_needsViewUpdate( false ),
		m_viewOrigin( Point::ZERO ),
		m_viewSize( Vector::ZERO ),
		m_zoom( 1.0f )
	{ }


	Renderer::~Renderer() { }


	void Renderer::init()
	{
		resetView();
	}


	void Renderer::beginDraw()
	{
		if( m_needsViewUpdate )
		{
			// Draw all X,Y coordinates on the X,Z plane.
			float projectionMatrix[] =
			{
				( 2.0f * m_zoom / m_viewSize.x ), 0.0f, 0.0f, 0.0f,
				0.0f, ( 2.0f * m_zoom / m_viewSize.y ), 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};

			// Load projection matrix.
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			glLoadMatrixf( projectionMatrix );

			// Apply view offset.
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			glTranslatef( -m_viewOrigin.x, -m_viewOrigin.y, 0.0f );

			// Mark that the view is up-to-date.
			m_needsViewUpdate = false;
		}
	}


	void Renderer::endDraw()
	{
	}


	void Renderer::clear( Color color )
	{
		// Clear the background.
		glClear( GL_COLOR_BUFFER_BIT );
		glClearColor( color.getRedValue(), color.getGreenValue(), color.getBlueValue(), color.getAlphaValue() );
	}


	void Renderer::setView( const Point& origin, float zoom )
	{
		// Save the current view settings.
		m_viewOrigin = origin;
		m_zoom = zoom;

		// Invalidate the view.
		m_needsViewUpdate = true;
	}


	void Renderer::setViewSize( const Vector& viewSize )
	{
		// Set the view size and invalidate the view.
		m_viewSize = viewSize;
		m_needsViewUpdate = true;
	}


	void Renderer::pushTransform( const Vector& translation, const Rotation& rotation )
	{
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glTranslatef( translation.x, translation.y, 0.0f );
		glRotatef( rotation.toDegrees(), 0.0f, 0.0f, 1.0f );
	}


	void Renderer::popTransform()
	{
		glPopMatrix();
	}


	void Renderer::drawLine( const Point& start, const Point& end )
	{
		glBegin( GL_LINES );

		glVertex2f( start.x, start.y );
		glVertex2f( end.x, end.y );

		glEnd();
	}


	void Renderer::drawCircle( float radius, int subdivisions )
	{
		glBegin( GL_LINE_LOOP );

		// Make sure subdivisions are at least 3 (triangle).
		subdivisions = std::max( 3, subdivisions );

		Rotation rotationBetweenPoints = Rotation::createFromRadians( Angle::TWO_PI / subdivisions );
		Rotation rotation;

		for( int i = 0; i < subdivisions; ++i )
		{
			Point nextPoint = Point::ZERO + ( radius * Direction( rotation ) );

			glVertex2f( nextPoint.x, nextPoint.y );

			rotation += rotationBetweenPoints;
		}

		glEnd();
	}


	void Renderer::drawRectangle( const Point& start, const Point& end )
	{
		glBegin( GL_LINE_LOOP );

		glVertex2f( start.x, start.y );
		glVertex2f( end.x, start.y );
		glVertex2f( end.x, end.y );
		glVertex2f( start.x, end.y );

		glEnd();
	}


	void Renderer::fillRectangle( const Point& start, const Point& end )
	{
		glBegin( GL_TRIANGLE_FAN );

		glVertex2f( start.x, start.y );
		glVertex2f( end.x, start.y );
		glVertex2f( end.x, end.y );
		glVertex2f( start.x, end.y );

		glEnd();
	}


	void Renderer::fillCircle( float radius, int subdivisions )
	{
		glBegin( GL_TRIANGLE_FAN );

		// Make sure subdivisions are at least 3 (triangle).
		subdivisions = std::max( 3, subdivisions );

		Rotation rotationBetweenPoints = Rotation::createFromRadians( Angle::TWO_PI / subdivisions );
		Rotation rotation;

		glVertex2f( 0.0f, 0.0f );

		for( int i = 0; i <= subdivisions; ++i )
		{
			Point nextPoint = Point::ZERO + ( radius * Direction( rotation ) );

			glVertex2f( nextPoint.x, nextPoint.y );

			rotation += rotationBetweenPoints;
		}

		glEnd();
	}


	void Renderer::renderText( Font* font, const std::string& text, const Point& position, float size )
	{
		Point glyphBottomLeft = position;

		// Get the width and height of a glyph of the requested size.
		Vector glyphSize = Vector( font->getGlyphWidth( size ), font->getGlyphHeight( size ) );

		int glyphCount = (int) text.size();

		for( int i = 0; i < glyphCount; ++i )
		{
			Point glyphTopRight = ( glyphBottomLeft + glyphSize );

			float glyphTextureWidth = font->getGlyphTextureWidth();
			float glyphTextureLeft = font->getGlyphTextureOffset( text[ i ] );
			float glyphTextureRight = ( glyphTextureLeft + glyphTextureWidth );
			float glyphTextureTop = 0.0f;
			float glyphTextureBottom = 1.0f;

			// Bind the font texture.
			font->getTexture()->apply();

			// Draw a glyph for each character in the string.
			glBegin( GL_TRIANGLES );

			glTexCoord2f( glyphTextureLeft, glyphTextureBottom );
			glVertex2f( glyphBottomLeft.x, glyphBottomLeft.y );
			glTexCoord2f( glyphTextureRight, glyphTextureTop );
			glVertex2f( glyphTopRight.x, glyphTopRight.y );
			glTexCoord2f( glyphTextureLeft, glyphTextureTop );
			glVertex2f( glyphBottomLeft.x, glyphTopRight.y );

			glTexCoord2f( glyphTextureLeft, glyphTextureBottom );
			glVertex2f( glyphBottomLeft.x, glyphBottomLeft.y );
			glTexCoord2f( glyphTextureRight, glyphTextureBottom );
			glVertex2f( glyphTopRight.x, glyphBottomLeft.y );
			glTexCoord2f( glyphTextureRight, glyphTextureTop );
			glVertex2f( glyphTopRight.x, glyphTopRight.y );

			glEnd();

			font->getTexture()->clear();

			glyphBottomLeft.x += glyphSize.x;
		}
	}
}