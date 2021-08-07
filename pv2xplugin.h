#ifndef PV2XPLUGIN_H
#define PV2XPLUGIN_H

#include <SDL.h>
#include <math.h>
#include <iostream>
#include "defines.h"
#include "sdltools.h"
#include "misctools.h"
#include "gp2x.h"

using namespace std;

extern "C" char *pv2x_pluginName();
extern "C" void pv2x_fade(SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst);

/* user has pressed any key or are we over time? */
int pluginMustExit();

/* return a value relative to max showing how far the effect should be
 *
 * examples:
 *
 * int percent=getPluginTimerValue(100); // would return 50 on half the time
 * int imgwidth=getPluginTimerValue(320); // would return 160 on half the time
 * */
int getPluginTimerValue(int max);

/* used by the plugin system for internal timing purpose */
void initPluginTimer(Uint32 time);

#endif
