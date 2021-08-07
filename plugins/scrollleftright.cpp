#include <SDL.h>
#include <pv2xplugin.h>

// this is the name your plugin appears in the user interface
extern "C" char *pv2x_pluginName() {
	return "Scroll left to right";
}

// this is where your fade effect starts
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst) {
	int done=0;
	int pos;

	// set up or blit rectangle
	SDL_Rect rect,rect2;
	rect.y=0;
	rect.x=0;
	rect.h=screen->h;
	rect2.y=0;

	while (!done) {
		// lets get the position relative to time already working
		pos=getPluginTimerValue(screen->w);

		// scroll image
		if (pos>0) {
			rect.x=dst->w-pos;
			rect.w=pos;
			SDL_BlitSurface(dst,&rect,screen,NULL);
			rect.x=0;
			rect.w=src->w-pos;
			rect2.x=pos;
			SDL_BlitSurface(src,&rect,screen,&rect2);
			SDL_Flip(screen);
		}
		// has the user pressed any buttons wanting us to quit or are we over time?
		if (pluginMustExit()) done=1;
	}
}

