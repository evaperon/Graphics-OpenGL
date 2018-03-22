
#ifndef linux
#include <windows.h>
#endif

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TAU 6.28318530718


typedef GLfloat point2[2];

enum options {
    EXIT, FIRST, SECOND, TRIANGLE, PENTAGON1, PENTAGON2, HEXAGON
};

int w, h;

int n = 5;
float r = 1.0f / 3.0f;
int loops = 10000;
char colorful = 0;
int seed;

float x_off = 0, y_off = 0;

void init();
void display();

void set_projection();
void set_random_color();

void create_polygon(int n, point2 vertices[]);
void create_random_point(int n, point2 vertices[], GLfloat point[]);
void create_menu();
void create_seed();

void mouse(int button, int state, int mouse_x, int mouse_y);
void motion(int mouse_x, int mouse_y);
void reshape(int new_w, int new_h);
void menu(int id);

// ===

void init() {

    glEnable(GL_BLEND);
    glClearColor(1.0, 1.0, 1.0, 0.0); /* white background */
    glColor3f(1.0, 0.0, 0.0); /* draw in red */

    set_projection();
}

void set_projection() {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0 + x_off, w + x_off, 0.0 + y_off, h + y_off);

    glMatrixMode(GL_MODELVIEW);
}

void create_polygon(int n, point2 vertices[]) {

    int size = (w < h ? w : h) / 4; // half the size of the screen

    for (int i = 0; i < n; i++) {

        vertices[i][0] = w / 2 + sin(i * TAU / n) * size;
        vertices[i][1] = h / 2 + cos(i * TAU / n) * size;
    }

}

void create_random_point(int n, point2 vertices[], GLfloat point[]) {
	int i1 = rand() % n;
	int i2 = rand() % n;
	float a = rand() % 1000 / 1000.0f;

	point[0] = vertices[i1][0] + (vertices[i2][0] - vertices[i1][0]) * a;
	point[1] = vertices[i1][1] + (vertices[i2][1] - vertices[i1][1]) * a;
}

void set_random_color() {

   glColor3ub(rand() % 255, rand() % 255, rand() % 255);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    srand(seed);

    point2 vertices[n];
    create_polygon(n, vertices);

    point2 p = {};
	create_random_point(n, vertices, p);

    set_random_color();
    glBegin(GL_POINTS);
    for (int i = 0; i < loops; i++) {

        int j = rand() % n;

        p[0] += (vertices[j][0] - p[0]) * (1 - r);
        p[1] += (vertices[j][1] - p[1]) * (1 - r);

        if (colorful) {
            set_random_color();
        }

        glVertex2fv(p);
    }
    glEnd();

    glutSwapBuffers();
}

int mouse_dragging = 0, mouse_last_x, mouse_last_y;

void mouse(int button, int state, int mouse_x, int mouse_y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouse_dragging = 1;
        mouse_last_x = mouse_x;
        mouse_last_y = mouse_y;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        mouse_dragging = 0;
    }

}

void motion(int mouse_x, int mouse_y) {

    if (mouse_dragging) {

        x_off -= mouse_x - mouse_last_x;
        y_off += mouse_y - mouse_last_y;

        mouse_last_x = mouse_x;
        mouse_last_y = mouse_y;

        set_projection();
        glutPostRedisplay();
    }
}

void reshape(int new_w, int new_h) {

    w = new_w;
    h = new_h;

    set_projection();
    glutPostRedisplay();
}

void menu(int id) {

    switch (id) {
        case EXIT:
            exit(0);
            break;
        case FIRST:
            loops = 8000;
            colorful = 0;
            break;
        case SECOND:
            loops = 10000;
            colorful = 1;
            break;
        case TRIANGLE:
            n = 3;
            r = 1.0f / 2.0f;
            break;
        case PENTAGON1:
            n = 5;
            r = 1.0f / 3.0f;
            break;
        case PENTAGON2:
            n = 5;
            r = 3.0f / 8.0f;
            break;
        case HEXAGON:
            n = 6;
            r = 1.0f / 3.0f;
            break;
    }

    create_seed();
    glutPostRedisplay();
}

void create_menu() {

    int shapes = glutCreateMenu(menu);
    glutAddMenuEntry("Triangle", TRIANGLE);
    glutAddMenuEntry("Pentagon 1", PENTAGON1);
    glutAddMenuEntry("Pentagon 2", PENTAGON2);
    glutAddMenuEntry("Hexagon", HEXAGON);

    glutCreateMenu(menu);
    glutAddMenuEntry("8000 points, same color", FIRST);
    glutAddMenuEntry("10000 points, random color", SECOND);
    glutAddSubMenu("Fractal",shapes);
    glutAddMenuEntry("Exit", EXIT);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void create_seed() {

    // TODO make true random
    seed = rand();
}

void main(int argc, char **argv) {

    w = 500;
    h = 500;

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Fractals");


    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    create_seed();
    create_menu();

    init();
    glutMainLoop(); /* enter event loop */
}
