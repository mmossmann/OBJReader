#include "mesh.h"

void Mesh::addGroup(Group* newGroup){
	groups.push_back(newGroup);
}

void Mesh::addVerts(Vertex newV){
	verts.push_back(newV);
}

void Mesh::addNorms(Vertex newN){
	norms.push_back(newN);
}

void Mesh::addTexts(Texts newT){
	texts.push_back(newT);
}

void Mesh::addMats(Material newM){
	string name = newM.getName();
	if(name.size()){
		mats[name] = newM;
	}
}

vector<Group*> Mesh::getGroups(void){
	return groups;
}
Group* Mesh::getGroupAt(int i){
	return groups.at(i);
}

vector<Vertex> Mesh::getVerts(void){
	return verts;
}

vector<Vertex> Mesh::getNorms(void){
	return norms;
}

vector<Texts> Mesh::getTexts(void){
	return texts;
}

map<string, Material> Mesh::getMats(void){
	return mats;
}

Material Mesh::getMtl(string name){
	return mats[name];
}

void Mesh::render(void){
	
	glColor3f(0.0, 0.8, 0.7);
	
	int glMode = GL_TRIANGLES;
	int oldSides = 3;
	
	glBegin(glMode);
	for(Group* g : groups){		
		
		string mtlName = g->getMtl();
		
		if(!mtlName.empty()){
			Material mtl = getMtl(mtlName);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
			glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
			glMaterialf(GL_FRONT, GL_SHININESS, mtl.getShininess());
		}
		
		for(Face* f : g->getFaces()){
		
			vector<int> v = f->getVerts();
			vector<int> n = f->getNorms();
			vector<int> t = f->getTexts();
			
			bool hasNorm = !n.empty();
			bool hasText = !t.empty();
			
			int nv = v.size();
			
			if(nv != oldSides){
				glEnd();				
				glMode = nv == 3 ? GL_TRIANGLES : nv == 4 ? GL_QUADS : GL_POLYGON;
				glBegin(glMode);
				oldSides = nv;
			}	
			
			for(int x = 0; x < nv; ++x){
				//*
				if(hasNorm) {
					glNormal3fv(norms[n[x]].getCoords());
				}
				if(hasText){
					glTexCoord2fv(texts[t[x]].getCoords());
				}
				glVertex3fv(verts[v[x]].getCoords());/**/
				//glArrayElement(v[x]);
			}
			
			if(glMode == GL_POLYGON){
				glEnd();
				glBegin(GL_POLYGON);
			}	
		}
	}
	glEnd();
}