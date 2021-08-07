// TTF_Init() must be called on once on application start

#ifndef SDLTTF_H
#define SDLTTF_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#define TEXTMODE_SOLID 1
#define TEXTMODE_SHADED 2
#define TEXTMODE_BLENDED 3

class SDLTTF {
private:
	TTF_Font *font;
	SDL_Color foreground;
	SDL_Color background;
	int renderMode;
	int height;

public:
	SDLTTF(std::string fontfile, int pointSize);
	~SDLTTF();
	
	int error;
	TTF_Font *getFont();
	void setForegroundColor(Uint8 r, Uint8 g, Uint8 b);
	void setForegroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b);
	void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void setRenderMode(int mode);
	void render(SDL_Surface *target, std::string text);
	void render(SDL_Surface *target, std::string text, int x, int y);
	int getTextHeight();
	int getStringWidth(std::string str);
};

#endif
