#ifndef PLUGINS_H
#define PLUGINS_H

#include <SDL.h>
#include <fcntl.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include "pv2xplugin.h"

using namespace std;

class Plugins {
private:
	vector<string> pluginFiles;
	vector<string> pluginNames;

public:
	Plugins();
	~Plugins();
	void scan();
	void call(int pnum, SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst, Uint32 time);
	vector<string> getNames();
};

#endif
