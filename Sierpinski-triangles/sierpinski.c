/*
Copyright 2017-18 Sireesh Kodali

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY,WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glut.h>
#endif

#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>

float smallest_length = 0.015625f;
int iter = 0;
bool keep_zooming = true;
const unsigned max_iters = 865;

void mouse(int button, int state, int x, int y)
{
	keep_zooming = !keep_zooming;
	button = state = x = y = 0;
}

void drawSubTriangle(float x1, float x2, float x3, float y1, float y2, float y3)
{
	//random values that result in a cyanish colour
	glColor3f(0.4, 0.8, 0.8);
	//if we want colours, we need to ensure that
	//the colour does not cause issues with redraws
	//glColor3f(fabs(y1) * 0.8 - fabs(x1), green, fabs(y2) + fabs(x3) * 0.1);

	//draw lines around the borders
	glLineWidth(smallest_length);
	//glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	{
		glVertex3f(x1, y1, 0.0);
		glVertex3f(x2, y2, 0.0);

		glVertex3f(x2, y2, 0.0);
		glVertex3f(x3, y3, 0.0);

		glVertex3f(x3, y3, 0.0);
		glVertex3f(x1, y1, 0.0);
	}
	glEnd();
}

void drawSierpinski(const float x1, const float x2, const float x3, const float y1, const float y2, const float y3)
{
	drawSubTriangle(x1, x2, x3, y1, y2, y3);
	//ensure that the size of the triangles is not too small
	//check that the side length of each triangle is large enough
	if (pow((pow((x2 - x1), 2) + pow((y2 - y1), 2)), 0.5) > smallest_length / 2) //formula is ((x2 - x1)^2 + (y2 - y1)^2)^0.5
	{
		//pick the midpoints of two sides and one vertex for the next triangle
		drawSierpinski((x1 + x3) / 2, (x3 + x2) / 2, x3, (y1 + y3) / 2, (y3 + y1) / 2, y3); //top triangle
		drawSierpinski(x1, (x1 + x2) / 2, (x1 + x3) / 2, y1, (y1 + y2) / 2, (y1 + y3) / 2); //left triangle
		drawSierpinski((x2 + x1) / 2, x2, (x3 + x2) / 2, (y2 + y1) / 2, y2, (y3 + y2) / 2); //right triangle
	}
}

void display(void)
{
	//the points for the initial triangle
	const float ox1 = -1.0, ox2 = 1.0, ox3 = 0.0, oy1 = -1.0, oy2 = -1.0, oy3 = 1;

	//glEnable(GL_LINE_SMOOTH);
	if (keep_zooming) {
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.0f, 0.0f, 0.0f);
		drawSierpinski(ox1, ox2, ox3, oy1, oy2, oy3);
		//Redraw the triangles with higher resolution to make the transition more smooth
		if (iter++ == max_iters) {
			iter = 0;
			smallest_length *= 1.005;
			glLoadIdentity();
		} else if (iter == max_iters / 4) {
			smallest_length /= 1.00166;
		} else if (iter == max_iters / 2) {
			smallest_length /= 1.00166;
		} else if (iter == 648) {
			smallest_length /= 1.00166;
		}
		glScalef(1.0008, 1.0008, 0.0);
		glTranslatef(0.0008, 0.0008, 0.0);

		glutSwapBuffers();
		glFlush();
	}
}

int main(int argc, char *argv[static argc])
{
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'l')
		printf(
"Copyright 2017-18 Sireesh Kodali\n\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"),"
"to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,"
"and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
"LIABILITY,WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(1280, 800);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutIdleFunc(glutPostRedisplay);

	glutMainLoop();
	return 0;
}
