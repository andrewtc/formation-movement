#ifndef ATC_COLOR_INL
#define ATC_COLOR_INL

#include "Math.h"

namespace atc
{
	inline Color::Color( const unsigned char* data )
	{
		// Copy the channel data into the struct.
		memcpy( getData(), data, sizeof( unsigned char ) * CHANNEL_COUNT );
	}


	inline Color::Color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha ) :
		red( red ), blue( blue ), green( green ), alpha( alpha )
	{ }


	inline Color Color::createFromValues( float red, float green, float blue, float alpha )
	{
		return Color
		(
			(unsigned char) ( red * CHANNEL_MAX ),
			(unsigned char) ( green * CHANNEL_MAX ),
			(unsigned char) ( blue * CHANNEL_MAX ),
			(unsigned char) ( alpha * CHANNEL_MAX )
		);
	}


	inline Color Color::lerp( Color first, Color second, float percentage )
	{
		return Color::createFromValues( Math::lerp( first.getValue( CHANNEL_RED ),   second.getValue( CHANNEL_RED ),   percentage ),
										Math::lerp( first.getValue( CHANNEL_GREEN ), second.getValue( CHANNEL_GREEN ), percentage ),
										Math::lerp( first.getValue( CHANNEL_BLUE ),  second.getValue( CHANNEL_BLUE ),  percentage ),
										Math::lerp( first.getValue( CHANNEL_ALPHA ), second.getValue( CHANNEL_ALPHA ), percentage ) );
	}


	inline unsigned char* Color::getData()
	{
		// Return a pointer to the beginning of the struct as an array of unsigned characters.
		return reinterpret_cast< unsigned char* >( this );
	}


	inline const unsigned char* Color::getData() const
	{
		// Return a pointer to the beginning of the struct as an array of unsigned characters.
		return reinterpret_cast< const unsigned char* >( this );
	}


	inline unsigned char& Color::operator[]( int channel )
	{
		// Treat the struct as an array of channels and grab the channel with the specified offset.
		return getData()[ channel ];
	}


	inline const unsigned char& Color::operator[]( int channel ) const
	{
		// Treat the struct as an array of channels and grab the channel with the specified offset.
		return getData()[ channel ];
	}


	inline float Color::getValue( int channel ) const
	{
		return Math::normalize< unsigned char, float >( operator[]( channel ), 0, CHANNEL_MAX );
	}


	inline float Color::getRedValue() const
	{
		return getValue( CHANNEL_RED );
	}


	inline float Color::getGreenValue() const
	{
		return getValue( CHANNEL_GREEN );
	}


	inline float Color::getBlueValue() const
	{
		return getValue( CHANNEL_BLUE );
	}


	inline float Color::getAlphaValue() const
	{
		return getValue( CHANNEL_ALPHA );
	}
}

#endif
