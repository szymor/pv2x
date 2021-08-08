#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <list>

#include "defines.h"
#include "gp2x.h"
#include "scaledimage.h"
#include "filelist.h"
#include "filelistrandomizer.h"
#include "sdlttf.h"
#include "projectfunctions.h"
#include "plugins.h"

using namespace std;

SDL_Surface *screen = NULL;
SDL_Joystick *joy = NULL;

void Terminate(void) {
	SDL_Quit();
#ifdef GP2X
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif
}

int main (int argc, char **argv) {
	int done;

	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
		exit (1);
	}
	atexit (Terminate);

	SDL_ShowCursor(SDL_DISABLE);

	screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf (stderr, "Couldn't set 320x240x16 video mode: %s\n", SDL_GetError ());
		exit (2);
	}

	if (SDL_NumJoysticks() > 0) {
		joy = SDL_JoystickOpen(0);
		if(!joy) {
			fprintf (stderr, "Couldn't open joystick 0: %s\n", SDL_GetError ());
		}
	}

	if (TTF_Init() == -1) {
		exit(3);
	}

#ifdef LINUX
	string fileDir="/usr/share/pixmaps";
#endif

#ifdef GP2X
	string fileDir="/mnt/sd";
#endif

#ifdef MIYOO
	string fileDir="/mnt";
