#include <SDL.h>
#include <pv2xplugin.h>

// this is the name your plugin appears in the user interface
extern "C" char *pv2x_pluginName() {
	return "Wipe top to bottom";
}

// this is where your fade effect starts
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst) {
	int done=0;
	int pos;

	// set up or blit rectangle
	SDL_Rect rect;
	rect.y=0;
	rect.x=0;
	rect.w=screen->w;

	while (!done) {
		// lets get the position relative to time already working
		pos=getPluginTimerValue(screen->h);

		if (pos>0) {
			// blit the new pictrue from y=0 to y=position to screen
			rect.h=pos;
			SDL_BlitSurface(dst,&rect,screen,NULL);
			SDL_Flip(screen);
		}
		// has the user pressed any buttons wanting us to quit or are we over time?
		if (pluginMustExit()) done=1;
	}
}

