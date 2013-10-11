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

void Mesh::addMats(Material* newM){
	string name = newM->getName();
	if(!name.empty()){
		mats[name] = newM;
	}
}

vector<Group*> Mesh::getGroups(void){
	return groups;
}

Group* Mesh::getGroupAt(int i){
	return groups[i];
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

map<string, Material*> Mesh::getMats(void){
	return mats;
}

Material* Mesh::getMtl(string name){
	return mats[name];
}

void Mesh::render(int renderMode){

	glColor3f(1.0, 1.0, 1.0);
	
	int glMode = GL_TRIANGLES;
	int oldSides = 3;
	
	int name = 0;
	int currentID = 0;
	glBindTexture(GL_TEXTURE_2D, currentID);
	
	for(Group* g : groups){	
	
		if(renderMode == GL_SELECT){
			glLoadName(name++);
		}
		
		if(!g->getVisible()){
			continue;
		}
		
		string mtlName = g->getMtl();
		
		if(!mtlName.empty()){
			Material* mtl = getMtl(mtlName);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mtl->getSpecular());
			glMaterialfv(GL_FRONT, GL_AMBIENT, mtl->getAmbient());
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl->getDiffuse());
			glMaterialf(GL_FRONT, GL_SHININESS, mtl->getShininess());
			
			
			int tID = mtl->getID();
			if(tID != currentID){
				currentID = tID;
				glBindTexture(GL_TEXTURE_2D, currentID);
			}
			
		}
		
		glBegin(glMode);	
		
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
				if(hasNorm) {
					glNormal3fv(norms[n[x]].getCoords());
				}
				if(hasText){
					glTexCoord2fv(texts[t[x]].getCoords());
				}
				glVertex3fv(verts[v[x]].getCoords());
			}
			
			if(glMode == GL_POLYGON){
				glEnd();
				glBegin(GL_POLYGON);
			}	
		}
		glEnd();
	}
}