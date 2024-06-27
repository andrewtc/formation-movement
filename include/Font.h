#ifndef ATC_FONT_H
#define ATC_FONT_H

namespace atc
{
	/**
	 * Simple text renderer class.
	 */
	class Font
	{
	public:
		static Font* loadFromFile( const std::string& path, int glyphCount );

		Font( Texture* texture, int glyphCount );
		~Font();

		void drawText( const std::string& text, const Point& position, float fontSize );

		Texture* getTexture() const;
		int getGlyphCount() const;
		float getGlyphWidth( float size ) const;
		float getGlyphHeight( float size ) const;
		float getGlyphTextureWidth() const;
		float getGlyphTextureHeight() const;
		float getGlyphTextureOffset( char glyph ) const;

	protected:
		Texture* m_texture;
		int m_glyphCount;
		float m_glyphTextureWidth;
		float m_glyphTextureHeight;
		float m_glyphAspectRatio;
	};
}

#endif