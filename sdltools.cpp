#include "sdltools.h"

SDL_Surface *sdlCreateSurface(int width, int height, int debug) {
	SDL_Surface *screen=SDL_GetVideoSurface();
	SDL_Surface *surface=SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		width,
		height,
		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask
	);
	
	if(surface == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	return surface;
}

void sdlLock(SDL_Surface *surface) {
	if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
}

void sdlUnlock(SDL_Surface *surface) {
	if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
}

void sdlClearEvents() {
	SDL_Event e;
	SDL_PumpEvents();
        SDL_PeepEvents(&e, 64, SDL_GETEVENT, 0xffffffff);
}

void sdlRotoZoom(SDL_Surface *source, SDL_Surface *target, float zoom, float rotation, int transp) {
	Uint16 *src=(Uint16*)source->pixels;
	Uint16 *dst=(Uint16*)target->pixels;
	
	int sinfp=(int)((sin(rotation)*65536.0)/zoom);
        int cosfp=(int)((cos(rotation)*65536.0)/zoom);

        int xc=(source->w<<15) - ((target->w>>1)*(cosfp+sinfp));
        int yc=(source->h<<15) - ((target->h>>1)*(cosfp-sinfp));

        int tx,ty;
        int x,y;
        int tempx,tempy;

        int targetpitch=target->pitch>>1;
	int sourcepitch=source->pitch>>1;

        for ( y=0; y<target->h; y++ ) {

                tx=xc;
                ty=yc;

                for( x=0; x<target->w; x++ ) {

                        tempx=(tx>>16);
                        tempy=(ty>>16);

                        if( (tempx<0) || (tempx>=source->w) || (tempy<0) || (tempy>=source->h) ) {
				if (!transp) *(dst+x+y*targetpitch) = 0;
			} else {
				*(dst+x+y*targetpitch) = *(src+tempx+tempy*sourcepitch);
			}

                        tx+=cosfp;
                        ty-=sinfp;
                }
                xc+=sinfp;
                yc+=cosfp;
        }
}

