#include "reader.h"

void Reader::readObj(const char* name, Mesh* m){

	ifstream in;

	string buffer;
	vector<string> tokens;
	vector<string> indexes;

	Group* g = new Group();
	m->addGroup(g);
	Face* f;
	Vertex v;
	Texts t;
	string mtl;
	
	in.open(name);
	
	if(!in.is_open()){
		cout<<"Arquivo nao foi aberto com sucesso: "<<name<<endl;
		return;
	}
	
	while(!in.eof()){
		getline(in, buffer);
	
		tokens = split(buffer, ' ', false);
		
		switch(buffer[0]){
			case 'v':
				if(tokens.size() > 3){
					v = Vertex(
								atof(tokens[1].c_str()), 
								atof(tokens[2].c_str()), 
								atof(tokens[3].c_str()));
				}else{
					t = Texts(
								atof(tokens[1].c_str()), 
								atof(tokens[2].c_str()));
				}
				
				switch(buffer[1]){
					case 't':
						m->addTexts(t);
						break;
					case 'n':
						m->addNorms(v);
						break;
					default:
						m->addVerts(v);
						break;
				}
				break;
				
			case 'f':
				f = new Face();
				g->addFace(f);
				for(unsigned int x = 1; x < tokens.size(); ++x){
					indexes = split(tokens[x], '/', true);
					f->addVert(atoi(indexes[0].c_str()) - 1);
					if(indexes.size() > 1)
						if(indexes[1].size() > 0)
							f->addText(atoi(indexes[1].c_str()) - 1);						
					if(indexes.size() > 2) 
						f->addNorm(atoi(indexes[2].c_str()) - 1);
				}
				break;
				
			case 'g':
			
				if(!g->getFaces().empty()){
				
					if(tokens.size() == 1){
						g = new Group();
					}
					else {
						g = new Group(tokens[1]);
					}
					m->addGroup(g);
				}
				break;
				
			case 'm':
				mtl = tokens[1];
				break;
				
			case 'u':
				g->setMtl(tokens[1]);
				break;
		}
	}
	
	if(mtl.empty()){
		for(Group* g1 : m->getGroups()){
			g1->setMtl("");
		}
	}else{
		string n(name);
		mtl = n.substr(0, n.rfind('\\')) + '\\' + mtl;
		if(!readMtl(mtl.c_str(), m)){
			for(Group* g1 : m->getGroups()){
				g1->setMtl("");
			}
		}
	}
}

bool Reader::readMtl(const char* s, Mesh* m){

	ifstream in;

	string buffer;
	vector<string> tokens;
	vector<string> indexes;

	Material mtl;
	
	in.open(s);
	if(!in.is_open()){
		cout<<"Arquivo nao foi aberto com sucesso: "<<s<<endl;
		return false;
	}
	cout<<"Arquivo aberto com sucesso: "<<s<<endl;
	
	while(!in.eof()){
	
		getline(in, buffer);
		
		tokens = split(buffer, ' ', false);
		
		switch(buffer[0]){
			case 'K':
			case 'k':
				switch(buffer[1]){
					case 'd':
						mtl.setDiffuse(
								atof(tokens[1].c_str()), 
								atof(tokens[2].c_str()), 
								atof(tokens[3].c_str()));
						break;
					case 's':
						mtl.setSpecular(
								atof(tokens[1].c_str()), 
								atof(tokens[2].c_str()), 
								atof(tokens[3].c_str()));
						break;
					case 'a':
						mtl.setAmbient(
								atof(tokens[1].c_str()), 
								atof(tokens[2].c_str()), 
								atof(tokens[3].c_str()));
						break;
				}
				break;
				
			case 'N':
			case 'n':
				switch(buffer[1]){
					case 's':
						mtl.setShininess(atof(tokens[1].c_str()));
						break;
					case 'e':
						m->addMats(mtl);
						mtl = Material(tokens[1]);
						break;
				}
				break;
				
			case 'M':
			case 'm':
				//TODO ler textura
				break;
		}
	}
	m->addMats(mtl);
	return true;
}

vector<string> Reader::split(const string &s, char delim, bool empty) {
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		if(!empty && item.empty()){
			continue;
		}
		elems.push_back(item);
	}
	return elems;
}
