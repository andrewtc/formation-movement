#ifndef ATC_COLOR_H
#define ATC_COLOR_H

namespace atc
{
	/**
	 * Simple structure for storing and modifying RGBA colors.
	 */
	struct Color
	{
	public:
		enum Channel
		{
			CHANNEL_RED = 0,
			CHANNEL_GREEN = 1,
			CHANNEL_BLUE = 2,
			CHANNEL_ALPHA = 3
		};

		static const int CHANNEL_COUNT = 4;
		static const unsigned char CHANNEL_MAX = 255;

		static const Color BLACK;
		static const Color WHITE;
		static const Color TRANSPARENT_BLACK;
		static const Color TRANSPARENT_WHITE;

		explicit Color( const unsigned char* data );
		explicit Color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = CHANNEL_MAX );

		static Color createFromValues( float red, float green, float blue, float alpha = 1.0f );
		static Color lerp( Color first, Color second, float percentage );

		unsigned char* getData();
		const unsigned char* getData() const;
		unsigned char& operator[]( int channel );
		const unsigned char& operator[]( int channel ) const;

		float getValue( int channel ) const;
		float getRedValue() const;
		float getGreenValue() const;
		float getBlueValue() const;
		float getAlphaValue() const;

		unsigned char red, green, blue, alpha;
	};
}

#endif
