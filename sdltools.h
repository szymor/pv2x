#ifndef SDLTOOLS_H
#define SDLTOOLS_H

#include <SDL.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define ROTOZOOM_OPAQUE 0
#define ROTOZOOM_TRANSP 1

SDL_Surface *sdlCreateSurface(int width, int height, int debug=0);
void sdlLock(SDL_Surface *surface);
void sdlUnlock(SDL_Surface *surface);
void sdlClearEvents();
void sdlRotoZoom(SDL_Surface *source, SDL_Surface *target, float zoom, float rotation, int transp=ROTOZOOM_OPAQUE);

#endif
