#ifndef ATC_HUD_H
#define ATC_HUD_H

namespace atc
{
	/**
	 * Heads-up display that draws important information about the simulation.
	 */
	class HUD
	{
	public:
		HUD();
		~HUD();

		void update( double elapsedTime );
		void draw( Renderer* renderer );

	protected:
		Font* m_font;
	};
}

#endif