
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

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
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT);

	point2 vertices[n];

	for (int i = 0; i < n; i++) {

		vertices[i][0] = 0;
		vertices[i][1] = 0;
	}

	point2 p = {75.0, 50.0}; // TODO make it random

	glBegin(GL_POINTS);
	for (int k = 0; k < loops; k++) {

		int j = rand() % 3;

		p[0] = (p[0] + vertices[j][0]) * r;
		p[1] = (p[1] + vertices[j][1]) * r;

		glVertex2fv(p);
	}
	glEnd();

	glutSwapBuffers();
}

void mouse(int button, int state, int mouse_x, int mouse_y) {

}

void reshape(int new_w, int new_h) {

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
	glutReshapeFunc(reshape);

	myinit();

	glutMainLoop(); /* enter event loop */
}
