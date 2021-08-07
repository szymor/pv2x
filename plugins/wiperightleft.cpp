#include <SDL.h>
#include <pv2xplugin.h>

// this is the name your plugin appears in the user interface
extern "C" char *pv2x_pluginName() {
	return "Wipe right to left";
}

// this is where your fade effect starts
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst) {
	int done=0;
	int pos;

	// set up or blit rectangle
	// why use two rects holding the same values?
	// dont ask me, ask sdl, its leafing ugly glitches if not
	SDL_Rect rect,rect2;
	rect.y=0;
	rect.h=screen->h;
	rect.w=screen->w;
	rect2.y=0;
	rect2.h=screen->h;
	rect2.w=screen->w;

	while (!done) {
		// lets get the position relative to time already working
		pos=getPluginTimerValue(screen->w);

		if (pos>0) {
			// blit the new pictrue from x=320 to x=position to screen
			rect.x=screen->w-pos;
			rect2.x=screen->w-pos;
			SDL_BlitSurface(dst,&rect,screen,&rect2);
			SDL_Flip(screen);
		}
		// has the user pressed any buttons wanting us to quit or are we over time?
		if (pluginMustExit()) done=1;
	}
}
