#include "projectfunctions.h"

int noFilesFoundDialog() {
	SDLTTF ttf=SDLTTF(FONT,12);
	SDL_Surface *screen=SDL_GetVideoSurface();

	ttf.setForegroundColor(255,255,0);
	SDL_FillRect(screen, NULL, 0);
	ttf.render(screen, "no files found!",110,80),
	ttf.setForegroundColor(255,255,255);
	ttf.render(screen, "START - return to pv2x configuration",45,120),
	ttf.render(screen, "SELECT - quit",45,140),
	SDL_Flip(screen);

	sdlClearEvents();
	SDL_Event event;
	while (1) {
		while (SDL_PollEvent (&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
				case SDL_JOYBUTTONDOWN:
					if (
							( (event.key.keysym.sym == SDLK_c) && (event.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)) ) ||
							( event.jbutton.button == VK_SELECT )
					   ) {
						return 0;
					}
					if (
							(event.key.keysym.sym==SDLK_RETURN) ||
							(event.jbutton.button == VK_START)
					   ) {
						return 1;
					}
					break;
			}
			SDL_Delay(100);
		}
	}
	return 0;
}

Config configDialog(Config actualConfig) {
	SDL_Surface *screen=SDL_GetVideoSurface();

	string path=actualConfig.path;

	SDLTTF *ttf=new SDLTTF(FONT,12);
	ttf->setForegroundColor(255,255,0);
	int textheight=ttf->getTextHeight();
	
	int ypos=0;
	int done=0;
	int delay=(int) actualConfig.delay/1000;
	int pluginDelay=(int) actualConfig.pluginDelay/1000;
	int plugin=actualConfig.plugin;
	int showFilename=actualConfig.showFilename;
	int displayOrder=actualConfig.displayOrder;
	int rotateMode=actualConfig.rotateMode;
	string delaytext,rotateText,pluginText,pluginDelayText;
	string orientations[3]={
		"Horizontal",
		"Vertical",
		"Fit best"
	};
	string booleans[2]={
		"No",
		"Yes"
	};
	string displayOrders[2]={
		"Random",
		"Alphabetical"
	};
	
	int numberOfMenuEntries=7;
	
	while (!done) {
                SDL_Event event;

			sdlLock(screen);
                        SDL_FillRect(screen, NULL, 0);

			ttf->setForegroundColor(0,255,255);
			ttf->render(screen, "pv2x configuration",0,0);

			if (ypos==0) {
                                ttf->setForegroundColor(255,255,0);
			} else {
                                ttf->setForegroundColor(255,255,255);
			}
		        ttf->render(screen,"SELECT PATH",10,2*textheight);
		        if (ypos==1) {
                                ttf->setForegroundColor(255,255,0);
			} else {
                                ttf->setForegroundColor(255,255,255);
			}
			delaytext="DELAY: " + intToString(delay) + "s";

			ttf->render(screen,delaytext,10,4*textheight);
			if (ypos==2) {
                                ttf->setForegroundColor(255,255,0);
			} else {
                                ttf->setForegroundColor(255,255,255);
			}
			rotateText="ORIENTATION: "+orientations[rotateMode];
			ttf->render(screen,rotateText,10,6*textheight);

			if (ypos==3) {
                                ttf->setForegroundColor(255,255,0);
			} else {
                                ttf->setForegroundColor(255,255,255);
			}
			rotateText="DISPLAY ORDER: "+displayOrders[displayOrder];
			ttf->render(screen,rotateText,10,8*textheight);

			if (ypos==4) {
                                ttf->setForegroundColor(255,255,0);
			} else {
                                ttf->setForegroundColor(255,255,255);
			}
			rotateText="SHOW FILENAME: "+booleans[showFilename];
			ttf->render(screen,rotateText,10,10*textheight);

			if (ypos==5) {
                                ttf->setForegroundColor(255,255,0);
			} else {
                                ttf->setForegroundColor(255,255,255);
			}
			if ((unsigned int)plugin>actualConfig.pluginNames.size()) {
				plugin=0;
			}
			pluginText="FADE PLUGINS: "+actualConfig.pluginNames[plugin];
			ttf->render(screen,pluginText,10,12*textheight);

			if (ypos==6) {
                                ttf->setForegroundColor(255,255,0);
			} else {
                                ttf->setForegroundColor(255,255,255);
			}
			pluginDelayText="FADE TIME: "+intToString(pluginDelay)+"s";
			ttf->render(screen,pluginDelayText,10,14*textheight);

			sdlUnlock(screen);
			SDL_Flip(screen);
			
		while (SDL_PollEvent (&event)) {

	
			switch (event.type) {
				case SDL_KEYDOWN:
				case SDL_JOYBUTTONDOWN:
					if (
						(event.key.keysym.sym == SDLK_RETURN) ||
						( event.jbutton.button == VK_START )
					) {
						done = 1;
					}
					if (
						(event.key.keysym.sym==SDLK_DOWN) ||
						(event.jbutton.button==VK_DOWN)
					) {
						if (ypos<numberOfMenuEntries-1) ypos++;
					}
					if (
						(event.key.keysym.sym==SDLK_UP) ||
                                                (event.jbutton.button==VK_UP)
					) {
						if (ypos>0) ypos--;
					}
					if (
                                                (event.jbutton.button==VK_TAT) ||
                                                (event.jbutton.button==VK_FB) ||
                                                (event.jbutton.button==VK_FX)
					) {
						if (ypos==0) {
							path=pathDialog(path);
						}
					}
					if (
						(event.key.keysym.sym==SDLK_LEFT) ||
                                                (event.jbutton.button==VK_LEFT)
					) {
						switch (ypos) {
							case 0:
								path=pathDialog(path);
								break;
							case 1:
								if (delay>0) delay--;
								break;
							case 2:
								if (rotateMode>0) rotateMode--;
								break;
							case 3:
								displayOrder=0;
								break;
							case 4:
								showFilename=0;
								break;
							case 5:
								if (plugin>0) plugin--;
								break;
							case 6:
								if (pluginDelay>1) pluginDelay--;
								break;
						}
					}
					if (
						(event.key.keysym.sym==SDLK_RIGHT) ||
                                                (event.jbutton.button==VK_RIGHT)
					) {
						switch (ypos) {
							case 0:
								path=pathDialog(path);
								break;
							case 1:
								delay++;
								break;
							case 2:
								if (rotateMode<2) rotateMode++;
								break;
							case 3:
								displayOrder=1;
								break;
							case 4:
								showFilename=1;
								break;
							case 5:
								if (plugin<(int)actualConfig.pluginNames.size()-1) {
									plugin++;
								} else {
									plugin=actualConfig.pluginNames.size()-1;
								}
								break;
							case 6:
								pluginDelay++;
								break;
						}
					}
					if (
                                                (event.jbutton.button==VK_SELECT)
					) {
						exit(0);
					}
					break;
				case SDL_KEYUP:
					break;
				case SDL_JOYBUTTONUP:
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}
		
		if (!done) SDL_Delay(50);
	}
	
	sdlLock(screen);
	SDL_FillRect(screen, NULL, 0);
	sdlUnlock(screen);
	SDL_Flip(screen);
	Config c;
	c.path=path;
	c.delay=delay*1000;
	c.rotateMode=rotateMode;
	c.displayOrder=displayOrder;
	c.showFilename=showFilename;
	c.plugin=plugin;
	c.pluginDelay=pluginDelay*1000;
	c.pluginNames=actualConfig.pluginNames;

	saveConfig(c);
	
	return c;
}

set<string> readPath(string path) {
	set<string> dirListing;
	if (path.compare("/")!=0) dirListing.insert(".. up");

	DIR *dir=opendir(path.c_str());
	if (dir==NULL) return dirListing;

	struct dirent *entry;
	struct stat fileStat;

	do {
		if ((entry=readdir(dir))!=NULL) {
			string filename=entry->d_name;
			string fullname=path+"/"+entry->d_name;
			if ( (filename.compare(".")!=0) && (filename.compare("..")!=0) ) {
				if (stat(fullname.c_str(),&fileStat)==0) {
					if ((fileStat.st_mode&S_IFMT)==S_IFDIR) {
						dirListing.insert(filename);
					}
				}
			}
		}
	} while (entry!=NULL);
	closedir(dir);

	return dirListing;
}

int lastpage=0;
int lastpageOffset=0;

string pathDialog(string path) {
	SDL_Surface *screen=SDL_GetVideoSurface();
	sdlLock(screen);
	SDL_FillRect(screen, NULL, 0);
	sdlUnlock(screen);
	SDLTTF ttf(FONT,12);

	int page=0;
	int pageOffset=0;

	set<string> dirListing=readPath(path);
	if (dirListing.size()<=1) {
		dirListing.clear();
		dirListing=readPath(path.substr(0,path.rfind("/")));
		path=path.substr(0,path.rfind("/"));
		page=lastpage;
		pageOffset=lastpageOffset;
	}
	vector<string> dirVector=stringSetToVector(dirListing);

	int textHeight=ttf.getTextHeight();
	int linesPerPage=SCREEN_HEIGHT/textHeight;
	string newpath;

	int done=0;
	SDL_Event event;
	sdlClearEvents();
	while (!done) {
		sdlLock(screen);
		SDL_FillRect(screen, NULL, 0);
		for (int i=0;i+page*linesPerPage<(int)dirVector.size()&&i<linesPerPage;i++) {
			if (i==pageOffset) {
                                ttf.setForegroundColor(255,255,0);
			} else {
                                ttf.setForegroundColor(255,255,255);
			}
			ttf.render(screen, dirVector[page*linesPerPage+i], 0, i*textHeight);
		}
		sdlUnlock(screen);

		SDL_Flip(screen);
		SDL_Delay(25);

		while (SDL_PollEvent (&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
                                case SDL_JOYBUTTONDOWN:
					if (
							(event.key.keysym.sym==SDLK_x) ||
							(event.jbutton.button == VK_FX) ||
							(event.jbutton.button == VK_START)
					) {
						done=1;
					}
					if (
							(event.key.keysym.sym==SDLK_UP) ||
							(event.jbutton.button == VK_UP)
					) {
						if ( (page>0) || (pageOffset>0) ) {
							pageOffset--;
							if (pageOffset<0) {
								pageOffset=linesPerPage-1;
								page--;
							}
						}
					}
					if (
							(event.key.keysym.sym==SDLK_DOWN) ||
							(event.jbutton.button == VK_DOWN)
					) {
						if (page*linesPerPage+pageOffset<(int)dirVector.size()-1) {
							pageOffset++;
							if (pageOffset==linesPerPage) {
								pageOffset=0;
								page++;
							}
						}
					}
					if (
							(event.key.keysym.sym==SDLK_SPACE) ||
							(event.jbutton.button == VK_TAT) ||
							(event.jbutton.button == VK_FB)
					) {
						newpath=path;
						if ((path.compare("/")!=0) && (page==0) && (pageOffset==0)) {
							if (path.rfind("/")!=string::npos) {
								if (path.rfind("/")!=0) {
									newpath=path.substr(0,path.rfind("/"));
								} else {
									newpath="/";
								}
							}
						} else {
							newpath+="/"+dirVector[page*linesPerPage+pageOffset];
						}
						dirListing.clear();
						dirListing=readPath(newpath);
						while (dirListing.size()<=1) {
							dirListing.clear();
							newpath=newpath.substr(0,newpath.rfind("/"));
							dirListing=readPath(newpath);
						}
						if (path.compare(newpath)!=0) {
							page=0;
							pageOffset=0;
						}
						path=newpath;
						dirVector.clear();
						dirVector=stringSetToVector(dirListing);
					}
					break;
				case SDL_QUIT:
					done = 1;
					break;
			}
		}
	}

	lastpage=page;
	lastpageOffset=pageOffset;
	sdlClearEvents();
	if ((path.compare("/")!=0) && (page==0) && (pageOffset==0)) {
		return path;
	}
	return path+"/"+dirVector[page*linesPerPage+pageOffset];
}

void saveConfig(Config conf) {
	ofstream out;
	out.open("pv2x.cfg", ios::out|ios::trunc);

	if (!out) {
		return;
	}
	out << conf.path << endl;
	out << conf.delay << endl;
	out << conf.rotateMode << endl;
	out << conf.displayOrder << endl;
	out << conf.showFilename << endl;
	out << conf.plugin << endl;
	out << conf.pluginDelay << endl;
	out.close();
}

void readConfig(Config *conf) {
	ifstream in;
	string line;
	struct stat fileStat;

	string path;
	int delay=5000;
	int rotateMode=0;
	int displayOrder=0;
	int showFilename=0;
	int plugin=0;
	int pluginDelay=3000;

	int i=0;
	in.open("pv2x.cfg");

	if (!in) return;

	while (!in.eof()) {
		in >> line;
		switch (i) {
			case 0:
				path=line;
				break;
			case 1:
				delay=atoi(line.c_str());
				break;
			case 2:
				rotateMode=atoi(line.c_str());
				break;
			case 3:
				displayOrder=atoi(line.c_str());
				break;
			case 4:
				showFilename=atoi(line.c_str());
				break;
			case 5:
				plugin=atoi(line.c_str());
				break;
			case 6:
				pluginDelay=atoi(line.c_str());
				break;
		}
		i++;
	}

	if (stat(path.c_str(),&fileStat)==0) {
		conf->path=path;
	}
	conf->delay=delay;
	conf->rotateMode=rotateMode;
	conf->displayOrder=displayOrder;
	conf->showFilename=showFilename;
	conf->plugin=plugin;
	conf->pluginDelay=pluginDelay;
}
