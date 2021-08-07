#include "filelistrandomizer.h"

FileListRandomizer::FileListRandomizer(FileList *fl) {
	randomInit();
	this->setFileList(fl);

}

FileListRandomizer::~FileListRandomizer() {
}

void FileListRandomizer::setFileList(FileList *fl) {
	this->fileList=fl;
	this->files.clear();
	this->files=stringSetToVector(fl->getFiles());
	this->numberOfFiles=this->fileList->getNumberOfFiles();
	this->counter=-1;
	this->randomList.clear();
	int tmp;
	for (int i=0;i<this->numberOfFiles;i++) {
		tmp=rand()%(this->numberOfFiles);
		while (this->randomList.find(tmp)!=this->randomList.end()) {
			tmp=rand()%(this->numberOfFiles);
		}
		this->randomList[i]=tmp;
	}
}

string FileListRandomizer::getFileName(int mode, int nextoffset) {
	this->counter+=nextoffset;
	if (this->counter>=this->numberOfFiles) {
		this->counter=0;
	}
	if (this->counter<0) {
		this->counter=this->numberOfFiles-1;
	}
	
	if (mode==0) { // random
		int tmp=this->randomList[this->counter];
		return (string)this->files.at(tmp);
	}
	return (string)this->files.at(this->counter);
}
