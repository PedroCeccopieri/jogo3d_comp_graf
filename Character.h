#include <GL/glut.h>
#include <math.h>

#include "Entity.h"
#include "utils.h"

class Character:public Entity {
    private:
        
        int points = 0;
        
        int animation = 0, da = 5;
        int interpxz = INTERXZ, interpy = INTERY;

        bool run = false, shooting = false, jumped = false;
        int state = 0;

        float jumpHeight = 0;

        float floor = FLOOR;

        float rotationAngle = 0;
        float dposxz, dposy;

        float neckx = 0, necky = 0, neckz = 0;
        float headx = 0, heady = 0, headz = 0;

        float rarmx = 0, rarmy = 0, rarmz = 0;
        float larmx = 0, larmy = 0, larmz = 0;

        float rforearmx = 0, rforearmy = 0, rforearmz = 0;
        float lforearmx = 0, lforearmy = 0, lforearmz = 0;

        float legsx = 0, legsy = 0, legsz = 0;
        float kneesx = 0, kneesy = 0, kneesz = 0;

    public:

        Character(float x, float y, float z, std::vector<unsigned int> tId) : Entity(x,y,z,tId) {
            scale = 1;
            axisx = 0, axisy = 0, axisz = 0;
            wHitbox = 6, hHitbox = 14.4, dHitbox = 6;
        }

        void addPoint(int p) {
            points += p;
        }
        
        int getPoints() {
            return points;
        }

        float getRotation() {
            return rotationAngle*M_PI/180;
        }

        void resetInterpx() {
            interpxz = 0;
        }

        void resetInterpy() {
            if (!jumped) interpy = 0;
        }

        void setRun(bool r) {
            run = r;
        }

        void setShoot(bool s) {
            shooting = s;
        }

        void movePosXZ(int dxz) {

            dposxz = dxz;
        
            if (dxz > 0) axisy = 90;
            if (dxz < 0) axisy = 270;
        }

        void rotate(int r) {
            if (rotationAngle + r < 0) rotationAngle = 360 - r;
            else rotationAngle += r;
        }

        void movePosY(int dy) {
            dposy = dy;
        }

        void setState(int s) {
            if (s != state) {
                reset();
                state = s;
            }
        }
        
        void reset() {
            animation = 0, da = 5;

            neckx = 0, necky = 0, neckz = 0;
            headx = 0, heady = 0, headz = 0;

            rarmx = 0, rarmy = 0, rarmz = 0;
            larmx = 0, larmy = 0, larmz = 0;

            rforearmx = 0, rforearmy = 0, rforearmz = 0;
            lforearmx = 0, lforearmy = 0, lforearmz = 0;

            legsx = 0, legsy = 0, legsz = 0;
            kneesx = 0, kneesy = 0, kneesz = 0;
        }

        void setJumped() {
            jumped = true;
        }
        
        void resetJumped() {
            jumped = false;
        }
        
        void updateState() {
            if (interpxz < INTERXZ) {
                interpxz++;
            }

            if (interpy < INTERY) {
                interpy++;
            }

            if (interpxz < INTERXZ && !run && !shooting) setState(1);
            else if (interpxz < INTERXZ && run && !shooting) setState(2);
            else if (interpxz == INTERXZ && shooting) setState(3);
            else if (interpxz < INTERXZ && !run && shooting) setState(4);
            else if (interpxz < INTERXZ && run && shooting) setState(5);
            else setState(0);
        }

