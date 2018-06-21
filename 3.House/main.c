
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
	EXIT,
	POLYGONS_LOW, POLYGONS_HIGH, POLYGONS_ULTRA,
	SPOTLIGHT_ON, SPOTLIGHT_ON_MOVING, SPOTLIGHT_OFF,
	GRASS_NONE, GRASS_SHORT, GRASS_TALL,
	SUN_STATIC, SUN_DYNAMIC,
	SHADER_SMOOTH, SHADER_FLAT
};

// === Data ===

int w, h;
int seed;
int polygons;

float sun_speed = 40;
float camera_rotation = 0;
float spotlight_speed = 0;
float grass = 0;
char sun_dynamic = 0;

float t; // contains the current time in seconds

// ==

const GLfloat sunlight_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat sunlight_position[] = {0.0f, 0.0f, 0.0f, 1.0f};

const GLfloat spotlight_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat spotlight_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat spotlight_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

const GLfloat sphere_emission[] = {1.0f, 1.0f, 0.0f, 1.0f};
const GLfloat sphere_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat sphere_diffuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat sphere_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

const GLfloat ground_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat ground_ambient[] = {0.5f, 1.0f, 0.0f, 1.0f};
const GLfloat ground_diffuse[] = {0.5f, 1.0f, 0.0f, 1.0f};
const GLfloat ground_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

const GLfloat soil_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat soil_ambient[] = {0.4f, 0.15f, 0.0f, 1.0f};
const GLfloat soil_diffuse[] = {0.4f, 0.15f, 0.0f, 1.0f};
const GLfloat soil_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

const GLfloat house_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat house_ambient[] = {0.4f, 0.15f, 0.0f, 1.0f};
const GLfloat house_diffuse[] = {0.4f, 0.15f, 0.0f, 1.0f};
const GLfloat house_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

const GLfloat roof_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat roof_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
const GLfloat roof_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
const GLfloat roof_specular[] = {0.9f, 0.9f, 0.9f, 1.0f};
const GLfloat high_shin[] = {100.0f};

const int grass_soil_ration = 12;

// === Declarations ===

void init();

void display();

void idle();

void set_projection();

void create_menu();

void create_seed();

void reshape(int new_w, int new_h);

void menu(int id);

void draw_quad(point3 p1, point3 p2, point3 p3, point3 p4);

void draw_triangle(point3 p1, point3 p2, point3 p3);

// === Utils ===

void point3_mid(point3 p, point3 p1, point3 p2) {

	for (int i = 0; i < 3; i++) p[i] = (p1[i] + p2[i]) / 2;
}

void point3_add(point3 p, point3 p1, point3 p2) {

	for (int i = 0; i < 3; i++) p[i] = p1[i] + p2[i];
}

void point3_dif(point3 p, point3 p1, point3 p2) {

	for (int i = 0; i < 3; i++) p[i] = p1[i] - p2[i];
}

void point3_norm(point3 p) {

	float len = sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
	if (len == 0) return;

	for (int i = 0; i < 3; i++) p[i] /= len;
}

void point3_normal(point3 p, point3 p1, point3 p2, point3 p3, point3 p4) {

	point3 a, b;
	point3_dif(a, p3, p1);
	point3_dif(b, p4, p2);

	p[0] = a[1] * b[2] - a[2] * b[1];
	p[1] = a[2] * b[0] - a[0] * b[2];
	p[2] = a[0] * b[1] - a[1] * b[0];

	point3_norm(p);
}

void draw_quad(point3 p1, point3 p2, point3 p3, point3 p4) {

	point3 normal;
	point3_normal(normal, p1, p2, p3, p4);

	glBegin(GL_QUADS);
	glNormal3fv(normal);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p4);
	glEnd();
}

void draw_triangle(point3 p1, point3 p2, point3 p3) {

	point3 normal;
	point3_normal(normal, p1, p2, p3, p3); // TODO fix

	glBegin(GL_TRIANGLES);
	glNormal3fv(normal);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glEnd();
}

