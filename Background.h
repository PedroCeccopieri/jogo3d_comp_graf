#include <GL/glut.h>
#include <math.h>

#include "utils.h"

class Background {
    private:
        int width, height;

        int qnt;

    public:

    Background(int w, int h, int q) {
        width = w;
        height = h;
        qnt = q;
    }

    void draw() {
        for (int i = 0; i < qnt; i++) {

            glBegin(GL_QUADS);
            color(0,250,250);
            glVertex3f(width * (2*i - 1),height,0);
            glVertex3f(width * (2*i - 1),-height,0);
            glVertex3f(width * (2*i + 1),-height,0);
            glVertex3f(width * (2*i + 1),height,0);
            glEnd();

            color(0,255,0);
            glBegin(GL_POLYGON);
            glVertex3f(width * (2*i + 1),-height,0.1);
            glVertex3f(width * (2*i + 1),4,0.1);
            glVertex3f(width * (2*i + 1 - 45/100.0) ,0,0.1);
            glVertex3f(width * (2*i + 1 - 70/100.0) ,2,0.1);
            glVertex3f(width * (2*i + 1 - 120/100.0) ,-2,0.1);
            glVertex3f(width * (2*i + 1 - 130/100.0) ,7,0.1); // *
            glVertex3f(width * (2*i + 1 - 180/100.0) ,-3,0.1);
            glVertex3f(width * (2*i - 1) ,4,0.1); // *
            glVertex3f(width * (2*i - 1),-height,0.1);
            glEnd();

            glPushMatrix();

            glTranslatef(width*(qnt-1),0,0);
            
            glPushMatrix();
            glColor3f(0.5,0.5,0.5);
            glTranslatef(0,-height+4,0.2);
            glScalef(2,2,2);
            glScalef(width*qnt-0.15,2,1);
            drawSquare(true);
            glPopMatrix();

            glPushMatrix();
            glColor3f(1,1,1);
            glTranslatef(0,-height+4.5,0.3);
            glScalef(2,2,2);
            glScalef(width*qnt-0.15,0.1,1);
            drawSquare(true);
            glPopMatrix();

            glPopMatrix();

        }
        
    }
};