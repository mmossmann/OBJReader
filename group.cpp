#include "group.h"

Group::Group(){
	name = "";
}

Group::Group(string n){
	name = n;
}

vector<Face*> Group::getFaces(void){
	return faces;
}

string Group::getMtl(void){
	return mtl;
}

void Group::addFace(Face* newFace){
	faces.push_back(newFace);
}

void Group::setName(string n){
	name = n;
}

void Group::setMtl(string name){
	mtl = name;
}
