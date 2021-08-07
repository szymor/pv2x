#include "misctools.h"

void randomInit() {
	struct timeb tmp;
	ftime(&tmp);

	srand(tmp.millitm+tmp.time);
}

string basename(string path) {
#ifdef WIN32
	string::size_type pos=path.rfind("\\");
#else
       	string::size_type pos=path.rfind("/");
#endif
	if (pos==string::npos) {
		return path;
	}
	return path.substr(pos+1);
}

string intToString(int i) {
        ostringstream out;
	if (!(out << i)) {
		return string("NaN");
	}
	return out.str();
}

vector<string> stringSetToVector(set<string> stringSet) {
	set<string>::iterator iter;
	vector<string> ret;
	iter=stringSet.begin();
	while (iter!=stringSet.end()) {
		ret.push_back(*iter);
		iter++;
	}
	return ret;
}

