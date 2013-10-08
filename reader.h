#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "mesh.h"

using namespace std;

class Reader
{
	private:
		static vector<string> split(const string &s, char delim);

	public:
		static void readObj(const char* name, Mesh* m);
		static bool readMtl(const char* s, Mesh* m);
		static int* readPPM(const char* s);
		Group* searchForExistingGroup(string name);

};
