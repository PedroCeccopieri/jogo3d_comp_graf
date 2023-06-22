#include <GL/glut.h>

#include "utils.h"
#include "Entity.h"

class Bullet: public Entity {
    private:

        float velocity = 0.1;

    public:

        Bullet(float x, float y, float z): Entity(x,y,z) {
            
            scale = 1;
            wHitbox = 1.25, hHitbox = 1.25, dHitbox = 1.25;
        }

        void animate() {

            posx -= velocity;

            updateHitbox();
        }

        void draw() {

            glPushMatrix();

            glTranslatef(posx,posy,posz);
            glScalef(scale,scale,scale);

            glScalef(wHitbox,hHitbox,dHitbox);

            color(50,50,50);
            drawCube(true);

            glPopMatrix();
        }
};
