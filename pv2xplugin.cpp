#include "pv2xplugin.h"

Uint32 pluginInternalTimer;
Uint32 pluginInternalTime;

void initPluginTimer(Uint32 time) {
	pluginInternalTimer=SDL_GetTicks();
	pluginInternalTime=time;
}

int getPluginTimerValue(int max) {
	Uint32 offset=SDL_GetTicks()-pluginInternalTimer;
	return (int)(((float)offset/(float)pluginInternalTime)*max);
}

int pluginMustExit() {
	SDL_Event event;
	Uint32 offset=SDL_GetTicks()-pluginInternalTimer;

	if (offset>pluginInternalTime) return 1;

	SDL_PumpEvents();
	if (SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_KEYDOWN)|SDL_EVENTMASK(SDL_JOYBUTTONDOWN)|SDL_EVENTMASK(SDL_QUIT))) {
		if (event.type==SDL_QUIT) return 1;
		if (
#ifdef MIYOO
				( event.key.keysym.sym == SDLK_RETURN ) ||
				(event.key.keysym.sym == SDLK_ESCAPE) ||
				(event.key.keysym.sym == SDLK_RCTRL)
#else
				( (event.key.keysym.sym == SDLK_c) && (event.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)) ) ||
				( event.key.keysym.sym == SDLK_RETURN ) ||
				( event.jbutton.button == VK_SELECT ) ||
				( event.jbutton.button == VK_START )
#endif
		) {
			return 1;
		}
		SDL_PollEvent(&event);
	}
	return 0;
}
