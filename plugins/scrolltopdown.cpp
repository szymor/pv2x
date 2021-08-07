#include <SDL.h>
#include <pv2xplugin.h>

// this is the name your plugin appears in the user interface
extern "C" char *pv2x_pluginName() {
	return "Scroll top to bottom";
}

// this is where your fade effect starts
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst) {
	int done=0;
	int pos;

	// set up or blit rectangle
	SDL_Rect rect,rect2;
	rect.y=0;
	rect.x=0;
	rect.w=screen->w;
	rect.h=screen->h;
	rect2.x=0;

	while (!done) {
		// lets get the position relative to time already working
		pos=getPluginTimerValue(screen->h);

		// scroll image
		if (pos>0) {
			rect.y=dst->h-pos;
			SDL_BlitSurface(dst,&rect,screen,NULL);
			rect.y=0;
			rect2.y=pos;
			SDL_BlitSurface(src,&rect,screen,&rect2);
			SDL_Flip(screen);
		}
		// has the user pressed any buttons wanting us to quit or are we over time?
		if (pluginMustExit()) done=1;
	}
}

