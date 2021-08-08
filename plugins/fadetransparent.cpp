#include <SDL.h>
#include <pv2xplugin.h>

// this is the name your plugin appears in the user interface
extern "C" char *pv2x_pluginName() {
	return "Fade transparent";
}

// this is where your fade effect starts
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst) {
	int done=0;
	int fade;

	while (!done) {
		// lets get the alpha relative to time already working
		fade=getPluginTimerValue(255);

		if (fade>255) fade=255;

		// fade in new image
		SDL_SetAlpha(dst, SDL_SRCALPHA, (Uint8) fade);
		// fade out old image
		SDL_SetAlpha(src, SDL_SRCALPHA, (Uint8) 255-fade);
		// combine them on screen
		sdlLock(screen);
		SDL_BlitSurface(src, NULL, screen, NULL);
		SDL_BlitSurface(dst, NULL, screen, NULL);
		sdlUnlock(screen);
		SDL_Flip(screen);
		// has the user pressed any buttons wanting us to quit or are we over time?
		if (pluginMustExit()) done=1;
	}
}