// === Implementation ===

void init() {

	glClearColor(0.0, 0.0, 0.0, 1.0);
	set_projection();
}

void set_projection() {


	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float s = 60;

	// keep a fixed ratio
	if (h > w) {
		float ratio = 1.0f * h / w;
		glOrtho(-s, s, -s * ratio, s * ratio, -300, 300);
	} else {
		float ratio = 1.0f * w / h;
		glOrtho(-s * ratio, s * ratio, -s, s, -300, 300);
	}

	glMatrixMode(GL_MODELVIEW);
}

void draw_ground() {

	GLfloat size = 40;

	point3 p1 = {-size, 0, size};
	point3 p2 = {size, 0, size};
	point3 p3 = {size, 0, -size};
	point3 p4 = {-size, 0, -size};

	if (grass > 0 && polygons > 0) {

		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, soil_emission);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, soil_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, soil_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, soil_specular);

		draw_quad(p1, p2, p3, p4);
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ground_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ground_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ground_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ground_specular);

	if (polygons == 1) {

		draw_quad(p1, p2, p3, p4);

	} else {

		point3 normal;
		point3_normal(normal, p1, p2, p3, p4);

		float step = 2 * size / sqrt(polygons);

		for (float x = -size; x < size; x += step) {

			glBegin(GL_QUAD_STRIP);
			glNormal3fv(normal);

			for (float z = -size; z <= size; z += step) {

				// when grass is on, mix the materials
				if (grass > 0 && rand() % grass_soil_ration == 0) {
					glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, soil_emission);
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, soil_ambient);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, soil_diffuse);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, soil_specular);
				} else {
					glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ground_emission);
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ground_ambient);
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ground_diffuse);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ground_specular);
				}

				float h1 = grass * ((rand() % 1000) / 1000.0f);
				float h2 = grass * ((rand() % 1000) / 1000.0f);

				point3 p1 = {x, h1, z};
				point3 p2 = {x + step, h2, z};
				glVertex3fv(p1);
				glVertex3fv(p2);
			}

			glEnd();
		}

	}

}

void draw_house() {

	// body

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, house_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, house_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, house_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, house_specular);

	{
		point3 p1 = {-5, 0, 10};
		point3 p2 = {-5, 10, 10};
		point3 p3 = {5, 10, 10};
		point3 p4 = {5, 0, 10};

		draw_quad(p1, p2, p3, p4);
	}
	{
		point3 p1 = {-5, 0, -10};
		point3 p2 = {-5, 10, -10};
		point3 p3 = {5, 10, -10};
		point3 p4 = {5, 0, -10};

		draw_quad(p1, p2, p3, p4);
	}
	{ // left
		point3 p1 = {-5, 0, -10};
		point3 p2 = {-5, 0, 10};
		point3 p3 = {-5, 10, 10};
		point3 p4 = {-5, 10, -10};

		draw_quad(p1, p2, p3, p4);
	}
	{
		point3 p1 = {5, 0, -10};
		point3 p2 = {5, 10, -10};
		point3 p3 = {5, 10, 10};
		point3 p4 = {5, 0, 10};

		draw_quad(p1, p2, p3, p4);
	}
	{
		point3 p1 = {5, 0, -10};
		point3 p2 = {-5, 0, -10};
		point3 p3 = {-5, 0, 10};
		point3 p4 = {5, 0, 10};

		draw_quad(p1, p2, p3, p4);
	}
	{
		point3 p1 = {5, 10, -10};
		point3 p2 = {-5, 10, -10};
		point3 p3 = {-5, 10, 10};
		point3 p4 = {5, 10, 10};

		draw_quad(p1, p2, p3, p4);
	}

	// roof

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, roof_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, roof_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, roof_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, roof_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shin);


	GLfloat rooftop = 10 + sin(M_PI / 3) * 10;

	{ // left
		point3 p1 = {0, rooftop, 10};
		point3 p2 = {0, rooftop, -10};
		point3 p3 = {-5, 10, -10};
		point3 p4 = {-5, 10, 10};

		draw_quad(p1, p2, p3, p4);
	}
	{
		point3 p1 = {5, 10, 10};
		point3 p2 = {5, 10, -10};
		point3 p3 = {0, rooftop, -10};
		point3 p4 = {0, rooftop, 10};

		draw_quad(p1, p2, p3, p4);
	}
	{
		point3 p1 = {-5, 10, 10};
		point3 p2 = {0, rooftop, 10};
		point3 p3 = {5, 10, 10};

		draw_triangle(p1, p2, p3);
	}
	{
		point3 p1 = {-5, 10, -10};
		point3 p2 = {5, 10, -10};
		point3 p3 = {0, rooftop, -10};

		draw_triangle(p1, p2, p3);
	}

}

