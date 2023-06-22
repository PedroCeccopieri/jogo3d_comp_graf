#include <GL/glut.h>
#include <math.h>

#include "Entity.h"
#include "utils.h"

class Character:public Entity {
    private:
        
        int points = 0;
        
        int animation = 0, da = 5;
        int interpx = INTERX, interpy = INTERY;

        bool run = false, shooting = false, jumped = false;
        int state = 0;

        float jumpHeight = 0;

        float floor = FLOOR;

        float dposx, dposy, dposz;

        float neckx = 0, necky = 0, neckz = 0;
        float headx = 0, heady = 0, headz = 0;

        float rarmx = 0, rarmy = 0, rarmz = 0;
        float larmx = 0, larmy = 0, larmz = 0;

        float rforearmx = 0, rforearmy = 0, rforearmz = 0;
        float lforearmx = 0, lforearmy = 0, lforearmz = 0;

        float legsx = 0, legsy = 0, legsz = 0;
        float kneesx = 0, kneesy = 0, kneesz = 0;

    public:

        Character(float x, float y, float z) : Entity(x,y,z) {
            scale = 0.25;
            axisx = 0, axisy = 90, axisz = 0;
            wHitbox = 4.5, hHitbox = 14.4, dHitbox = 6;
        }

        void addPoint(int p) {
            points += p;
        }
        
        int getPoints() {
            return points;
        }

        void resetInterpx() {
            interpx = 0;
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

        void movePosX(int dx) {

            dposx = dx;
        
            if (dx > 0) axisy = 90;
            if (dx < 0) axisy = 270;
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
            if (interpx < INTERX) {
                interpx++;
            }

            if (interpy < INTERY) {
                interpy++;
            }

            if (interpx < INTERX && !run && !shooting) setState(1);
            else if (interpx < INTERX && run && !shooting) setState(2);
            else if (interpx == INTERX && shooting) setState(3);
            else if (interpx < INTERX && !run && shooting) setState(4);
            else if (interpx < INTERX && run && shooting) setState(5);
            else setState(0);
        }

        void showHitbox() {

            glPushMatrix();

            glTranslatef(posx,posy + jumpHeight,posz);
            glScalef(scale,scale,scale);

            glRotatef(axisz,0,0,1); // rotação corpo z
            glRotatef(axisy,0,1,0); // rotação corpo y
            // glRotatef(axisx,1,0,0); // rotação corpo x

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

                hitbox[c] = (((0.5*i * wHitbox)*cos(axisy*M_PI/180) + (0.5*k * dHitbox)*sin(axisy*M_PI/180))*cos(axisz*M_PI/180) - (0.5*j * hHitbox)*sin(axisz*M_PI/180)) * scale + posx;
	            hitbox[c+1] = (((0.5*i * wHitbox)*cos(axisy*M_PI/180) + (0.5*k * dHitbox)*sin(axisy*M_PI/180))*sin(axisz*M_PI/180) + (0.5*j * hHitbox)*cos(axisz*M_PI/180)) * scale + posy + jumpHeight;
	            hitbox[c+2] = ((-(0.5*i * wHitbox)*sin(axisy*M_PI/180) + (0.5*k * dHitbox)*cos(axisy*M_PI/180))) * scale + posz;
            }
        }

        bool checkcolision(double hb[12]) {
	
            return !(hb[0] > hitbox[9] || hb[9] < hitbox[0] || hb[1] < hitbox[4] || hb[4] > hitbox[1]);

        }

        void updatePos() {
            if (interpx < INTERX) {
                if (state == 1 || state == 4) posx += dposx/(2*INTERX);
                if (state == 2 || state == 5) posx += dposx/INTERX;
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
            
            glRotatef(axisz,0,0,1); // rotação corpo z
            glRotatef(axisy,0,1,0); // rotação corpo y
            // glRotatef(axisx,1,0,0); // rotação corpo x

            glPushMatrix(); // corpo {
            color(0,100,140);
            glScalef(2,4,2);
            drawCube(true);
            glPopMatrix(); // }

            glPushMatrix(); // {

            glTranslatef(0,2,0);
            glRotatef(neckx,1,0,0); // rotação pescoço x 
            glRotatef(necky,0,1,0); // rotação pescoço y
            glRotatef(neckz,0,0,1); // rotação pescoço z

            glPushMatrix(); // pescoço {
            color(255,190,150);
            glTranslatef(0,0.5,0);
            glScalef(1,1,1);
            drawCube(true);
            glPopMatrix(); // }

            glPushMatrix(); // cabeça {
            color(255,190,150);
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
            
            glPushMatrix(); // braço direito {
            color(0,100,140);
            glTranslatef(0.5,-1,0);
            glScalef(1,2,2);
            drawCube(true);
            glPopMatrix(); // }

            glPushMatrix(); // anti-braço direito {
            //color(255,190,150);
            color(0,0,255);
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
            
            glPushMatrix(); // braço esquerdo {
            color(0,100,140);
            glTranslatef(-0.5,-1,0);
            glScalef(1,2,2);
            drawCube(true);
            glPopMatrix(); // }

            glPushMatrix(); // anti-braço esquerdo {
            color(255,190,150);
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
            
            glPushMatrix(); // perna direita {
            color(255,0,0);
            glTranslatef(0,-1.25,0);
            glScalef(1,2.5,2);
            drawCube(true);
            glPopMatrix(); // }

            glPushMatrix(); // joelho direito {
            color(255,190,150);
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
            
            glPushMatrix(); // perna esquerda {
            color(255,0,0);
            glTranslatef(0,-1.25,0);
            glScalef(1,2.5,2);
            drawCube(true);
            glPopMatrix(); // }

            glPushMatrix(); // joelho esquerdo {
            // color(255,190,150);
            color(255,255,0);
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
