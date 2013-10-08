#include "face.h"
#include <string>

using namespace std;

class Group{
	private:
		vector<Face*> faces;
		string name;
		string mtl;
		bool visible;

	public:
		Group();
		Group(string n);
		vector<Face*> getFaces(void);
		string getMtl(void);
		bool getVisible(void);

		void addFace(Face* newFace);
		void setName(string n);
		void setMtl(string name);
		void setVisible(bool b);
};
