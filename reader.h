#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "image.h"
#include "mesh.h"

using namespace std;

class Reader
{
	private:
		static vector<string> split(const string &s, char delim, bool ignoreEmpty);

	public:
		static void readObj(const char* name, Mesh* m);
		static bool readMtl(const char* s, Mesh* m);
		static Image* readPpm(string s);
		Group* searchForExistingGroup(string name);

};
