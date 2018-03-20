
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
char colorful = 0;

float x_off = 0, y_off = 0;

void myinit(void) {

/* attributes */
	glEnable(GL_BLEND);
	glClearColor(1.0, 1.0, 1.0, 0.0); /* white background */
	glColor3f(1.0, 0.0, 0.0); /* draw in red */

/* set up viewing */
/* 500 x 500 window with origin lower left */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0 + x_off, w + x_off, 0.0 + y_off, h + y_off);
	glMatrixMode(GL_MODELVIEW);
}

void create_polygon(int n, point2 vertices[]) {

	int size = (w < h ? w : h) / 4; // half the size of the screen

	for (int i = 0; i < n; i++) {

		vertices[i][0] = w/2 + sin(i * TAU / n) * size;
		vertices[i][1] = h/2 + cos(i * TAU / n) * size;
	}

}

void set_random_color() {

	// TODO generate better colors
	glColor3ub(rand() % 255, rand() % 255, rand() % 255);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT);

	point2 vertices[n];
	create_polygon(n, vertices);

//	glBegin(GL_TRIANGLE_FAN);
//	for (int i = 0; i < n; i++) glVertex2fv(vertices[i]);
//	glEnd();

	point2 p = {vertices[0][0], vertices[0][1]}; // TODO make it random

	set_random_color();
	glBegin(GL_POINTS);
	for (int i = 0; i < loops; i++) {

		int j = rand() % n;

		p[0] += (vertices[j][0] - p[0]) * (1-r);
		p[1] += (vertices[j][1] - p[1]) * (1-r);

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

		myinit();
		glutPostRedisplay();

	}


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

void menu(int id) {

	if (id == 0) {
		exit(0);
	}

	switch (id) {
		case 1:
			loops = 8000;
			colorful = 0;
			break;
		case 2:
			loops = 10000;
			colorful = 1;
			break;
	}

	glutPostRedisplay();
}

void create_menu() {

	glutCreateMenu(menu);

	glutAddMenuEntry("8000 points, same color", 1);
	glutAddMenuEntry("10000 points, random color", 2);
	glutAddMenuEntry("Exit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