void draw_part(point3 p1, point3 p2, point3 p3, int splits) {

	if (splits == 0) {

		glBegin(GL_TRIANGLES);
		glVertex3fv(p1);
		glVertex3fv(p2);
		glVertex3fv(p3);
		glEnd();

	} else {

		point3 p12, p23, p31;
		point3_mid(p12, p1, p2);
		point3_mid(p23, p2, p3);
		point3_mid(p31, p3, p1);

		point3_norm(p12);
		point3_norm(p23);
		point3_norm(p31);

		draw_part(p1, p12, p31, splits - 1);
		draw_part(p2, p23, p12, splits - 1);
		draw_part(p3, p31, p23, splits - 1);
		draw_part(p12, p23, p31, splits - 1);
	}
}

void draw_sun(float sun_angle) {

	glLightfv(GL_LIGHT0, GL_POSITION, sunlight_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunlight_ambient);

	float lumens = 0.3 + 0.7 * sin(sun_angle * M_PI / 180); // from 0.3 to 1.0

	if (sun_dynamic) {

		float mod = pow (sin(sun_angle * M_PI / 180), 2);

		GLfloat sunlight_diffuse[] = {lumens, lumens * mod, lumens * mod, 1.0f};
		GLfloat sunlight_specular[] = {lumens, lumens * mod, lumens * mod, 1.0f};

		glLightfv(GL_LIGHT0, GL_DIFFUSE, sunlight_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sunlight_specular);

	} else {

		GLfloat sunlight_diffuse[] = {lumens, lumens, lumens, 1.0f};
		GLfloat sunlight_specular[] = {lumens, lumens, lumens, 1.0f};

		glLightfv(GL_LIGHT0, GL_DIFFUSE, sunlight_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sunlight_specular);
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sphere_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sphere_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sphere_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sphere_specular);

	point3 v[] = {
		{0.0,        0.0,        1.0f},
		{0.0,        0.942809f,  -0.33333f},
		{-0.816497f, -0.471405f, -0.333333f},
		{0.816497f,  -0.471405f, -0.333333f}
	};

	int splits = 4;

	draw_part(v[0], v[1], v[2], splits);
	draw_part(v[0], v[2], v[3], splits);
	draw_part(v[0], v[3], v[1], splits);
	draw_part(v[1], v[2], v[3], splits);

}

