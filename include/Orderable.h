#ifndef ATC_ORDERABLE_H
#define ATC_ORDERABLE_H

namespace atc
{
	/**
	 * Interface for issuing orders.
	 */
	class Orderable
	{
	public:
		virtual void orderMoveTo( const Point& location ) = 0;
	};
}

#endif