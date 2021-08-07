// TTF_Init() must be called on once on application start

#include "sdlttf.h"

SDLTTF::SDLTTF(std::string fontfile, int pointSize) {
	this->error=0;
	// FIXME: why does this end up with invisible fonts on some linux systems?
#ifdef GP2X
	this->renderMode=TEXTMODE_SOLID;
#endif
#ifdef LINUX
	this->renderMode=TEXTMODE_SHADED;
#endif
	this->font=NULL;
	this->setForegroundColor(0xff,0xff,0xff);
	this->setBackgroundColor(0,0,0);

	this->font = TTF_OpenFont(fontfile.c_str(), pointSize);
	if (this->font == NULL){
		this->error=2;
	}
	
	int w,h;
	TTF_SizeText(this->font, "TPgq", &w, &h);
	this->height=h;
}

SDLTTF::~SDLTTF() {
	if (this->font != NULL) {
		TTF_CloseFont(this->font);
	}
}

TTF_Font *SDLTTF::getFont() {
	return this->font;
}

void SDLTTF::setForegroundColor(Uint8 r, Uint8 g, Uint8 b) {
	this->setForegroundColor(r,g,b,0xff);
}

void SDLTTF::setForegroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	this->foreground.r=r;
	this->foreground.g=g;
	this->foreground.b=b;
	this->foreground.unused=a;
}

void SDLTTF::setBackgroundColor(Uint8 r, Uint8 g, Uint8 b) {
	this->setBackgroundColor(r,g,b,0xff);
}

void SDLTTF::setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	this->background.r=r;
	this->background.g=g;
	this->background.b=b;
	this->background.unused=a;
}

void SDLTTF::setRenderMode(int mode) {
	this->renderMode=mode;
}

void SDLTTF::render(SDL_Surface *target, std::string text) {
	this->render(target, text, 0, 0);
}

void SDLTTF::render(SDL_Surface *target, std::string text, int x, int y) {
	SDL_Surface *renderedText;

	switch (this->renderMode) {
		case TEXTMODE_SHADED:
                        renderedText = TTF_RenderText_Shaded(this->font, text.c_str(), this->foreground, this->background);
                       	break;
		case TEXTMODE_BLENDED:
                        renderedText = TTF_RenderText_Blended(this->font, text.c_str(), this->foreground);
                       	break;
		default:
                       	renderedText = TTF_RenderText_Solid(this->font, text.c_str(), this->foreground);
                       	break;
	}

	SDL_Rect targetPos;
	targetPos.x=x;
	targetPos.y=y;
	SDL_BlitSurface(renderedText,NULL,target,&targetPos);
	SDL_FreeSurface(renderedText);
}

int SDLTTF::getTextHeight() {
	return this->height;
}

int SDLTTF::getStringWidth(std::string str) {
        int w,h;
	TTF_SizeText(this->font, str.c_str(), &w, &h);
	return w;
}
