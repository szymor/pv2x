#include <SDL.h>
#include <pv2xplugin.h>

// this is the name your plugin appears in the user interface
extern "C" char *pv2x_pluginName() {
	return "Sinus scroll left to right";
}

// this is where your fade effect starts
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst) {
	int done=0;
	int pos;
	int y;
	int offset=0;
	int lastpos=0;

	// set up or blit rectangle
	SDL_Rect rect,rect2;
	rect.y=0;
	rect.x=0;
	rect.h=2;
	rect2.y=0;

	// setup sinus table
	int sinvals[240*2];
	for (int i=0;i<240;i++) {
		sinvals[i]=-320;
		sinvals[i+240]=(int)(((double)sin((3.1416/2/240)*(double)i)+1)*320)-320;
	}

	while (!done) {
		// lets get the position relative to time already working
		pos=getPluginTimerValue(320);

		// scroll image
		if (pos>0) {
			if (offset<480) {
				for (y=0;y<240;y++) {
					if (y+offset<480) {
						rect.x=0;
						rect.y=y;
						rect.w=320-sinvals[y+offset]+1;
						rect2.x=sinvals[y+offset];
						rect2.y=y;
						SDL_BlitSurface(src,&rect,screen,&rect2);
						rect.x=(320-sinvals[y+offset])-1;
						rect.y=y;
						rect.w=320;
						rect2.y=y;
						rect2.x=0;
						SDL_BlitSurface(dst,&rect,screen,&rect2);
					}
				}
				offset+=(pos-lastpos)<<1;
			}
			SDL_Flip(screen);
		}
		// has the user pressed any buttons wanting us to quit or are we over time?
		if (pluginMustExit()) done=1;
		lastpos=pos;
	}
}