        void showHitbox() {

            glBindTexture(GL_TEXTURE_2D, texId[0]);

            glPushMatrix();

            glTranslatef(posx,posy + jumpHeight,posz);
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
                hitbox[c+1] = 0.5 * j * hHitbox * scale + posy + jumpHeight;
                hitbox[c+2] = 0.5 * k * dHitbox * scale + posz;
            }
        }

        bool checkcolision(double hb[24]) {
            
            return !(hitbox[0] >= hb[3] || hitbox[3] <= hb[0] || hitbox[1] >= hb[10] || hitbox[10] <= hb[1] || hitbox[14] >= hb[2] || hitbox[2] <= hb[14]);

        }

        void updatePos() {
            if (interpxz < INTERXZ) {

                // std::cout << "state " << state << std::endl;
                if (state == 1 || state == 4) {
                    posx += dposxz/(2*INTERXZ)*cos(rotationAngle*M_PI/180);
                    posz += -dposxz/(2*INTERXZ)*sin(rotationAngle*M_PI/180);
                }
                if (state == 2 || state == 5) {
                    posx += dposxz/(INTERXZ)*cos(rotationAngle*M_PI/180);
                    posz += -dposxz/(INTERXZ)*sin(rotationAngle*M_PI/180);
                }
            }

            if (interpy < INTERY) {
                posy += dposy/INTERY;
            }

            if (posy > floor) {
                if (posy - GRAV * interpy/5 < floor) {
                    posy = floor;
                } else posy -= GRAV * interpy/5;

                if (posy == floor) {
                    resetJumped();
                }
            }
        }

        void animate() {

            if (animation == 50) da = -5;
	        if (animation == -50) da = 5;
            animation += da;

            switch (state) {

                case 0: // stopped
                    jumpHeight = 0;
                    break;
                case 1: // walking
                    legsx += da;
                    rarmx += da;
                    larmx += da;
                    rforearmx = 15;
                    lforearmx = 15;
                    kneesx = 15;
                    jumpHeight += (da % 2) * 0.005;
                    break;
                case 2: // running
                    legsx += da * 1.8;
                    rarmx += da * 1.8;
                    larmx += da * 1.8;
                    rforearmx = 45;
                    lforearmx = 45;
                    kneesx = 45;
                    jumpHeight += (da % 2) * 0.01;
                    break;
                case 3: // stopped and shotting
                    rarmx = -90;
                    break;
                case 4: // walking and shotting
                    legsx += da;
                    rarmx = -90;
                    larmx += da;
                    lforearmx = 15;
                    kneesx = 15;
                    jumpHeight += (da % 2) * 0.005;
                    break;
                case 5: // running and shotting
                    legsx += da * 1.8;
                    rarmx = -90;
                    larmx += da * 1.8;
                    lforearmx = 45;
                    kneesx = 45;
                    jumpHeight += (da % 2) * 0.01;
                    break;
                case 6: // jumping
                    legsx = (animation + 50)/2 * 80.0/50;
                    rarmx = -80 - animation;
                    rforearmx = 45;
                    larmx -= da;
                    lforearmx = 45;
                    kneesx = 45;
                    // posy = 2 * (animation + 50)/2 * 3.0/50;
                    // posy += (da % 2) * std::max(0,animation/10);
                    break;
            }

            updateHitbox();
        }

        void draw() {

            glPushMatrix(); // personagem {
    
            glTranslatef(posx,posy + jumpHeight,posz);
            glScalef(scale,scale,scale);

            glRotatef(rotationAngle,0,1,0);
            glRotatef(axisy,0,1,0); // rotação corpo y

            glBindTexture(GL_TEXTURE_2D, texId[1]);
            
            glPushMatrix(); // corpo {
            glScalef(2,4,2);
            drawCube(true);
            glPopMatrix(); // }

            glPushMatrix(); // {

            glTranslatef(0,2,0);
            glRotatef(neckx,1,0,0); // rotação pescoço x 
            glRotatef(necky,0,1,0); // rotação pescoço y
            glRotatef(neckz,0,0,1); // rotação pescoço z

            glBindTexture(GL_TEXTURE_2D, texId[2]);

            glPushMatrix(); // pescoço {
            glTranslatef(0,0.5,0);
            glScalef(1,1,1);
            drawCube(true);
            glPopMatrix(); // }

            glPushMatrix(); // cabeça {
            glTranslatef(0,1,0);
            glRotatef(headx,1,0,0);
            glRotatef(heady,0,1,0);
            glRotatef(headz,0,0,1);
            glTranslatef(0,2,0);
            glScalef(4,4,4);
            drawCube(true);
            glPopMatrix(); // }

            glPopMatrix(); // }

            glPushMatrix(); // {
            
            glTranslatef(1,2,0);
            glRotatef(rarmx,1,0,0); // rotação braço direito x
            glRotatef(rarmy,0,1,0); // rotação braço direito y
            glRotatef(rarmz,0,0,1); // rotação braço direito z

            glBindTexture(GL_TEXTURE_2D, texId[1]);
            
            glPushMatrix(); // braço direito {
            glTranslatef(0.5,-1,0);
            glScalef(1,2,2);
            drawCube(true);
            glPopMatrix(); // }

            glBindTexture(GL_TEXTURE_2D, texId[2]);

            glPushMatrix(); // anti-braço direito {
            glTranslatef(0.5,-2,0);
            glRotatef(-rforearmx,1,0,0);
            glRotatef(-rforearmy,0,1,0);
            glRotatef(-rforearmz,0,0,1);
            glTranslatef(0,-1,0);
            glScalef(1,2,2);
            drawCube(true);
            glPopMatrix(); // }

            glPopMatrix(); // }

            glPushMatrix(); // {
            
            glTranslatef(-1,2,0);
            glRotatef(-larmx,1,0,0); // rotação braço esquerdo x
            glRotatef(-larmy,0,1,0); // rotação braço esquerdo y
            glRotatef(-larmz,0,0,1); // rotação braço esquerdo z

            glBindTexture(GL_TEXTURE_2D, texId[1]);
            
            glPushMatrix(); // braço esquerdo {
            glTranslatef(-0.5,-1,0);
            glScalef(1,2,2);
            drawCube(true);
            glPopMatrix(); // }

            glBindTexture(GL_TEXTURE_2D, texId[2]);

            glPushMatrix(); // anti-braço esquerdo {
            glTranslatef(-0.5,-2,0);
            glRotatef(-lforearmx,1,0,0);
            glRotatef(-lforearmy,0,1,0);
            glRotatef(-lforearmz,0,0,1);
            glTranslatef(0,-1,0);
            glScalef(1,2,2);
            drawCube(true);
            glPopMatrix(); // }

            glPopMatrix(); // }

            glPushMatrix(); // {
            
            glTranslatef(0.5,-2,0);
            glRotatef(-legsx,1,0,0); // rotação perna direita x
            glRotatef(-legsy,0,1,0); // rotação perna direita y
            glRotatef(-legsz,0,0,1); // rotação perna direita z

            glBindTexture(GL_TEXTURE_2D, texId[3]);
            
            glPushMatrix(); // perna direita {
            glTranslatef(0,-1.25,0);
            glScalef(1,2.5,2);
            drawCube(true);
            glPopMatrix(); // }

            glBindTexture(GL_TEXTURE_2D, texId[2]);

            glPushMatrix(); // joelho direito {
            glTranslatef(0,-2.5,0);
            glRotatef(kneesx,1,0,0);
            glRotatef(kneesy,0,1,0);
            glRotatef(kneesz,0,0,1);
            glTranslatef(0,-1,0);
            glScalef(1,2,2);
            drawCube(true);
            glPopMatrix(); // }

            glPopMatrix(); // }

            glPushMatrix(); // {
            
            glTranslatef(-0.5,-2,0);
            glRotatef(legsx,1,0,0); // rotação perna esquerda x
            glRotatef(legsy,0,1,0); // rotação perna esquerda y
            glRotatef(legsz,0,0,1); // rotação perna esquerda z

            glBindTexture(GL_TEXTURE_2D, texId[3]);
            
            glPushMatrix(); // perna esquerda {
            glTranslatef(0,-1.25,0);
            glScalef(1,2.5,2);
            drawCube(true);
            glPopMatrix(); // }

            glBindTexture(GL_TEXTURE_2D, texId[2]);

            glPushMatrix(); // joelho esquerdo {
            glTranslatef(0,-2.5,0);
            glRotatef(kneesx,1,0,0);
            glRotatef(kneesy,0,1,0);
            glRotatef(kneesz,0,0,1);
            glTranslatef(0,-1,0);
            glScalef(1,2,2);
            drawCube(true);
            glPopMatrix(); // }

            glPopMatrix(); // }

            glPopMatrix(); // }
        }
};
