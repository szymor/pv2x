#ifndef PROJECTFUNCTIONS_H
#define PROJECTFUNCTIONS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <fstream>

#include "defines.h"
#include "gp2x.h"
#include "misctools.h"
#include "sdltools.h"
#include "sdlttf.h"
#include "scaledimage.h"
#include "config.h"

using namespace std;

int noFilesFoundDialog();
Config configDialog(Config actualConfig);
string pathDialog(string path="");
void saveConfig(Config conf);
void readConfig(Config *conf);

#endif
