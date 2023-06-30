#include <GL/glut.h>

#include "utils.h"
#include "Entity.h"

class Bullet: public Entity {
    private:

        float velocityx;
        float velocityz;

    public:

        Bullet(float x, float y, float z, float vx, float vz, std::vector<unsigned int> tId) : Entity(x,y,z,tId) {
            
            scale = 2;
            wHitbox = 2.25, hHitbox = 2.25, dHitbox = 2.25;
            velocityx = vx, velocityz = vz;

        }

        void animate() {

            posx += velocityx;
            posz += velocityz;

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
            // glutSolidTeapot(1);

            glPopMatrix();
        }
};