#endif
	SDLTTF *ttf=new SDLTTF(FONT,12);
	ttf->setForegroundColor(255,255,0);

	Plugins plugins;
	plugins.scan();
	Config config;
	config.path=fileDir;
	config.delay=5000;
	config.rotateMode=0;
	config.displayOrder=0;
	config.showFilename=0;
	config.plugin=0;
	config.pluginNames=plugins.getNames();
	config.pluginDelay=3000;

	readConfig(&config);

	list<string> *filter=new list<string>;
	filter->push_back(".jpg");
	filter->push_back(".pjpg");
	filter->push_back(".jpeg");
	filter->push_back(".pjpeg");
	filter->push_back(".gif");
	filter->push_back(".png");
	filter->push_back(".bmp");
	filter->push_back(".xpm");
	filter->push_back(".tga");
	filter->push_back(".pcx");
	filter->push_back(".ppm");

	FileList *fl=new FileList(filter);
	string path=fileDir;
	do {
		config=configDialog(config);
		fl->resetList();
		fl->scanDir(config.path);
		if (fl->getNumberOfFiles()==0) {
			if (noFilesFoundDialog()==0) {
				exit(0);
			}
		}
	} while (fl->getNumberOfFiles()==0);
	FileListRandomizer *flrand=new FileListRandomizer(fl);

	string filename;
	string basen;
	string lastpath=config.path;

	filename=flrand->getFileName(config.displayOrder);
	basen=basename(filename.c_str());
	ScaledImage *newimg;
	ScaledImage *img=new ScaledImage(filename, config.rotateMode);
	while (img->loadError) {
		filename=flrand->getFileName(config.displayOrder);
		basen=basename(filename.c_str());
		delete img;
		img=new ScaledImage(filename, config.rotateMode);
	}

	img->scale((double) SCREEN_HEIGHT/img->image->h);

	sdlLock(screen);
	SDL_FillRect(screen, NULL, 0);
	sdlUnlock(screen);
	SDL_Flip(screen);

	done = 0;
	int proceed=0;
	int nextimage=0;
	int drawoffset;
	int nextoffset=1;
	Uint32 lastTicks=0xffffffff;
	SDL_Event event;
	SDL_Surface *tmpnew;
	SDL_Surface *tmpold;

	if (img->scaledImage->w<SCREEN_WIDTH) {
		drawoffset=(SCREEN_WIDTH-img->scaledImage->w)>>1;
	} else {
		drawoffset=0;
	}

	while (!done) {
		while (SDL_PollEvent (&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
				case SDL_JOYBUTTONDOWN:
					if (
#ifdef MIYOO
							(event.key.keysym.sym == SDLK_ESCAPE) ||
							(event.key.keysym.sym == SDLK_RCTRL)
#else
							( (event.key.keysym.sym == SDLK_c) && (event.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)) ) ||
							( event.jbutton.button == VK_SELECT )
#endif
					   ) {
						done = 1;
					}
					if (
#ifdef MIYOO
							(event.key.keysym.sym==SDLK_RETURN)
#else
							(event.key.keysym.sym==SDLK_RETURN) ||
							(event.jbutton.button == VK_START)
#endif
					   ) {
						do {
							config=configDialog(config);
							if (config.path!=lastpath) {
								fl->resetList();
								fl->scanDir(config.path);
								if (fl->getNumberOfFiles()==0) {
									if (noFilesFoundDialog()==0) {
										exit(0);
									}
								}
							}
						} while (fl->getNumberOfFiles()==0);
						if (config.path!=lastpath) {
							nextimage=1;
							lastpath=config.path;
						}
						sdlLock(screen);
						SDL_FillRect(screen, NULL, 0);
						sdlUnlock(screen);
						SDL_Flip(screen);
						flrand->setFileList(fl);
						sdlClearEvents();
						lastTicks=SDL_GetTicks();
					}
					if (
						(event.key.keysym.sym==SDLK_LEFT) ||
						(event.jbutton.button == VK_FL)
					) {
						nextimage=1;
						nextoffset=-1;
					}
					if (
						(event.key.keysym.sym==SDLK_RIGHT) ||
						(event.jbutton.button == VK_FR)
					) {
						nextimage=1;
					}
					break;
				case SDL_KEYUP:
					break;
				case SDL_JOYBUTTONUP:
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}

		sdlLock(screen);
		SDL_FillRect(screen, NULL, 0);
		if (img->scaledImage->w<SCREEN_WIDTH) {
			img->blit(screen,(SCREEN_WIDTH-img->scaledImage->w)>>1,0);
		} else {
			img->blit(screen);
		}
		if (config.showFilename) ttf->render(screen, basen, 0, 240-ttf->getTextHeight());

		sdlUnlock(screen);
		SDL_Flip(screen);
		if (lastTicks==0xffffffff) lastTicks=SDL_GetTicks();


		if (nextimage) {
			filename=flrand->getFileName(config.displayOrder,nextoffset);
			basen=basename(filename.c_str());
			newimg=new ScaledImage(filename, config.rotateMode);
			while (newimg->loadError) {
				filename=flrand->getFileName(config.displayOrder,nextoffset);
				basen=basename(filename.c_str());
				delete newimg;
				newimg=new ScaledImage(filename, config.rotateMode);
			}
			newimg->scale((double) SCREEN_HEIGHT/newimg->image->h);

			tmpold=sdlCreateSurface(SCREEN_WIDTH,SCREEN_HEIGHT);
			SDL_FillRect(tmpold,NULL,0);
			img->blit(tmpold,drawoffset,0);

			if (newimg->scaledImage->w<SCREEN_WIDTH) {
				drawoffset=(SCREEN_WIDTH-newimg->scaledImage->w)>>1;
			} else {
				drawoffset=0;
			}

			tmpnew=sdlCreateSurface(SCREEN_WIDTH,SCREEN_HEIGHT);
			sdlLock(tmpnew);
			SDL_FillRect(tmpnew,NULL,0);
			newimg->blit(tmpnew,drawoffset,0);
			sdlUnlock(tmpnew);
			plugins.call(config.plugin,screen,tmpold,tmpnew,config.pluginDelay);
			SDL_FreeSurface(tmpnew);
			SDL_FreeSurface(tmpold);

			delete img;
			img=newimg;
			nextimage=0;
			nextoffset=1;
			lastTicks=SDL_GetTicks();
		}

		proceed=0;

		if (
				(config.delay>0) &&
				((SDL_GetTicks()-lastTicks)>config.delay)
		) {
			nextimage++;
		}
	}

	return 0;
}
