
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <filesystem>

#include "Skybox.h"
#include "Entity.h"
#include "Character.h"
#include "Coin.h"
#include "Bullet.h"
#include "Hole.h"
#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int width = 500;
int height = 500;
float aspect = width / height;

int frame = 0;

int skyboxSize = 300;

float xmax = skyboxSize/2, zmax = skyboxSize/2;
float xmin = -skyboxSize/2, zmin = -skyboxSize/2;

float rcamera = 140;
float xcameraPos = 30, ycameraPos = 30, zcameraPos = 40;
float xcameraFocus = 0, ycameraFocus = 0, zcameraFocus = 0;
bool cameras[4] = {false,false,false,true};

float ambientLight0[4] = {0.7, 0.7, 0.7, 1.0};
float diffuseLight0[4] = {0.7, 0.7, 0.7, 1.0};
float specularLight0[4] = {0.2, 0.2, 0.2, 1.0};
float posLight0[4] = {0.0, 1.0, 0.0, 0.0};

float ambientLight1[4] = {0.3, 0.3, 0.3, 1.0};
float diffuseLight1[4] = {0.3, 0.3, 0.3, 1.0};
float specularLight1[4] = {0.3, 0.3, 0.3, 1.0};
float posLight1[4] = {0, 0.1, 0, 1.0};

bool keystates[256];

bool newScreen = true;

int coinsLimit = 20, bulletsLimit = 20, holesLimit = 5;
int holeSize = 2;

std::string path = std::filesystem::current_path().string() + "\\textures\\";

std::vector<unsigned int> characterTexture;
std::vector<unsigned int> skyboxTexture;
std::vector<unsigned int> coinTexture;
std::vector<unsigned int> bulletTexture;
std::vector<unsigned int> holeTexture;

Skybox *skybox;
Character *character;

std::vector<Coin> coins;
std::vector<Bullet> bullets;
std::vector<Hole> holes;

void refresh();
void generateCoins();
void generateBullets();
void checkBulletsOut();
void generateHoles();
void resetCameras();

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::string score = "Score: " + std::to_string((*character).getPoints());
	char* ca = new char[score.length() + 1];
	strcpy(ca, score.c_str());
	drawText((*character).getPosx(),(*character).getPosy() + 8,(*character).getPosz(),ca);
	
	(*skybox).draw();
	(*character).draw();

	for (int i = 0; i < coins.size(); i++) coins[i].draw();
	for (int i = 0; i < holes.size(); i++) holes[i].draw();
	for (int i = 0; i < bullets.size(); i++) bullets[i].draw();
	
	refresh();

	if (cameras[0]) {
		xcameraPos = (*character).getPosx();
		ycameraPos = (*character).getPosy() + 4;
		zcameraPos = (*character).getPosz();
		xcameraFocus = (*character).getPosx() + 30*cos(-(*character).getRotation());
		ycameraFocus = (*character).getPosy();
		zcameraFocus = (*character).getPosz() + 30*sin(-(*character).getRotation());
	}

	if (cameras[1]) {
		xcameraPos = (*character).getPosx();
		ycameraPos = (*character).getPosy() + 4;
		zcameraPos = (*character).getPosz();
		xcameraFocus = (*character).getPosx() + 30*cos(-(*character).getRotation() + M_PI);
		ycameraFocus = (*character).getPosy();
		zcameraFocus = (*character).getPosz() + 30*sin(-(*character).getRotation() + M_PI);
	}
	
	if (cameras[2]) {
		xcameraPos = 30*cos(rcamera*M_PI/180);
		ycameraPos = 30;
		zcameraPos = 30*sin(rcamera*M_PI/180);
		xcameraFocus = 0;
		ycameraFocus = 0;
		zcameraFocus = 0;
	}

	if (cameras[3]) {
		xcameraPos = (*character).getPosx() + 30*cos(-(*character).getRotation() + M_PI);
		ycameraPos = (*character).getPosy() + 10;
		zcameraPos = (*character).getPosz() + 30*sin(-(*character).getRotation() + M_PI);
		xcameraFocus = (*character).getPosx();
		ycameraFocus = (*character).getPosy();
		zcameraFocus = (*character).getPosz();
	}

	gluLookAt(xcameraPos,ycameraPos,zcameraPos,xcameraFocus,ycameraFocus,zcameraFocus,0,1,0);

	glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y) {
	keystates[key] = true;

	if (key == '1') {
		resetCameras();
		cameras[0] = true;
	}

	if (key == '2') {
		resetCameras();
		cameras[2] = true;
	}

	if (key == '3') {
		resetCameras();
		cameras[3] = true;
	}
}

