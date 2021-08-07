#include "filelist.h"

void FileList::init() {
	// this->files=new vector<string>;
	this->screen=SDL_GetVideoSurface();
	this->ttf=new SDLTTF(FONT, 12);

	this->lastDraw=0;
	this->drawDirection=0;
	this->gray=255;
}

FileList::FileList(void) {
	this->init();
	this->suffixFilter=NULL;
}

FileList::FileList(list<string> *suffixFilter) {
	this->init();
	this->suffixFilter=suffixFilter;
}

FileList::FileList(list<string> *suffixFilter, string path) {
	this->init();
	this->suffixFilter=suffixFilter;
	this->scanDir(path);
}

FileList::~FileList() {
	if (this->suffixFilter!=NULL) {
		delete this->suffixFilter;
	}
	// delete this->files;
	delete this->ttf;
}

void FileList::resetList() {
	this->files.clear();
	this->lastDraw=0;
	this->drawDirection=0;
	this->gray=255;
}

void FileList::scanDir(string path) {
	DIR *dir=opendir(path.c_str());
	if (dir==NULL) return;

	struct dirent *entry;
	struct stat fileStat;
	do {
		if (SDL_GetTicks()-this->lastDraw>50) {
			sdlLock(screen);
			this->ttf->setForegroundColor(this->gray,this->gray,this->gray);
			this->ttf->render(this->screen,"SCANNING FOR FILES...",80,110);
			sdlUnlock(screen);
			SDL_Flip(screen);
			if (this->drawDirection) {
				this->gray+=10;
				if (this->gray>=255) {
					this->drawDirection=0;
				}
			} else {
				this->gray-=10;
				if (this->gray<100) {
					this->drawDirection=1;
				}
			}
			this->lastDraw=SDL_GetTicks();
		}
		 if ((entry=readdir(dir))!=NULL) {
			 string filename=path+"/"+entry->d_name;
			 if (stat(filename.c_str(),&fileStat)==0) {
				 if ((fileStat.st_mode&S_IFMT)==S_IFDIR) {
					if (
						(strcmp(entry->d_name,".")!=0) &&
						(strcmp(entry->d_name,"..")!=0)
					) {
						this->scanDir(filename);
					}
				 } else {
					 if (this->suffixFilter==NULL) {
						 this->files.insert(filename);
					 } else {
						 list<string>::iterator iter;
						 for (iter=this->suffixFilter->begin();iter!=this->suffixFilter->end();iter++) {
							 string lowerFilename=filename;
							 string lowerSuffix=*iter;
							 transform (lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
							 transform (lowerSuffix.begin(), lowerSuffix.end(), lowerSuffix.begin(), ::tolower);
							 if (lowerFilename.rfind(lowerSuffix)==filename.length()-(*iter).length()) {
						 		this->files.insert(filename);
							 }
						 }
					 }
				 }
			 }
		 }
	} while (entry!=NULL);
	closedir(dir);
}

set<string> FileList::getFiles() {
	return this->files;
}

int FileList::getNumberOfFiles() {
	return this->files.size();
}
