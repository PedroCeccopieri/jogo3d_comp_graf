#include <GL/glut.h>

#include "utils.h"
#include "Entity.h"

class Hole: public Entity {
    private:

    public:

        Hole(float x, float y, float z): Entity(x,y,z) {
            
            scale = 4;
            wHitbox = 0.3, hHitbox = 1;
        }

        void animate() {

            updateHitbox();
        }

        void draw() {

            glPushMatrix();

            glTranslatef(posx,posy-4,posz);
            glScalef(scale,scale,scale);

            glScalef(wHitbox,hHitbox,dHitbox);

            color(0,0,0);
            drawSquare(true);

            glPopMatrix();
        }
};
