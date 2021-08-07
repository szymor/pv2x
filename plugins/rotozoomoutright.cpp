#include <SDL.h>
#include <pv2xplugin.h>

// this is the name your plugin appears in the user interface
extern "C" char *pv2x_pluginName() {
	return "Zoom out rotating right";
}

// this is where your fade effect starts
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst) {
	int done=0;
	int pos;

	while (!done) {
		pos=getPluginTimerValue(1000);

		if (pos>0) {
			SDL_BlitSurface(dst, NULL, screen, NULL);
			sdlRotoZoom(src, screen,1-(float)pos/1000,6.283185*(float)pos/1000,ROTOZOOM_TRANSP);
			SDL_Flip(screen);
		}
		// has the user pressed any buttons wanting us to quit or are we over time?
		if (pluginMustExit()) done=1;
	}
	SDL_BlitSurface(dst, NULL, screen, NULL);
}