void draw_spotlight() {

	GLfloat rooftop = 10 + sin(M_PI / 3) * 10;

	GLfloat p[] = {0.01f, rooftop, 10, 1};

	float angle = sin(t * spotlight_speed) * 0.5;
	GLfloat direction[] = {angle, -1.0f, 0.4f};
	point3_norm(direction);

	glLightfv(GL_LIGHT1, GL_AMBIENT, spotlight_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spotlight_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spotlight_specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0f);
	glLightfv(GL_LIGHT1, GL_POSITION, p);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);

}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	srand(seed);

	glLoadIdentity();

	gluLookAt(0, 40, 70, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(camera_rotation, 0, 1, 0);

	float sun_angle = fmod(t * sun_speed, 180); // in degrees

	draw_ground();
	draw_house();
	draw_spotlight();

	glPushMatrix();
	glRotatef(-sun_angle, 0, 0, 1);
	glTranslatef(-50, 0, 0);
	draw_sun(sun_angle);
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

	switch (id) {
		case EXIT:
			exit(0);
			break;
		case POLYGONS_LOW:
			polygons = 1;
			break;
		case POLYGONS_HIGH:
			polygons = 100;
			break;
		case POLYGONS_ULTRA:
			polygons = 10000;
			break;
		case SPOTLIGHT_ON:
			glEnable(GL_LIGHT1);
			spotlight_speed = 0;
			break;
		case SPOTLIGHT_ON_MOVING:
			glEnable(GL_LIGHT1);
			spotlight_speed = 1;
			break;
		case SPOTLIGHT_OFF:
			glDisable(GL_LIGHT1);
			break;
		case GRASS_NONE:
			grass = 0;
			break;
		case GRASS_SHORT:
			grass = 5;
			menu(POLYGONS_ULTRA);
			break;
		case GRASS_TALL:
			grass = 8;
			menu(POLYGONS_ULTRA);
			break;
		case SUN_STATIC:
			sun_dynamic = 0;
			break;
		case SUN_DYNAMIC:
			sun_dynamic = 1;
			break;
		case SHADER_SMOOTH:
			glShadeModel(GL_SMOOTH);
			break;
		case SHADER_FLAT:
			glShadeModel(GL_FLAT);
			break;
		default:
			return;
	}

	glutPostRedisplay();
}

void create_menu() {

	int menu_polygons = glutCreateMenu(menu);
	glutAddMenuEntry("Low", POLYGONS_LOW);
	glutAddMenuEntry("High", POLYGONS_HIGH);
	glutAddMenuEntry("Ultra", POLYGONS_ULTRA);

	int menu_spotlight = glutCreateMenu(menu);
	glutAddMenuEntry("On", SPOTLIGHT_ON);
	glutAddMenuEntry("On Moving", SPOTLIGHT_ON_MOVING);
	glutAddMenuEntry("Off", SPOTLIGHT_OFF);

	int menu_grass = glutCreateMenu(menu);
	glutAddMenuEntry("Flat", GRASS_NONE);
	glutAddMenuEntry("Short", GRASS_SHORT);
	glutAddMenuEntry("Tall", GRASS_TALL);

	int menu_sun = glutCreateMenu(menu);
	glutAddMenuEntry("Static hue", SUN_STATIC);
	glutAddMenuEntry("Dynamic hue", SUN_DYNAMIC);

	int menu_shader = glutCreateMenu(menu);
	glutAddMenuEntry("Smooth", SHADER_SMOOTH);
	glutAddMenuEntry("Flat", SHADER_FLAT);

	glutCreateMenu(menu);
	glutAddSubMenu("Polygons", menu_polygons);
	glutAddSubMenu("Spotlight", menu_spotlight);
	glutAddSubMenu("Grass", menu_grass);
	glutAddSubMenu("Sun", menu_sun);
	glutAddSubMenu("Shader", menu_shader);
	glutAddMenuEntry("Exit", EXIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void camera_rotate(float turn) {

	camera_rotation += turn;
}

void key(unsigned char key, int x, int y) {

	if (key == 'q') {
		camera_rotate(1);
	} else if (key == 'e') {
		camera_rotate(-1);
	}
}

void mouse(int button, int state, int x, int y) {

//	if (state != GLUT_DOWN) {
//
//		if (button == GLUT_LEFT_BUTTON) {
//			camera_rotate(1);
//		} else if (button == GLUT_RIGHT_BUTTON) {
//			camera_rotate(-1);
//		}
//	}
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
	glutCreateWindow("House");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// setup event handlers
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);

	create_seed();
	create_menu();

	// init starting state
	menu(POLYGONS_HIGH);
	menu(SPOTLIGHT_OFF);
	menu(GRASS_NONE);
	menu(SUN_STATIC);
	menu(SHADER_SMOOTH);

	init();
	glutMainLoop();
}
