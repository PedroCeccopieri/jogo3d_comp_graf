#ifndef ENTITY
#define ENTITY

#include <GL/glut.h>
#include <iostream>
#include <vector>

#include "utils.h"

class Entity {

    protected:

        std::vector<unsigned int> texId;

        float posx, posy, posz;

        float axisx = 0, axisy = 0, axisz = 0;

        float scale;
        float wHitbox, hHitbox, dHitbox;

        double hitbox[24] = {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0};

    public:

        Entity(float x, float y, float z, std::vector<unsigned int> tId) {
            posx = x;
            posy = y;
            posz = z;
            texId = tId;
        }

        float getPosx() {
            return posx;
        }

        float getPosy() {
            return posy;
        }

        float getPosz() {
            return posz;
        }

        double* getHitbox() {
            return hitbox;
        }

        void showHitbox() {
            
            glBindTexture(GL_TEXTURE_2D, texId[0]);

            glPushMatrix();

            glTranslatef(posx,posy,posz);
            glScalef(scale,scale,scale);

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

            glPointSize(10);
            glBegin(GL_POINTS);
            color(255,255,0);
            glVertex3f(hitbox[12],hitbox[13],hitbox[14]);
            color(255,0,0);
            glVertex3f(hitbox[15],hitbox[16],hitbox[17]);
            color(0,255,0);
            glVertex3f(hitbox[18],hitbox[19],hitbox[20]);
            color(0,0,255);
            glVertex3f(hitbox[21],hitbox[22],hitbox[23]);
            glEnd();
        }

        void updateHitbox() {
            int i, j, k;

            int ord[24] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1, -1,-1,-1, 1,-1,-1, 1,1,-1, -1,1,-1};

            for (int c = 0;  c < 24; c += 3) {
                
                i = ord[c]; j = ord[c+1]; k = ord[c+2];

                hitbox[c] = 0.5 * i * wHitbox * scale + posx;
                hitbox[c+1] = 0.5 * j * hHitbox * scale + posy;
                hitbox[c+2] = 0.5 * k * dHitbox * scale + posz;
            }
        }

        void printHitbox() {
            for (int i = 0; i < 24; i += 3) std::cout << "(" << hitbox[i] << "," << hitbox[i+1] << "," << hitbox[i+2] << ")" << std::endl;        
        }

};

#endif