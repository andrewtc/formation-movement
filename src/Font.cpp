#include "common.h"
#include "Font.h"

namespace atc
{
	Font* Font::loadFromFile( const std::string& path, int glyphCount )
	{
		// Load the texture from the supplied path.
		Texture* texture = Texture::loadFromFile( path );
		requires( texture );

		// Load the font from the loaded texture.
		Font* font = new Font( texture, glyphCount );

		return font;
	}


	Font::Font( Texture* texture, int glyphCount ) :
		m_texture( texture ),
		m_glyphCount( glyphCount )
	{
		requires( m_texture );
		requires( m_glyphCount > 0 );

		// Calculate the width and height of a single glyph.
		m_glyphTextureWidth = ( 1.0f / m_glyphCount );
		m_glyphTextureHeight = 1.0f;
		m_glyphAspectRatio = ( m_glyphTextureWidth * texture->getAspectRatio() );
	}
}