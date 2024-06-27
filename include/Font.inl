namespace atc
{
	inline Texture* Font::getTexture() const
	{
		return m_texture;
	}


	inline int Font::getGlyphCount() const
	{
		return m_glyphCount;
	}


	inline float Font::getGlyphWidth( float size ) const
	{
		return ( size * m_glyphAspectRatio );
	}


	inline float Font::getGlyphHeight( float size ) const
	{
		return size;
	}


	inline float Font::getGlyphTextureWidth() const
	{
		return m_glyphTextureWidth;
	}


	inline float Font::getGlyphTextureHeight() const
	{
		return m_glyphTextureHeight;
	}


	inline float Font::getGlyphTextureOffset( char glyph ) const
	{
		return ( glyph - 32 ) * m_glyphTextureWidth;
	}
}