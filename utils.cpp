#include <GL/glut.h>

#include "utils.h"

void color(int r, int g, int b) {
	glColor3f((float)r/255,(float)g/255,(float)b/255);
}

void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, int stroke) {
	glPushMatrix();
	glLineWidth(stroke);
	glBegin(GL_LINES);
		glVertex3f(x1,y1,z1);
		glVertex3f(x2,y2,z2);
	glEnd();
	glPopMatrix();
}

void drawSquare(bool full) {

	if (full) {
		glBegin(GL_QUADS);
		glVertex2f(-0.5,-0.5);
		glVertex2f(0.5,-0.5);
		glVertex2f(0.5,0.5);
		glVertex2f(-0.5,0.5);
		glEnd();
	} else {
		glBegin(GL_LINE_LOOP);
		glVertex2f(-0.5,-0.5);
		glVertex2f(0.5,-0.5);
		glVertex2f(0.5,0.5);
		glVertex2f(-0.5,0.5);
		glEnd();
	}
}

void drawCube(bool full) {

	// glTexCoord2d(1,1);
	// glTexCoord2d(0,1);
	// glTexCoord2d(0,0);
	// glTexCoord2d(1,0);

	// glTexCoord2d(0,0);
	// glTexCoord2d(1,0);
	// glTexCoord2d(1,1);
	// glTexCoord2d(0,1);

	if (full) {
		glNormal3d(0,0,1);
		glBegin(GL_QUADS);
		glTexCoord2d(1,1);
		glVertex3d(-0.5,-0.5,0.5);
		glTexCoord2d(0,1);
		glVertex3d(0.5,-0.5,0.5);
		glTexCoord2d(0,0);
		glVertex3d(0.5,0.5,0.5);
		glTexCoord2d(1,0);
		glVertex3d(-0.5,0.5,0.5);
		glEnd();

		glNormal3d(0,0,-1);
		glBegin(GL_QUADS);
		glTexCoord2d(1,1);
		glVertex3d(0.5,-0.5,-0.5);
		glTexCoord2d(0,1);
		glVertex3d(-0.5,-0.5,-0.5);
		glTexCoord2d(0,0);
		glVertex3d(-0.5,0.5,-0.5);
		glTexCoord2d(1,0);
		glVertex3d(0.5,0.5,-0.5);
		glEnd();

		glNormal3d(1,0,0);
		glBegin(GL_QUADS);
		glTexCoord2d(1,1);
		glVertex3d(0.5,-0.5,0.5);
		glTexCoord2d(0,1);
		glVertex3d(0.5,-0.5,-0.5);
		glTexCoord2d(0,0);
		glVertex3d(0.5,0.5,-0.5);
		glTexCoord2d(1,0);
		glVertex3d(0.5,0.5,0.5);
		glEnd();

		glNormal3d(-1,0,0);
		glBegin(GL_QUADS);
		glTexCoord2d(1,1);
		glVertex3d(-0.5,-0.5,-0.5);
		glTexCoord2d(0,1);
		glVertex3d(-0.5,-0.5,0.5);
		glTexCoord2d(0,0);
		glVertex3d(-0.5,0.5,0.5);
		glTexCoord2d(1,0);
		glVertex3d(-0.5,0.5,-0.5);
		glEnd();

		glNormal3d(0,1,0);
		glBegin(GL_QUADS);
		glTexCoord2d(1,1);
		glVertex3d(-0.5,0.5,0.5);
		glTexCoord2d(0,1);
		glVertex3d(0.5,0.5,0.5);
		glTexCoord2d(0,0);
		glVertex3d(0.5,0.5,-0.5);
		glTexCoord2d(1,0);
		glVertex3d(-0.5,0.5,-0.5);
		glEnd();

		glNormal3d(0,-1,0);
		glBegin(GL_QUADS);
		glTexCoord2d(1,1);
		glVertex3d(-0.5,-0.5,-0.5);
		glTexCoord2d(0,1);
		glVertex3d(0.5,-0.5,-0.5);
		glTexCoord2d(0,0);
		glVertex3d(0.5,-0.5,0.5);
		glTexCoord2d(1,0);
		glVertex3d(-0.5,-0.5,0.5);
		glEnd();
	} else {
		glNormal3d(0,0,1);
		glBegin(GL_LINE_LOOP);
		glTexCoord2d(1,1);
		glVertex3d(-0.5,-0.5,0.5);
		glTexCoord2d(0,1);
		glVertex3d(0.5,-0.5,0.5);
		glTexCoord2d(0,0);
		glVertex3d(0.5,0.5,0.5);
		glTexCoord2d(1,0);
		glVertex3d(-0.5,0.5,0.5);
		glEnd();

		glNormal3d(0,0,-1);
		glBegin(GL_LINE_LOOP);
		glTexCoord2d(1,1);
		glVertex3d(0.5,-0.5,-0.5);
		glTexCoord2d(0,1);
		glVertex3d(-0.5,-0.5,-0.5);
		glTexCoord2d(0,0);
		glVertex3d(-0.5,0.5,-0.5);
		glTexCoord2d(1,0);
		glVertex3d(0.5,0.5,-0.5);
		glEnd();

		glNormal3d(1,0,0);
		glBegin(GL_LINE_LOOP);
		glTexCoord2d(1,1);
		glVertex3d(0.5,-0.5,0.5);
		glTexCoord2d(0,1);
		glVertex3d(0.5,-0.5,-0.5);
		glTexCoord2d(0,0);
		glVertex3d(0.5,0.5,-0.5);
		glTexCoord2d(1,0);
		glVertex3d(0.5,0.5,0.5);
		glEnd();

		glNormal3d(-1,0,0);
		glBegin(GL_LINE_LOOP);
		glTexCoord2d(1,1);
		glVertex3d(-0.5,-0.5,-0.5);
		glTexCoord2d(0,1);
		glVertex3d(-0.5,-0.5,0.5);
		glTexCoord2d(0,0);
		glVertex3d(-0.5,0.5,0.5);
		glTexCoord2d(1,0);
		glVertex3d(-0.5,0.5,-0.5);
		glEnd();

		glNormal3d(0,1,0);
		glBegin(GL_LINE_LOOP);
		glTexCoord2d(1,1);
		glVertex3d(-0.5,0.5,0.5);
		glTexCoord2d(0,1);
		glVertex3d(0.5,0.5,0.5);
		glTexCoord2d(0,0);
		glVertex3d(0.5,0.5,-0.5);
		glTexCoord2d(1,0);
		glVertex3d(-0.5,0.5,-0.5);
		glEnd();

		glNormal3d(0,-1,0);
		glBegin(GL_LINE_LOOP);
		glTexCoord2d(1,1);
		glVertex3d(-0.5,-0.5,-0.5);
		glTexCoord2d(0,1);
		glVertex3d(0.5,-0.5,-0.5);
		glTexCoord2d(0,0);
		glVertex3d(0.5,-0.5,0.5);
		glTexCoord2d(1,0);
		glVertex3d(-0.5,-0.5,0.5);
		glEnd();
	}
}

void drawText(float x, float y, float z, char *string) {
  color(0,0,0);
  glRasterPos3f(x, y, z);
  int len, i;
  len = (float)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
}

void grid(int x_min, int y_min, int x_max, int y_max) {	
	color(255,255,255);
	for (int i = y_min; i < y_max; i++) drawLine(x_max,i,0,x_min,i,0,1);
	for (int i = x_min; i < x_max; i++) drawLine(i,y_max,0,i,y_min,0,1);
}

void eixos() {
	color(0,0,255); // z verde
	drawLine(0,0,-10,0,0,10,5);
	color(0,255,0); // y azul
	drawLine(0,-10,0,0,10,0,5);
	color(255,0,0); // x vermelho
	drawLine(-10,0,0,10,0,0,5);
}