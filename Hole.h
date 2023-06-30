#include <GL/glut.h>

#include "utils.h"
#include "Entity.h"

class Hole: public Entity {
    private:

    public:

        Hole(float x, float y, float z, float w, float d, std::vector<unsigned int> tId): Entity(x,y,z,tId) {
            
            scale = 4;
            wHitbox = w, hHitbox = 1, dHitbox = d;
        }

        void animate() {

            updateHitbox();
        }

        void draw() {

            color(255,255,255);
            glBindTexture(GL_TEXTURE_2D, texId[1]);
            
            glPushMatrix();

            glTranslatef(posx,posy,posz);
            glScalef(scale,scale,scale);

            glScalef(wHitbox,hHitbox,dHitbox);

            drawCube(true);

            glPopMatrix();
        }
};
