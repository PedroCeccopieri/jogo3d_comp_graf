#ifndef UTILS
#define UTILS

#define INTERX 5
#define INTERY 10
#define GRAV 0.2
#define FLOOR -8

void color(int r, int g, int b);
void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, int stroke);
void drawSquare(bool full);
void drawCube(bool full);
void drawText(int x, int y, char *string);
void grid(int x_min, int y_min, int x_max, int y_max);
void eixos();

#endif