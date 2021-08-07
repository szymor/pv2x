#include <SDL.h>
#include <pv2xplugin.h>

/*
 * this effect looks best at 5 seconds fade time or higher
 */

// this is the name your plugin appears in the user interface
extern "C" char *pv2x_pluginName() {
	return "Flame (needs cpu)";
}

// this is where your fade effect starts
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst) {
	int done=0;
	int pos;

	SDL_Surface *dst32=SDL_CreateRGBSurface(SDL_HWSURFACE,dst->w,dst->h,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);

	SDL_BlitSurface(dst, NULL, dst32, NULL);

	int y,x;
	Uint32 pixel;
	Uint32 *data=(Uint32 *)dst32->pixels;
	Uint32 alpha;
	Uint8 r,g,b,a;
	int asum;
	int rndx;
	int drawsize;
	int fillcount=0;
	int offset;

	// set pixelwise alpha to 0 on destination picture
	sdlLock(dst32);
	for (y=0;y<240;y++) {
		for (x=0;x<320;x++) {
			pixel=data[y*320+x];
			SDL_GetRGBA(pixel,dst32->format,&r,&g,&b,&a);
			pixel=SDL_MapRGBA(dst32->format,r,g,b,0x0);
			data[y*320+x]=pixel;
		}
	}
	sdlUnlock(dst32);

	while (!done) {
		if (pluginMustExit()) {
			done=1;
			continue;
		}

		// lets get the position relative to time already working
		pos=getPluginTimerValue(240);

		SDL_BlitSurface(src,NULL,screen,NULL);

		if (pos>0) {
			sdlLock(dst32);

			for (int i=0;i<3;i++) {
				// calculate hotspot size
				drawsize=pos-(240>>4);
				if (drawsize<240>>4) {
					drawsize=240>>4;
				}
				if (drawsize>80) drawsize=80;
				drawsize=drawsize-rand()%20;

				// draw random alpha hotspots
				rndx=rand()%(320-drawsize);
				alpha=(0xff-(rand()%80))<<24;
				for (y=240-drawsize-1;y<240-1;y++) {
					for (x=rndx;x<drawsize+rndx;x++) {
						data[y*320+x]=data[y*320+x]&0x00ffffff|alpha;
					}
				}

				// interpolate alpha
				for (y=240-2-fillcount;y>0;y--) {
					for (x=320-2;x>=0;x--) {
						asum=(data[(y-1)*320+(x)]&0xff000000)>>24;
						asum+=(data[(y)*320+(x+1)]&0xff000000)>>24;
						asum+=(data[(y+1)*320+(x)]&0xff000000)>>24;
						asum+=(data[(y)*320+(x-1)]&0xff000000)>>24;
						asum=asum>>2;
						data[(y-1)*320+x]=data[(y-1)*320+x]&0x00ffffff|(((asum)&0xff)<<24);
					}
				}
				// fill screen with full alpha from bottom to top
				if (pos>180) {
					if (fillcount>239) fillcount=239;
					offset=0;
					for (y=240-(fillcount)-1;y<240-1;y++) {
						alpha=(175+(offset<<2))<<24;
						for (x=0;x<320;x++) {
							data[y*320+x]=data[y*320+x]&0x00ffffff|alpha;
						}
						if (offset<20) {
							offset++;
						}
					}
					fillcount=(pos-180)<<2;
				}
			}
			sdlUnlock(dst32);
		}

		SDL_BlitSurface(dst32,NULL,screen,NULL);
		SDL_Flip(screen);

	}

	SDL_FreeSurface(dst32);
}

