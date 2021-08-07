#ifndef FILELIST_H
#define FILELIST_H

#include <iostream>
#include <cctype>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <dirent.h>
#include <SDL.h>

#include "defines.h"
#include "sdlttf.h"
#include "sdltools.h"

using namespace std;

class FileList {
private:
	list<string> *suffixFilter;
	set<string> files;

	void init();

	SDL_Surface *screen;
	Uint32 lastDraw;
	int gray;
	int drawDirection;
	SDLTTF *ttf;

public:
	FileList(void);
	FileList(list<string> *suffixFilter);
	FileList(list<string> *suffixFilter, string path);
	~FileList();

	void resetList();
	void scanDir(string path);
	set<string> getFiles();
	int getNumberOfFiles();
};

#endif
