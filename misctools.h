#ifndef MISCTOOLS_H
#define MISCTOOLS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <set>
#include <vector>

using namespace std;

void randomInit();
string basename(string path);
string intToString(int i);
vector<string> stringSetToVector(set<string> stringSet);

#endif
