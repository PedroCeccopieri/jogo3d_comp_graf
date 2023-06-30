#include <GL/glut.h>

#include "utils.h"
#include "Entity.h"

class Coin: public Entity {
    private:
    
        int coinType;

    public:

        Coin(float x, float y, float z, int ct, std::vector<unsigned int> tId): Entity(x,y,z,tId) {
            
            coinType = ct;
            scale = 1.5;
            wHitbox = 1.75, hHitbox = 1.75, dHitbox = 1.75;
        }

        int getPoint() {
            switch (coinType) {
            case 0:
                return 1;
            case 1:
                return 3;
            case 2:
                return 5;
            case 3:
                return 10;
            }
            return -1;
        }

        void animate() {

            axisx++; axisz++;

            updateHitbox();
        }

        void draw() {

            glPushMatrix();

            glTranslatef(posx,posy,posz);
            glScalef(scale,scale,scale);

            glRotatef(axisx,1,0,0); // rotação corpo x
            glRotatef(axisy,0,1,0); // rotação corpo y
            glRotatef(axisz,0,0,1); // rotação corpo z

            glScalef(wHitbox,hHitbox,dHitbox);

            glBindTexture(GL_TEXTURE_2D, texId[coinType+1]);
            drawCube(true);

            glPopMatrix();
        }
};
