#ifndef CONFIG_H
#define CONFIG_H

#include <SDL.h>
#include <string>
#include <vector>
#include "plugins.h"

class Config {
public:
	std::string path;
	Uint32 delay;
	int rotateMode;
	int displayOrder;
	int showFilename;
	int plugin;
	vector<string> pluginNames;
	Uint32 pluginDelay;
};

#endif
