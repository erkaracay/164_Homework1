/*********
   CTIS164 - Template Source Program
----------
STUDENT : Suphi Erkin Karaçay - 22003597
SECTION : 02
HOMEWORK: #1
----------
PROBLEMS: 
o Shapes can spawn at the same location (collide)
----------
ADDITIONAL FEATURES:
o Can move diagonally on the press of F2
o Can change color with right click
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT  800

#define TIMER_PERIOD    16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R      0.0174532

// Modes
#define STOP             0
#define ACTION           1
#define ACTION2          2

// Shape Frames
#define COLOUR1          1
#define COLOUR2          2
#define COLOUR3          3
#define COLOUR4          4
#define COLOUR5          5
#define COLOUR6          6

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int mx, my;               // Current position of mouse pointer
int xShape, yShape;       // Assigned shape locations
int mode = STOP;          // ACTION: Start Moving, STOP: stop
int cMode = COLOUR1;      // Colour Modes
bool activeTimer = false, // Start timer to move shape
activeTimer2 = false;     // Horizontal timer
int sCnt = 0;             // Shape count
int r = 0;

typedef struct {
    int count, 
        sx, sy;           // Shape Coordinates
}shape_t;

shape_t shapes[10];

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void displayBackground() {
    // Sky
    glColor3ub(0, 0, 93);
    glBegin(GL_QUADS);
    glVertex2f(-500, 550);
    glVertex2f(500, 550);
    glColor3ub(51, 0, 51);
    glVertex2f(500, -150);
    glVertex2f(-500, -150);
    glEnd();

    // Road
    glColor3ub(64, 64, 64);
    glRectf(-500, -150, 500, -400);

    // Road Lines
    glColor3f(1, 1, 1);
    glLineWidth(8);
    glBegin(GL_LINES);
    glVertex2f(-500, -160);
    glVertex2f(500, -160);
    glVertex2f(-500, -390);
    glVertex2f(500, -390);
    glColor3ub(150, 150, 0);
    glVertex2f(-500, -270);
    glVertex2f(500, -270);
    glVertex2f(-500, -285);
    glVertex2f(500, -285);
    glEnd();
    
    // Stars
    glColor3f(0.7, 0.7, 0.7);
    vprint(-391, 230, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-407, 180, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-319, 270, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-232, 240, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-110, 265, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-60,  195, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(50,   224, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(130,  255, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(195,  200, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(286,  239, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(386,  218, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-163, 195, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-454, 241, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(303,  189, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-461, 322, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-399, 365, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-399, 317, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-333, 352, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-256, 317, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-187, 342, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-1,   292, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(46,   343, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(138,  364, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(224,  315, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(-59,  341, GLUT_BITMAP_TIMES_ROMAN_24, "*");
    vprint(283,  360, GLUT_BITMAP_TIMES_ROMAN_24, "*");

    // Apartment
    glColor3ub(39, 39, 39);
    glRectf(-420, 200, -220, -150);

    // Apt. Doors
    glColor3ub(102, 51, 0);
    glRectf(-345, -82, -295, -150);

    // Apt. Lines
    glLineWidth(3);
    glColor3ub(180, 180, 0);
    glBegin(GL_LINES);
    glVertex2f(-370, 200);
    glVertex2f(-370, -150);
    glVertex2f(-320, 200);
    glVertex2f(-320, -80);
    glVertex2f(-270, 200);
    glVertex2f(-270, -150);
    glVertex2f(-420, 130);
    glVertex2f(-220, 130);
    glVertex2f(-420, 60);
    glVertex2f(-220, 60);
    glVertex2f(-420, -10);
    glVertex2f(-220, -10);
    glVertex2f(-420, -80);
    glVertex2f(-220, -80);
    glEnd();

    // Apt. Windows
    glColor4ub(5, 5, 5, 180);
    glRectf(-360, 140, -330, 190);
    glColor4f(.9, .9, .9, 0.9);
    glRectf(-360, 70, -330, 120);
    glColor4ub(5, 5, 5, 180);
    glRectf(-360, 0, -330, 50);
    glRectf(-360, -70, -330, -20);
    glColor4ub(5, 5, 5, 180);
    glRectf(-310, 140, -280, 190);
    glRectf(-310, 70, -280, 120);
    glColor4f(.9, .9, .9, 0.9);
    glRectf(-310, 0, -280, 50);
    glColor4ub(5, 5, 5, 180);
    glRectf(-310, -70, -280, -20);
    glRectf(-410, 70, -380, 120); 
    glColor4ub(5, 5, 5, 180);
    glRectf(-410, 0, -380, 50);
    glColor4f(.9, .9, .9, 0.9);
    glRectf(-410, -70, -380, -20);
    glColor4ub(5, 5, 5, 180);
    glRectf(-260, 70, -230, 120);
    glColor4f(.9, .9, .9, 0.9);
    glRectf(-260, 0, -230, 50);
    glRectf(-260, -70, -230, -20);

    // Line that splits door into two
    glColor3f(.15, .15, .15);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-320, -82);
    glVertex2f(-320, -150);
    glEnd();

    // ID BOX
    glColor3ub(20, 20, 20);
    glRectf(285, 400, 500, 300);
    glColor3ub(51, 102, 0);
    glRectf(295, 400, 500, 310);
    glColor3ub(20, 20, 20);
    vprint(312, 370, GLUT_BITMAP_9_BY_15, "Suphi Erkin Karacay");
    vprint(360, 350, GLUT_BITMAP_9_BY_15, "22003597");
}

void displayCar() {
    for (int q = 1; q <= 10; q++) {
        xShape = (shapes + q - 1)->sx;
        yShape = (shapes + q - 1)->sy;

        if (!(xShape >= shapes[q].sx && xShape <= shapes[q].sx &&
            yShape >= shapes[q].sy && yShape <= shapes[q].sy)) {
            // BODY
            switch (cMode) {
            case COLOUR1: glColor3ub(153, 0, 0);
                break;
            case COLOUR2: glColor3ub(25, 25, 25);
                break;
            case COLOUR3: glColor3ub(40, 5, 105);
                break;
            case COLOUR4: glColor3ub(230, 230, 230);
                break;
            case COLOUR5: glColor3ub(0, 0, 0);
                break;
            case COLOUR6: glColor4ub(0, 0, 0, 0);
            }
            glBegin(GL_POLYGON);
            glVertex2f(xShape - 75, yShape - 15);
            glVertex2f(xShape - 75, yShape + 15);
            glVertex2f(xShape - 25, yShape + 50);
            glVertex2f(xShape + 20, yShape + 50);
            glVertex2f(xShape + 60, yShape + 15);
            glVertex2f(xShape + 85, yShape + 15);
            glVertex2f(xShape + 85, yShape - 15);
            glEnd();

            // EXHAUST
            glColor3ub(15, 15, 15);
            glRectf(xShape - 75, yShape - 10, xShape - 90, yShape - 3);

            // BODY OUTLINE
            glLineWidth(1);
            switch (cMode) {
            case COLOUR1: glColor3ub(255, 255, 0);
                break;
            case COLOUR2: glColor3ub(0, 153, 153);
                break;
            case COLOUR3: glColor3ub(204, 0, 204);
                break;
            case COLOUR4: glColor3ub(0, 0, 0);
                break;
            case COLOUR5: glColor3ub(0, 255, 0);
                break;
            case COLOUR6: glColor3ub(0, 0, 0);
            }
            glBegin(GL_LINE_LOOP);
            glVertex2f(xShape - 75, yShape - 15);
            glVertex2f(xShape - 75, yShape + 15);
            glVertex2f(xShape - 25, yShape + 50);
            glVertex2f(xShape + 20, yShape + 50);
            glVertex2f(xShape + 60, yShape + 15);
            glVertex2f(xShape + 85, yShape + 15);
            glVertex2f(xShape + 85, yShape - 15);
            glEnd();

            // FRONT WINDOW
            glColor3ub(0, 0, 0);
            glBegin(GL_POLYGON);
            glVertex2f(xShape + 50, yShape + 15);
            glVertex2f(xShape + 5, yShape + 15);
            glVertex2f(xShape + 5, yShape + 40);
            glVertex2f(xShape + 18, yShape + 40);
            glEnd();

            // REAR WINDOW
            glBegin(GL_POLYGON);
            glVertex2f(xShape - 63, yShape + 15);
            glVertex2f(xShape - 5, yShape + 15);
            glVertex2f(xShape - 5, yShape + 40);
            glVertex2f(xShape - 23, yShape + 40);
            glEnd();

            // FRONT WINDOW OTL.
            switch (cMode) {
            case COLOUR1: glColor3ub(255, 255, 0);
                break;
            case COLOUR2: glColor3ub(0, 153, 153);
                break;
            case COLOUR3: glColor3ub(204, 0, 204);
                break;
            case COLOUR4: glColor3ub(0, 0, 0);
                break;
            case COLOUR5: glColor3ub(0, 255, 0);
                break;
            case COLOUR6: glColor3ub(0, 0, 0);
            }
            glBegin(GL_LINE_LOOP);
            glVertex2f(xShape + 50, yShape + 15);
            glVertex2f(xShape + 5, yShape + 15);
            glVertex2f(xShape + 5, yShape + 40);
            glVertex2f(xShape + 18, yShape + 40);
            glEnd();

            // REAR WINDOW OTL.
            glBegin(GL_LINE_LOOP);
            glVertex2f(xShape - 63, yShape + 15);
            glVertex2f(xShape - 5, yShape + 15);
            glVertex2f(xShape - 5, yShape + 40);
            glVertex2f(xShape - 25, yShape + 40);
            glEnd();

            // WHEELS
            glColor3f(0, 0, 0);
            circle(xShape - 45, yShape - 22, 20);
            circle(xShape + 55, yShape - 22, 20);

            // NICE RIMS
            glColor3f(1, 1, 1);
            glBegin(GL_LINES);
            glVertex2f(xShape - 60, yShape - 22);
            glVertex2f(xShape - 30, yShape - 22);
            glVertex2f(xShape - 45, yShape - 7);
            glVertex2f(xShape - 45, yShape - 37);
            glVertex2f(xShape - 55, yShape - 13);
            glVertex2f(xShape - 35, yShape - 32);
            glVertex2f(xShape - 35, yShape - 13);
            glVertex2f(xShape - 55, yShape - 32);
            glVertex2f(xShape + 70, yShape - 22);
            glVertex2f(xShape + 40, yShape - 22);
            glVertex2f(xShape + 55, yShape - 7);
            glVertex2f(xShape + 55, yShape - 37);
            glVertex2f(xShape + 65, yShape - 13);
            glVertex2f(xShape + 45, yShape - 32);
            glVertex2f(xShape + 45, yShape - 13);
            glVertex2f(xShape + 65, yShape - 32);
            glEnd();

            // FRONT LIGHTS
            glColor3ub(255, 255, 255);
            glRectf(xShape + 85, yShape + 8, xShape + 75, yShape + 3);

            // REAR LIGHTS
            glColor3ub(15, 15, 15);
            glRectf(xShape - 75, yShape + 8, xShape - 65, yShape + 3);

            // Shape Count
            switch (cMode) {
            case 2:
            case 3:
            case 5:
            case 6: glColor3f(1, 1, 1);
                break;
            default: glColor3ub(15, 15, 15);
            }
            vprint(xShape - 5, yShape - 5, GLUT_BITMAP_TIMES_ROMAN_24, "%d", q);
        }
    }
}

void menu() {
    glColor3ub(0, 170, 0);
    vprint(250, 100, GLUT_BITMAP_9_BY_15, "F1: Move Horizontally");
    vprint(250, 85, GLUT_BITMAP_9_BY_15, "F2: Move Diagonally");
    vprint(250, 70, GLUT_BITMAP_9_BY_15, "Right Click: Change Color");
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    displayBackground();
    menu();
    displayCar();

    for (int y = 0; y < 10; y++) {
        if (shapes[y].sx > 590)
            shapes[y].sx = -585;

        if (shapes[y].sy > 445)
            shapes[y].sy = -450;
    }

    glColor3ub(0, 0, 102);
    vprint(380, -380, GLUT_BITMAP_9_BY_15, "[%-4d : %-4d]", mx, my);

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    if (key == GLUT_KEY_F1) {
        if (mode == STOP) {
            mode = ACTION;
            activeTimer = true;
        }
        else {
            mode = STOP;
            activeTimer = false;
        }
    }

    if (key == GLUT_KEY_F2) {
        if (mode == STOP) {
            mode = ACTION2;
            activeTimer2 = true;
        }
        else {
            mode = STOP;
            activeTimer2 = false;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//blob:file:///dea86568-732f-4541-9133-15adb7645756
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    xShape = x - winWidth /  2;
    yShape = winHeight / 2 - y;

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && sCnt < 10) {
        (*(shapes + r)).count = sCnt;
        (*(shapes + r)).sx = xShape;
        (*(shapes + r)).sy = yShape;

        sCnt++;
        r++;
    }

    if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) {
        if (cMode == COLOUR6)
            cMode = COLOUR1;
        else
            cMode++;
   }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    mx = x - winWidth /  2;
    my = winHeight / 2 - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (activeTimer)
        for (int q = 0; q < 10; q++)
            shapes[q].sx += 9;

    if (activeTimer2)
        for (int t = 0; t < 10; t++) {
            shapes[t].sx += 9;
            shapes[t].sy += 9;
        }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("A Starry Night");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}