void specialkeys (int key, int x, int y) {
	keystates[key] = true;
}

void keyboardUp (unsigned char key, int x, int y) {
	keystates[key] = false;
}

void specialkeysUp (int key, int x, int y) {
	keystates[key] = false;
}

void keysAction() {

	if (keystates['m']) {
		rcamera++;
		refresh();
		gluLookAt(xcameraPos,ycameraPos,zcameraPos,xcameraFocus,ycameraFocus,zcameraFocus,0,1,0);
	}
	if (keystates['n']) {
		rcamera--;
		refresh();
		gluLookAt(xcameraPos,ycameraPos,zcameraPos,xcameraFocus,ycameraFocus,zcameraFocus,0,1,0);
	}

	if (keystates['r']) (*character).setRun(true);
	else (*character).setRun(false);
	if (keystates['x']) (*character).setShoot(true);
	else (*character).setShoot(false);

	if (keystates['w']) {
		(*character).movePosXZ(3);
		(*character).resetInterpx();

		if (cameras[1]) {
			resetCameras();
			cameras[0] = true;
		}
	}
	if (keystates['s']) {
		(*character).movePosXZ(-3);
		(*character).resetInterpx();

		if (cameras[0]) {
			resetCameras();
			cameras[1] = true;
		}
	}

	if (keystates['a']) {
		(*character).rotate(5);
	}
	if (keystates['d']) {
		(*character).rotate(-5);
	}

	if (keystates[' ']) {
		(*character).movePosY(10);
		(*character).resetInterpy();
		(*character).setJumped();
	}
}

void nextFrame(int f) {
	frame++;

	keysAction();

	(*character).updatePos();
	(*character).updateState();
	(*character).animate();

	if (frame > 120) {
		generateCoins();
		generateBullets();
		generateHoles();
	}
	

	for (int i = 0; i < coins.size(); i++) {
		coins[i].animate();
		if ((*character).checkcolision(coins[i].getHitbox())) {
			(*character).addPoint(coins[i].getPoint());
			coins.erase(coins.begin()+i);
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].animate();
		if ((*character).checkcolision(bullets[i].getHitbox())) {
			(*character).addPoint(-5);
			bullets.erase(bullets.begin()+i);
		}
	}

	for (int i = 0; i < holes.size(); i++) {
		holes[i].animate();
		if ((*character).checkcolision(holes[i].getHitbox())) {
			(*character).addPoint(-5);
			holes.erase(holes.begin()+i);
		}
	}

	if ((*character).getPoints() < 0 || (*character).getPoints() >= 100) exit(0);

	glutPostRedisplay();
    glutTimerFunc(16,nextFrame,0);
}

void generateCoins() {

	if (coins.size() < coinsLimit) {

		int x, y, z;
		int coinsNumber = 1 + std::rand() % 5;

		for (int i = 0; i < coinsNumber; i++) {
			x = (std::rand() % (int)(skyboxSize/2)) * pow(-1, std::rand() % 2);
			y = 2 + std::rand() % 20;
			z = (std::rand() % (int)(skyboxSize/2)) * pow(-1, std::rand() % 2);
			int p = std::rand() % 4;
			coins.push_back(Coin(x,y,z,p,coinTexture));
		}
	}
}

void generateBullets() {

	checkBulletsOut();

	if (bullets.size() < bulletsLimit) {

		int x, y, z;
		int bulletsNumber = 1 + std::rand() % 10;

		for (int i = 0; i < bulletsNumber; i++) {
			x = (std::rand() % (int)(skyboxSize/2)) * pow(-1, std::rand() % 2);
			y = 2 + std::rand() % 20;
			z = (std::rand() % (int)(skyboxSize/2)) * pow(-1, std::rand() % 2);
			bullets.push_back(Bullet(x,y,z,0.3 * -x/abs(x),0.3 * -z/abs(z),bulletTexture));
		}
	}
}

