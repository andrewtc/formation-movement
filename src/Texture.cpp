#include "common.h"
#include "Texture.h"


namespace atc
{
	Texture::Texture( int width, int height, bool hasAlpha, unsigned int handle ) :
		m_width( width ), m_height( height ),
		m_hasAlpha( hasAlpha ), m_handle( handle )
	{
		requires( handle >= 0 );
		requires( width > 0 );
		requires( height > 0 );

		// Calculate aspect ratio.
		m_aspectRatio = ( (float) m_width / m_height );
	}


	Texture::~Texture()
	{
		if( m_handle >= 0u )
		{
			// Delete the OpenGL texture from the graphics card.
			glDeleteTextures( 1, &m_handle );
		}
	}


	void Texture::apply()
	{
		if( m_hasAlpha )
		{
			// Enable alpha blending.
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glEnable( GL_BLEND );
		}

		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_handle );
	}


	void Texture::clear()
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
		glDisable( GL_TEXTURE_2D );

		if( m_hasAlpha )
		{
			glDisable( GL_BLEND );
		}
	}


	Texture* Texture::loadFromMemory( int width, int height, const unsigned int* texels, bool hasAlpha )
	{
		// Make sure the texels array points to a valid array.
		requires( texels );
		requires( sizeof( *texels ) > 0 );

		// Generate an OpenGL texture.
		unsigned int dataHandle;
		glGenTextures( 1, &dataHandle );

		// Configure the texture properties.
		glBindTexture( GL_TEXTURE_2D, dataHandle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		// Load the texture from the supplied memory.
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, (GLuint) width, (GLuint) height,	// Format on graphics card.
					  0, GL_RGBA, GL_UNSIGNED_BYTE, texels );						// Format on CPU

		// Return a new Texture object that is bound to the GL texture.
		return new Texture( width, height, hasAlpha, dataHandle );
	}


	Texture* Texture::loadFromFile( const std::string& path )
	{
		// Make sure the supplied path is valid.
		requires( !path.empty() );

		// Load the file from the disk into a temporary array.
		unsigned int width, height, numChannels;
		unsigned int* texels = ( unsigned int* ) stbi_load( path.c_str(), (int*) &width, (int*) &height, (int*) &numChannels, 4 );

		bool hasAlpha = ( numChannels > 3 );

		// Create a new OpenGL texture from the file data.
		Texture* texture = loadFromMemory( width, height, texels, hasAlpha );

		// Destroy the temporary array.
		stbi_image_free( texels );

		return texture;
	}
}