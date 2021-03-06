#include <Windows.h> 
#include "camera.h"
#include "reader.h"
#include <stdio.h> 
#include <string.h>
#include <utility>
#include <GL/gl.h> 
#include <GL/glut.h>
  
using namespace std;

#define BUFSIZE 512

int width = 800, height = 600;

bool typeMode = false;

string objName = "exemplosOBJ\\torreDiPisa.obj";

Mesh* mesh;

Camera* camera;

int currentTime, frames = 0, timebase;

char s[20];

unsigned int* ids;

void loadOBJ(const char* s){
	mesh = new Mesh();
	Material::setTextCount(0);
	Reader::readObj(s, mesh);	
	
	int textCount = Material::getTextCount();

	ids = new unsigned int[textCount];
	glGenTextures(textCount, ids);
	int k = 0;
	for(pair<string, Material*> p : mesh->getMats()){
		Material* mtl = get<1>(p);
		if(mtl->hasText()){
			Image* img = Reader::readPpm(mtl->getTextName().c_str());
			mtl->setID(ids[k++]);
			glBindTexture(GL_TEXTURE_2D, mtl->getID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			// 0x80E1
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->getWidth(), img->getHeight(), 0, 0x80E1, GL_UNSIGNED_BYTE, img->getPixels());
			free(img->getPixels());
		}
	}
}

void processHits (GLint hits, GLuint buffer[])
{
   GLuint names, *ptr, minZ,*ptrNames;

   ptr = (GLuint *) buffer;
   minZ = 0xffffffff;
   for (unsigned int i = 0; i < hits; i++) {	
      names = *ptr;
	  ptr++;
	  if (*ptr < minZ) {
		  minZ = *ptr;
		  ptrNames = ptr+2;
	  }
	  
	  ptr += names+2;
	}
  ptr = ptrNames;
  mesh->getGroupAt(*ptr)->setVisible(false);
   
}

void calculeFps(){
	frames++;

	if (currentTime - timebase > 1000) {
		timebase = currentTime;
		frames = 0;
	}
}

void switch2D(void){
	//Switch to 2D
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void display2d(const char* msg, float x, float y, float l_size, void* font){		
	for(unsigned int i=0;i<strlen(msg);++i){
		glRasterPos2f(x, y);
		glutBitmapCharacter(font, msg[i]);
		x += l_size;
	}
}

void drawScene(){ 
    // Clear the window with current clearing color 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	
	mesh->render(GL_RENDER);
	
	switch2D();
	//Draw 2D stuff
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	glColor3f(0.0f,1.0f,1.0f);
	sprintf(s,"FPS: %4.2f", frames*1000.0/(currentTime-timebase));
	display2d(s, 0, 0, 20, GLUT_BITMAP_TIMES_ROMAN_24);

	if(typeMode){
		glColor3f(1.0f, 0.0f ,1.0f);
		display2d(objName.c_str(), 0, height / 2, 20, GLUT_BITMAP_HELVETICA_18);
	}

	//Go back to 3D
	camera->resetView(width, height);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	
    //Buffer swap
	glutSwapBuffers();
    glFlush();
} 

void handleKeypress(unsigned char key, int x, int y) {
	if(key == 27){//escape
		exit(0);
	}
	else
		if(typeMode){
			switch(key){
				case 8://backspace
					if(objName.size())
						objName.pop_back();
					break;
				case 13://enter
					loadOBJ(objName.c_str());
					typeMode = false;	
					break;
				default:
					objName += key;
			}
	} else {
		switch(key){
			case 'a':
			case 'A':
				camera->moveSide(1);
				break;
			case 's':
			case 'S':
				camera->move(-1);
				break;
			case 'd':
			case 'D':
				camera->moveSide(-1);
				break;
			case 'w':
			case 'W':
				camera->move(1);
				break;
		}
		glutPostRedisplay();
	}
}

void handleSpecialKey(int key, int x, int y){	

	switch(key){
		case GLUT_KEY_F1:
			if(typeMode){
				typeMode = false;
			}else
				typeMode = true;
			break;
		case GLUT_KEY_F2:
			objName = "exemplosOBJ\\";
			break;
		case GLUT_KEY_F3:
			objName = "exemplosOBJ\\FlorenceBridge\\PonteVecchio.obj";
			break;
		case GLUT_KEY_F4:
			objName = "exemplosOBJ\\LibertyStatue\\LibertStatue.obj";
			break;
		case GLUT_KEY_F5:
			objName = "exemplosOBJ\\torreDiPisa.obj";
			break;
	}
}

int switchSelect(GLuint* selectBuf, int x, int y){
	GLint viewport[4];

	glGetIntegerv (GL_VIEWPORT, viewport);

	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();
	
	gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 
				  5.0, 5.0, viewport);
	gluPerspective(45.0, width / (double)height, 0.2, 500.0);

	mesh->render(GL_SELECT);

	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glFlush();

	return glRenderMode (GL_RENDER);
	
}

void passiveMotionFunc(int x, int y)
{
	float y2 = (height - y) / (float)height;
	if (y2 != 0.5 || x != width/2) 
	{
		if(y2 != 0.5)
		{
			camera->setDirectionY(y2 - 0.5);
		}
		if(x != width/2)
		{
			camera->changeAngle((x - width/2) / 10);
		}
		
		glutWarpPointer(width/2, height/2);
		glutPostRedisplay();
	}
}

void handleResize(int w, int h) {
	if (h < 1) h = 1;
	width  = w;
	height = h;

	glutWarpPointer(width/2, height/2);

	// reestabele a visualização
	camera->resetView(w, h);
}

void handleMouse(int button, int state, int x, int y){

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
	  return;
	  
	GLuint selectBuf[BUFSIZE];
	GLint hits;

	hits = switchSelect(selectBuf, x, y);

	if (hits != 0)
		processHits(hits,selectBuf);
		
	glutPostRedisplay();
}
  
void init() { 
    // inicia propriedades de cor de fundo 
	glClearColor(0.0, 0.0, 1.0, 1.0); // cor do fundo 
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	// glEnable(GL_COLOR_MATERIAL);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glCullFace(GL_BACK);
	
	glutSetCursor(GLUT_CURSOR_NONE);


	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_specular[] = {1.0, 1.0, 1.0,1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0,1.0};

	glShadeModel (GL_SMOOTH);
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	camera = new Camera(90);

	camera->resetView(width, height);
	
	loadOBJ("exemplosOBJ\\bicicleta.obj");
	
	timebase = glutGet(GLUT_ELAPSED_TIME);

} 

void idle()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	calculeFps();	
	glutPostRedisplay();
}
  
int main(int argc, char** argv) { 
  
   	// inicializa glut 
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(width, height);
  
	// cria janela 
	glutCreateWindow("exemplo de visualização de objetos");  
	//glutFullScreen();
	init(); 
  
	// seta funções
	glutDisplayFunc(drawScene);
	glutMouseFunc(handleMouse);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKey);

	glutPassiveMotionFunc(passiveMotionFunc);

	glutIdleFunc(idle);
  
   	glutMainLoop(); 
  
   	return 0; 
} 
