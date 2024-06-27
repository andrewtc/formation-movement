#ifndef ATC_PATH_H
#define ATC_PATH_H

namespace atc
{
	/**
	 * Represents a stack of waypoints through which a Unit may
	 * pass to reach some destination.
	 */
	class Path
	{
	public:
		Path( Unit* unit = nullptr );
		Path( Unit* unit, const Point& destination );
		~Path();

		void draw( Renderer* renderer );

		void clear();
		void pushWaypoint( const Point& waypoint );
		Point popNextWaypoint();
		Point getWaypoint( int index ) const;
		Point getNextWaypoint() const;
		Point getDestination() const;
		bool isValid() const;

		Unit* getUnit() const;

	protected:
		Unit* m_unit;
		std::vector< Point > m_waypoints;
	};
}

#endif