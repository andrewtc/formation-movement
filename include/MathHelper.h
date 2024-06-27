#ifndef ATC_MATH_H
#define ATC_MATH_H

namespace atc
{
	/**
	 * Static class that defines useful math functions.
	 */
	class Math
	{
	public:
		template< typename T >
		static T clamp( T value, T min, T max )
		{
			requires( max >= min );

			if( value > max )
			{
				value = max;
			}
			else if( value < min )
			{
				value = min;
			}

			return value;
		}


		template< typename T >
		static T wrap( T value, T min, T max )
		{
			requires( max >= min );
			T range = ( max - min );

			while( value >= max )
			{
				value -= range;
			}

			while( value < min )
			{
				value += range;
			}

			return value;
		}


		template< typename T, typename percentage_t >
		static T lerp( T first, T second, percentage_t percentage )
		{
			return ( first + ( ( second - first ) * percentage ) );
		}


		template< typename T, typename percentage_t >
		static percentage_t normalize( T value, T first, T second )
		{
			return ( (percentage_t) ( value - first ) / (percentage_t) ( second - first ) );
		}
	};
}

#endif