#ifndef FILELISTRANDOMIZER_H
#define FILELISTRANDOMIZER_H

#include <SDL/SDL.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <map>
#include "filelist.h"
#include "misctools.h"

class FileListRandomizer:FileList {
private:
	FileList *fileList;
	int numberOfFiles;
	int counter;
	map<int,int> randomList;
	vector<string> files;
	
public:
	FileListRandomizer(FileList *fl);
	~FileListRandomizer();

	void setFileList(FileList *fl);
	string getFileName(int mode=0, int nextoffset=1);
};

#endif
