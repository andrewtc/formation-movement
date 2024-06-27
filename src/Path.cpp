#include "common.h"
#include "Path.h"

namespace atc
{
	Path::Path( Unit* unit ) :
		m_unit( unit )
	{ }


	Path::Path( Unit* unit, const Point& destination ) :
		m_unit( unit )
	{
		// Push the destination point onto the waypoint stack.
		pushWaypoint( destination );
	}


	Path::~Path() { }


	void Path::draw( Renderer* renderer )
	{
		if( isValid() )
		{
			requires( m_unit );

			// Otherwise, use the default color.
			renderer->setColor( Unit::DEFAULT_COLOR );

			// Start drawing from the first waypoint.
			Point lastPoint = getWaypoint( 0 );

			for( int i = 1; i < (int) m_waypoints.size(); ++i )
			{
				// Draw lines connecting the waypoints in the Path.
				Point nextPoint = getWaypoint( i );
				renderer->drawLine( lastPoint, nextPoint );
				lastPoint = nextPoint;
			}
		}
	}
}