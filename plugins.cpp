#include "plugins.h"

Plugins::Plugins() {
}

Plugins::~Plugins() {
}

void Plugins::scan() {
	DIR *dir=opendir("plugins");
	if (dir==NULL) return;

	set<string> plugins;
	struct dirent *entry;
	do {
		 if ((entry=readdir(dir))!=NULL) {
			 string filename="plugins/";
			 filename+=entry->d_name;
			 string lowerFilename=filename;
			 transform (lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
			 if (lowerFilename.rfind(".so")==filename.length()-3) {
				 plugins.insert(filename);
			 }
		 }
	} while (entry!=NULL);
	closedir(dir);

	pluginNames.push_back("None");

	if (plugins.size()==0) return;

	pluginNames.push_back("Randon");

	set<string>::iterator iter;
	string filename;
	void *handle;
	char *(*name)();
	char *error;
	iter=plugins.begin();
	while (iter!=plugins.end()) {
		this->pluginFiles.push_back(*iter);
		string dlname=*iter;
		handle=dlopen(dlname.c_str(),RTLD_LAZY);
		if (handle) {
			dlerror();
			*(void **) (&name)=dlsym(handle,"pv2x_pluginName");
			if ((error=dlerror())==NULL) {
				this->pluginNames.push_back((*name)());
			} else {
				cout << error << endl;
			}
			dlclose(handle);
		} else {
			cout << dlerror() << endl;
		}
		iter++;
	}
}

void Plugins::call(int pnum, SDL_Surface *screen, SDL_Surface *src, SDL_Surface *dst, Uint32 time) {
	dlerror();
	void *(*fade)(SDL_Surface*, SDL_Surface*, SDL_Surface*);
	char *error;
	int plugin;

	if ( (pnum==0) || (this->pluginFiles.size()==0) ) return;
	if (pnum==1) {
		plugin=rand()%this->pluginFiles.size();
	} else {
		plugin=pnum-2;
	}

	initPluginTimer(time);
	void *handle;
	handle=dlopen(this->pluginFiles[plugin].c_str(),RTLD_LAZY);
	if (handle) {
		dlerror();
		*(void **) (&fade)=dlsym(handle,"pv2x_fade");
		if ((error=dlerror())==NULL) {
			(*fade)(screen,src,dst);
		} else {
			cout << error << endl;
		}
		dlclose(handle);
	} else {
		cout << dlerror() << endl;
	}
}

vector<string> Plugins::getNames() {
	return this->pluginNames;
}
