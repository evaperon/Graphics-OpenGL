
#ifndef linux
#include <windows.h>
#endif

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// === Types ===

typedef GLfloat point3[3];

enum options {
    EXIT, FIRST_MODE, SECOND_MODE//, CUBE, ICOSAHEDRON
};

// === Data ===

int w, h;
int seed;

int mode = FIRST_MODE;

//int shape = CUBE;

float param_a = 8;
float param_b = 90;
point3 param_v = {1, 2, 6};

float angle_speed = 90;
float scale_speed = 2;

float t; // contains the current time in seconds

// === Declarations ===

void init();

void display();

void idle();

void set_projection();

void set_random_color();

void create_component();

void draw_component(float rotate_x, float rotate_y, float size);

void draw_shape();

void draw_cube();

void create_menu();

void create_seed();

void reshape(int new_w, int new_h);

void menu(int id);

// === Implementation ===

void init() {

    glEnable(GL_BLEND);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);

    set_projection();
}

void set_projection() {

    int s = 50;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-s, s, -s, s, -200, 200);

    glMatrixMode(GL_MODELVIEW);
}

void create_component() {

    glNewList(1, GL_COMPILE);
    {
        glBegin(GL_POLYGON);
        glVertex3f(-1, 1, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(1, -1, 1);
        glVertex3f(-1, -1, 1);
        glEnd();
    }
    glEndList();
}

void draw_component(float rotate_x, float rotate_y, float size) {
    glPushMatrix();

    float scale = size / 2; // 2 is the size of the component
    glScalef(scale, scale, scale);
    glRotatef(rotate_x, 1, 0, 0);
    glRotatef(rotate_y, 0, 1, 0);

    set_random_color();
    glCallList(1);

    glPopMatrix();
}

void draw_shape() {

    draw_cube();
}

void draw_cube() {
    glPushMatrix();

    draw_component(0, 0, param_a);
    draw_component(0, 90, param_a);
    draw_component(0, 180, param_a);
    draw_component(0, 270, param_a);
    draw_component(90, 0, param_a);
    draw_component(-90, 0, param_a);

    glPopMatrix();
}

void set_random_color() {

    glColor3ub(rand() % 255, rand() % 255, rand() % 255);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    srand(seed);

    glPushMatrix();

    float angle = t * angle_speed; // in degrees
    float scale = 1 + (1 + sin(t * scale_speed) * 1.0f); // from 1 to 3

    if (mode==FIRST_MODE) {

        glTranslatef(0, 0, -param_b);
        glRotatef(angle, param_v[0], param_v[1], param_v[2]);
        glScalef(scale, scale, scale);

        draw_shape();
    }
    else if(mode==SECOND_MODE){
        glTranslated(0,0,-8*param_b/10);
        glRotated(angle,param_v[0],param_v[1],param_v[2]);
        glTranslated(0,0,8*param_b/10);

        glRotatef(angle, param_v[0], param_v[1], param_v[2]);
        glScalef(scale, scale, scale);

        draw_shape();
    }

    glPopMatrix();

    glutSwapBuffers();
}

void idle() {

    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    glutPostRedisplay();
}

void reshape(int new_w, int new_h) {

    w = new_w;
    h = new_h;

    set_projection();
    glutPostRedisplay();
}

void menu(int id) {

    switch(id){
        case EXIT:
            exit(0);
            break;
        case FIRST_MODE:
            mode=FIRST_MODE;
            break;
        case SECOND_MODE:
            mode=SECOND_MODE;
            break;
        default:
            return;
    }

    glutPostRedisplay();
}

void create_menu() {

    glutCreateMenu(menu);

    glutAddMenuEntry("First mode", FIRST_MODE);
    glutAddMenuEntry("Second mode", SECOND_MODE);
    glutAddMenuEntry("Exit", EXIT);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void create_seed() {

    //srand(time(0)); // risky random seed
    srand(3); // nice fixed random seed
    seed = rand();
}

void main(int argc, char **argv) {

    w = 500;
    h = 500;

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Cube");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    create_seed();
    create_menu();
    create_component();

    init();
    glutMainLoop();
}