#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glut.h>
#endif

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int spinxface;
int spinyface;
int origx, origy;
bool rotate = false, to_scale_up = false, to_scale_down = false, auto_rotate = true;

struct {
	double ** ptr;
	size_t length;
} map;

enum {
	ROTATE_RIGHT = -1,
	ROTATE_LEFT = 1
} direction;

static inline double scale(double valueIn, double baseMin, double baseMax, double limitMin, double limitMax) {
	return ((limitMax - limitMin) * (valueIn - baseMin) / (baseMax - baseMin)) + limitMin;
}

static void colour(float c1, float c2)
{
	/*if (fabs(c1) < 0.001 && fabs(c2) < 0.001)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(fabs(c1) * 8, fabs(c2) * 16, 0.1);*/
	glColor3f(0.0, 0.5, 0.0);
}

static void motion(int x, int y)
{
	if (rotate) {
		spinyface = (x - origx) % 360;
		spinxface = (y - origy) % 360;
		origx = x;
		origy = y;
		glutPostRedisplay();
	}
}

static void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				origx = x;
				origy = y;
				rotate = true;
				auto_rotate = false;
			} else {
				rotate = false;
			}
			break;
		default:
			break;
	}
}

static void keyboard(unsigned char key, int x, int y)
{
	if (x == y)
		;
	switch (key) {
		case 'r':
		case 'R':
			auto_rotate = true;
			break;
		case 27:             // ESCAPE key
			exit (0);
			break;
		default:
			break;

	}
}

static void special_keyboard(int key, int x, int y)
{
	if (x == y)
		;
	switch (key) {
		case GLUT_KEY_UP:
			auto_rotate = false;
			to_scale_up = true;
			break;
		case GLUT_KEY_DOWN:
			auto_rotate = false;
			to_scale_down = true;
			break;
		default:
			break;
	}
}

static void drawMap(size_t length, double *terrain[static length])
{
	static const double line_width = 0.125;
	const double next_point = 2.0 / (length - 2);

	glLineWidth(line_width);
	glBegin(GL_LINES);

	colour(0.0, 0.0);
	for (size_t i = 0; i < length - 1; ++i) {
		double xc = scale(i, 0, length - 2, -1.0, 1.0);
		for (size_t j = 0; j < length - 1; ++j) {
			double yc = scale(j, 0, length - 2, -1.0, 1.0);
			//Virtical lines

			glVertex3f(xc, terrain[i][j], yc);
			glVertex3f(xc, terrain[i][j + 1], yc + next_point);

			//horizontal lines
			//colour(terrain[i][j], terrain[i + 1][j]);

			glVertex3f(xc, terrain[i][j], yc);
			glVertex3f(xc + next_point, terrain[i + 1][j], yc);

			//diagonal lines
			//colour(terrain[i][j], terrain[i + 1][j + 1]);

			glVertex3f(xc, terrain[i][j], yc);
			glVertex3f(xc + next_point, terrain[i + 1][j + 1], yc + next_point);
		}
	}
	glEnd();
}

static void display(void)
{
	static bool init_rotate = true;
	if (init_rotate) {
		glRotatef(20, 1.0, 0.0, 0.0);
		glScalef(1.9, 1.9, 1.9);
		init_rotate = false;
	}
	if (!auto_rotate) {
		if (rotate) {
			glRotated((GLdouble) spinxface, 1.0, 0.0, 0.0);
			glRotated((GLdouble) spinyface, 0.0, 1.0, 0.0);
			/*glRotatef(app_state.x, 1, 0, 0);
			glRotatef(app_state.y, 0, 1, 0);
			glRotatef(app_state.z, 0, 0, 1);*/
		}
		if (to_scale_up) {
			glScalef(1.11, 1.11, 1.11);
			to_scale_up = false;
		} else if (to_scale_down) {
			glScalef(0.89, 0.89, 0.89);
			to_scale_down = false;
		}
	} else {
		glRotatef(-0.1, 0.0, 0.00001, 0.0);
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawMap(map.length, map.ptr);


	glutSwapBuffers();
	glFlush();
}

extern void drawTerrain(size_t length, double *heightmap[static length])
{
	int argc = 1;
	char * argv[] = {"Terrain Map"};
	if (heightmap == NULL || length == 0)
		return;
	map.ptr = heightmap;
	map.length = length;

	double max = 0.0;
	for (size_t i = 0; i < length; ++i)
		for (size_t j = 0; j < length; ++j)
			if (fabs(map.ptr[i][j]) > max)
				max = fabs(map.ptr[i][j]);

	for (size_t i = 0; i < length; ++i)
		for (size_t j = 0; j < length; ++j)
			map.ptr[i][j] /= max;
	map.ptr[0][0] /= 2.0;
	map.ptr[0][length - 1] /= 2.0;
	map.ptr[length - 1][0] /= 2.0;
	map.ptr[length - 1][length - 1] /= 2.0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(1280, 800);
	glutCreateWindow(argv[0]);

	glClearColor(0.1, 0.1, 0.5, 1.0);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	//glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keyboard);
	glutIdleFunc(glutPostRedisplay);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}
