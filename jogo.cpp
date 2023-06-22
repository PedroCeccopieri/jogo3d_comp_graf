
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <math.h>
#include <vector>

#include "Background.h"

#include "Entity.h"
#include "Character.h"
#include "Coin.h"
#include "Bullet.h"
#include "Hole.h"
#include "utils.h"

int width = 500;
int height = 500;
float aspect = width / height;

int frame = 0;

float x_min = -18, y_min = -18;
float x_max = 18, y_max = 18;

int qnt = 10;

int previousScreen = -1, currentScreen, nextScreen, furthestScreen = -1;

float xcamera = 0, ycamera = 0, zcamera = 30;

bool keystates[256];

bool newScreen = true;
int coinsNumber;

Background background(x_max, y_max, qnt);

Character character(-10,-8,10);

std::vector<Coin> coins;
std::vector<Bullet> bullets;
std::vector<Hole> holes;

void refresh();
void generateScreen(int screen);

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::string score = "Score: " + std::to_string(character.getPoints());
	char* ca = new char[score.length() + 1];
	strcpy(ca, score.c_str());
	drawText(xcamera-10,10,ca);

	background.draw();
	character.draw();

	for (int i = 0; i < coins.size(); i++) coins[i].draw();
	for (int i = 0; i < bullets.size(); i++) bullets[i].draw();
	for (int i = 0; i < holes.size(); i++) holes[i].draw();

	glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y) {
	keystates[key] = true;

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
		xcamera++;
		refresh();
		gluLookAt(xcamera,ycamera,zcamera,xcamera,0,0,0,1,0);
	}
	if (keystates['n']) {
		xcamera--;
		refresh();
		gluLookAt(xcamera,ycamera,zcamera,xcamera,0,0,0,1,0);
	}

	if (keystates['z']) character.setRun(true);
	else character.setRun(false);
	if (keystates['x']) character.setShoot(true);
	else character.setShoot(false);

	if (keystates[GLUT_KEY_RIGHT]){
		character.movePosX(1);
		character.resetInterpx();
	}
	if (keystates[GLUT_KEY_LEFT]) {
		character.movePosX(-1);
		character.resetInterpx();
	} 
	if (keystates[GLUT_KEY_UP]) {
		character.movePosY(10);
		character.resetInterpy();
		character.setJumped();
	}
}

void nextFrame(int f) {
	frame++;

	keysAction();

	character.updatePos();
	character.updateState();
	
	if (character.getPosx() < 0) xcamera = 0;
	else if (character.getPosx() > x_max*(qnt-1)) xcamera = x_max*(qnt-1);
	else xcamera = character.getPosx();

	refresh();
	gluLookAt(xcamera,ycamera,zcamera,xcamera,0,0,0,1,0);

	for (int i = 0; i < qnt; i++) {
		if (x_min + 18 * i < xcamera and xcamera < x_max + 18 * i){
			previousScreen = i-1;
			currentScreen = i;
			nextScreen = i+1;
			if (currentScreen > furthestScreen) {
				furthestScreen = currentScreen;
				newScreen = true;
			}
		}
	}

	generateScreen(nextScreen);
	
	character.animate();

	for (int i = 0; i < coins.size(); i++) {
		if (coins[i].getPosx() > xcamera + x_min && coins[i].getPosx() < xcamera + x_max) {
			coins[i].animate();	
			if (character.checkcolision(coins[i].getHitbox())) {
				character.addPoint(coins[i].getPoint());
				coins.erase(coins.begin()+i);
			}
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].getPosx() > xcamera + x_min && bullets[i].getPosx() < xcamera + x_max) {
			bullets[i].animate();
			if (character.checkcolision(bullets[i].getHitbox())) {
				character.addPoint(-5);
				bullets.erase(bullets.begin()+i);
			}
		}
	}

	for (int i = 0; i < holes.size(); i++) {
		if (holes[i].getPosx() > xcamera + x_min && holes[i].getPosx() < xcamera + x_max) {
			holes[i].animate();
			if (character.checkcolision(holes[i].getHitbox())) {
				character.addPoint(-5);
				holes.erase(holes.begin()+i);
			}
		}
	}

	if (character.getPoints() < 0 || character.getPosx() > xcamera + x_max) exit(0);

	glutPostRedisplay();
    glutTimerFunc(16,nextFrame,0);
}

void generateScreen(int screen) {
	if (newScreen) {
		newScreen = false;

		int x, y;

		coinsNumber = 1 + std::rand() % 5;
		for (int i = 0; i < coinsNumber; i++) {
			x = x_min + 18 * screen + std::rand() % (int)(2*x_max);
			y = -(std::rand() % 9);
			int p = std::rand() % 4;
			coins.push_back(Coin(x,y,9,p));
		}

		x = x_min + 18 * screen + std::rand() % (int)(2*x_max);
		int aux = std::rand() % 10;
		if (aux < 5) y = -4;
		else y = -8;
		bullets.push_back(Bullet(x,y,9));

		x = x_min + 18 * screen + std::rand() % (int)(2*x_max);
		holes.push_back(Hole(x,-10,0.5));
	}
}

void refresh() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,1,2.0,50.0);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	width = w;
	height = h;
	aspect = (float) width/height;

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	refresh();
	gluLookAt(xcamera,ycamera,zcamera,xcamera,0,0,0,1,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

void init() {
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	for (int i = 0; i < 256; i++) keystates[i] = false;
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

