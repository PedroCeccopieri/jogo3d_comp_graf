#include <GL/glut.h>
#include <math.h>

#include "utils.h"

class Skybox {
    private:

        std::vector<unsigned int> texId;

        int size;

    public:

    Skybox(int s, std::vector<unsigned int> tId) {
        size = s;
        texId = tId;
    }

    void draw() {

        color(255,255,255);
        glBindTexture(GL_TEXTURE_2D, texId[1]);

        glPushMatrix();
        glRotatef(-90,1,0,0);
        glScalef(size,size,1);
        drawCube(true);
        glPopMatrix();
        
        glBindTexture(GL_TEXTURE_2D, texId[2]);

        glPushMatrix();
        glScalef(size,size,1);
        glTranslatef(0,0,size/2);
        drawCube(true);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, texId[3]);

        glPushMatrix();
        glScalef(size,size,1);
        glTranslatef(0,0,-size/2);
        drawCube(true);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, texId[4]);

        glPushMatrix();
        glScalef(1,size,size);
        glTranslatef(-size/2,0,0);
        drawCube(true);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, texId[5]);

        glPushMatrix();
        glScalef(1,size,size);
        glTranslatef(size/2,0,0);
        drawCube(true);
        glPopMatrix();
        
    }
};