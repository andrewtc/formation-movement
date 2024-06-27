#ifndef ATC_TEXTURE_H
#define ATC_TEXTURE_H


namespace atc
{
	/**
	 * Represents a renderable texture.
	 */
	class Texture
	{
	public:
		static Texture* loadFromMemory( int width, int height, const unsigned int* texels, bool hasAlpha = false );
		static Texture* loadFromFile( const std::string& path );

		~Texture();

		void apply();
		void clear();

		int getWidth() const;
		int getHeight() const;
		float getAspectRatio() const;
		bool hasAlpha() const;

	private:
		Texture( int width, int height, bool hasAlpha, unsigned int handle );
		Texture( const Texture& other );
		void operator=( const Texture& other );

		unsigned int m_handle;
		int m_width, m_height;
		float m_aspectRatio;
		bool m_hasAlpha;
	};
}


#endif