void checkBulletsOut() {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].getPosx() > skyboxSize/2 || bullets[i].getPosx() < -skyboxSize/2 || bullets[i].getPosz() > skyboxSize/2 || bullets[i].getPosz() < -skyboxSize/2)
			bullets.erase(bullets.begin()+i);
	}
}

void generateHoles() {
	if (holes.size() < holesLimit) {

		int x, y, z;
		int holesNumber = 1 + std::rand() % 5;

		for (int i = 0; i < holesNumber; i++) {
			x = (std::rand() % (int)(skyboxSize/2) - 4 * holeSize) * pow(-1, std::rand() % 2);
			y = 0;
			z = (std::rand() % (int)(skyboxSize/2) - 4 * holeSize) * pow(-1, std::rand() % 2);
			holes.push_back(Hole(x,y,z,holeSize,holeSize,holeTexture));
		}
	}
}

void resetCameras() {
	for (int i = 0; i < sizeof(cameras) / sizeof(bool); i++) cameras[i] = false;
}

void refresh() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70,1,2.0,1000.0);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	width = w;
	height = h;
	aspect = (float) width/height;

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	refresh();
	gluLookAt(xcameraPos,ycameraPos,zcameraPos,xcameraFocus,ycameraFocus,zcameraFocus,0,1,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

unsigned int loadTexture(const char *imagePath) {
	int width, height, nrChannels;
	unsigned char *data;
	unsigned int texId;

	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

	if (data) {
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else {
		 std::cout << "Failed to load texture" << std::endl;
		 std::cout << stbi_failure_reason() << std::endl;
	}

	stbi_image_free(data);

	return texId;
}

void init() {

	glClearColor(0, 0, 0, 1); // #####
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_COLOR_MATERIAL);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, posLight0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, posLight1);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);
	
	for (int i = 0; i < 256; i++) keystates[i] = false;

	characterTexture.push_back(loadTexture((path + "blank.jpg").c_str()));
	skyboxTexture.push_back(loadTexture((path + "blank.jpg").c_str()));
	coinTexture.push_back(loadTexture((path + "blank.jpg").c_str()));
	bulletTexture.push_back(loadTexture((path + "blank.jpg").c_str()));
	holeTexture.push_back(loadTexture((path + "blank.jpg").c_str()));

	characterTexture.push_back(loadTexture((path + "shirt.jpg").c_str()));
	characterTexture.push_back(loadTexture((path + "skin.jpg").c_str()));
	characterTexture.push_back(loadTexture((path + "short.jpg").c_str()));
	
	skyboxTexture.push_back(loadTexture((path + "grass.jpg").c_str()));
	skyboxTexture.push_back(loadTexture((path + "0.jpg").c_str()));
	skyboxTexture.push_back(loadTexture((path + "90.jpg").c_str()));
	skyboxTexture.push_back(loadTexture((path + "180.jpg").c_str()));
	skyboxTexture.push_back(loadTexture((path + "270.jpg").c_str()));

	coinTexture.push_back(loadTexture((path + "coin0.jpg").c_str()));
	coinTexture.push_back(loadTexture((path + "coin1.jpg").c_str()));
	coinTexture.push_back(loadTexture((path + "coin2.jpg").c_str()));
	coinTexture.push_back(loadTexture((path + "coin3.jpg").c_str()));

	bulletTexture.push_back(loadTexture((path + "bullet.jpg").c_str()));

	holeTexture.push_back(loadTexture((path + "hole.jpg").c_str()));

	static Skybox s(skyboxSize, skyboxTexture);
	static Character c(0,8,0,characterTexture);
	
	skybox = &s;
	character = &c;
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100,100);

    glutCreateWindow("Jogo");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutTimerFunc(16,nextFrame,0);

	glutIgnoreKeyRepeat(1);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeys);

	glutKeyboardUpFunc(keyboardUp);
	glutSpecialUpFunc(specialkeysUp);

    init();

    glutMainLoop();
    return 0;
}

