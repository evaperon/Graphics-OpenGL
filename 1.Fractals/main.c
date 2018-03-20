
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TAU 6.28318530718

typedef GLfloat point2[2];

int w, h;

int n = 5;
float r = 1.0f / 3.0f;
int loops = 10000;

void myinit(void) {

/* attributes */
	glEnable(GL_BLEND);
	glClearColor(1.0, 1.0, 1.0, 0.0); /* white background */
	glColor3f(1.0, 0.0, 0.0); /* draw in red */

/* set up viewing */
/* 500 x 500 window with origin lower left */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
	glMatrixMode(GL_MODELVIEW);
}

void create_polygon(int n, point2 vertices[]) {

	int size = (w < h ? w : h) / 4; // half the size of the screen

	for (int i = 0; i < n; i++) {

		vertices[i][0] = w/2 + sin(i * TAU / n) * size;
		vertices[i][1] = h/2 + cos(i * TAU / n) * size;
	}

}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT);

	point2 vertices[n];
	create_polygon(n, vertices);

//	glBegin(GL_TRIANGLE_FAN);
//	for (int i = 0; i < n; i++) glVertex2fv(vertices[i]);
//	glEnd();

	point2 p = {vertices[0][0], vertices[0][1]}; // TODO make it random

	glBegin(GL_POINTS);
	for (int i = 0; i < loops; i++) {

		int j = rand() % n;

		p[0] += (vertices[j][0] - p[0]) * (1-r);
		p[1] += (vertices[j][1] - p[1]) * (1-r);

		glVertex2fv(p);
	}
	glEnd();

	glutSwapBuffers();
}

int mouse_dragging = 0, mouse_last_x, mouse_last_y;

void mouse(int button, int state, int mouse_x, int mouse_y) {

}

void motion(int mouse_x, int mouse_y) {

}

void reshape(int new_w, int new_h) {

//	w = new_w;
//	h = new_h;
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0.0, w, 0.0, h);
//	glMatrixMode(GL_MODELVIEW);
//
//	glutPostRedisplay();
}

void create_menu() {

	glutCreateMenu(menu);

	glutAddMenuEntry("test", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int id) {

}

void main(int argc, char **argv) {

/* Standard GLUT initialization */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); /* default, not needed */

	glutInitWindowSize(w = 500, h = 500); /* 500 x 500 pixel window */
	glutInitWindowPosition(0, 0); /* place window top left on display */
	glutCreateWindow("Fractals");

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);

	create_menu();

	myinit();

	glutMainLoop(); /* enter event loop */
}
