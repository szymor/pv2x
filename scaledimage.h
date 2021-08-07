#ifndef SCALEDIMAGE_H
#define SCALEDIMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <math.h>

#include "sdltools.h"

#define ROTATEMODE_NONE 0
#define ROTATEMODE_ROTATE 1
#define ROTATEMODE_BEST 2

class ScaledImage {

private:
	void init();
	
public:
	SDL_Surface *image;
	SDL_Surface *scaledImage;
	int isScaled;
	int loadError;
	
	ScaledImage(std::string filename, int rotateMode=0);
	~ScaledImage();
	
	void blit(SDL_Surface *target);
	void blit(SDL_Surface *target, int x, int y);
	void blit(SDL_Surface *target, SDL_Rect *rect);
	void blit(SDL_Surface *target, SDL_Rect *rect, SDL_Rect *destrect);
	void scale(double factor, double angle=0.0);
};

#endif
