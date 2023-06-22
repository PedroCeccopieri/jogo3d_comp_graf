#ifndef ENTITY
#define ENTITY

#include <GL/glut.h>
#include <iostream>

#include "utils.h"


class Entity {

    protected:

        float posx, posy, posz;

        float axisx = 0, axisy = 0, axisz = 0;

        float scale;
        float wHitbox, hHitbox, dHitbox;

        double hitbox[12] = {0,0,0, 0,0,0, 0,0,0, 0,0,0};

    public:

        Entity(float x, float y, float z) {
            posx = x;
            posy = y;
            posz = z;
        }

        float getPosx() {
            return posx;
        }

        float getPosy() {
            return posy;
        }

        double* getHitbox() {
            return hitbox;
        }

        void showHitbox() {

            glPushMatrix();

            glTranslatef(posx,posy,posz);
            glScalef(scale,scale,scale);

            // glRotatef(axisx,1,0,0); // rotação corpo x
            // glRotatef(axisy,0,1,0); // rotação corpo y
            // glRotatef(axisz,0,0,1); // rotação corpo z

            glScalef(wHitbox,hHitbox,dHitbox);
            color(255,255,255);
            drawCube(false);

            glPopMatrix();

            glPointSize(10);
            glBegin(GL_POINTS);
            color(255,255,0);
            glVertex3f(hitbox[0],hitbox[1],hitbox[2]);
            color(255,0,0);
            glVertex3f(hitbox[3],hitbox[4],hitbox[5]);
            color(0,255,0);
            glVertex3f(hitbox[6],hitbox[7],hitbox[8]);
            color(0,0,255);
            glVertex3f(hitbox[9],hitbox[10],hitbox[11]);
            glEnd();
        }

        void updateHitbox() {
            int i, j, k;
            int *a = NULL;

            if (0 <= (int)axisy % 360 && (int)axisy % 360 < 90) {
                int b[4] = {3,0,1,2};
                a = b;
            } else if (90 <= (int)axisy % 360 && (int)axisy % 360 < 180) {
                int b[4] = {7,4,0,3};
                a = b;
            } else if (180 <= (int)axisy % 360 && (int)axisy % 360 < 270) {
                int b[4] = {6,5,4,7};
                a = b;
            } else if (270 <= (int)axisy % 360 && (int)axisy % 360 < 360) {
                int b[4] = {2,1,5,6};
                a = b;
            }

            int ord[24] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1, -1,-1,-1, 1,-1,-1, 1,1,-1, -1,1,-1};

            for (int c = 0;  c < 12; c += 3) {

                int d = a[(c+1)/3]*3;
                
                i = ord[d]; j = ord[d+1]; k = ord[d+2];

                hitbox[c] = 0.5 * i * wHitbox * scale + posx;
                hitbox[c+1] = 0.5 * j * hHitbox * scale + posy;
                hitbox[c+2] = 0.5 * k * dHitbox * scale + posz;
            }
        }

};

#endif