#include <GL/glut.h>
#include <GL/glu.h>

#include "linear.h"

void draw_vec3(vec3 *o, vec3 *v)
{
	glBegin(GL_LINES);
		/* back */
		glNormal3f(0.0, 0.0, 1.0);
		glColor3f(1, 1, 1);

		if (o) {
			glVertex3f((*o)[0], (*o)[1], (*o)[2]);
			glVertex3f((*o)[0] + (*v)[0], (*o)[1] + (*v)[1], (*o)[2] + (*v)[2]);
		} else {
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f((*v)[0], (*v)[1], (*v)[2]);
		}

	glEnd();
}

void draw_vec4(vec4 *o, vec4 *v)
{
	glBegin(GL_LINES);
		/* back */
		glNormal3f(0.0, 0.0, 1.0);
		glColor3f(1, 1, 1);

		if (o) {
			glVertex3f((*o)[0], (*o)[1], (*o)[2]);
			glVertex3f((*o)[0] + (*v)[0], (*o)[1] + (*v)[1], (*o)[2] + (*v)[2]);
		} else {
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f((*v)[0], (*v)[1], (*v)[2]);
		}

	glEnd();
}

GLuint frame_display_list(void)
{
	GLuint list;
	vec3 vertex[] = {
		{0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0}
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);

	list = glGenLists(1);

	glNewList(list, GL_COMPILE);
		glBegin(GL_LINES);
			/* Bottom */
			glNormal3f(0.0, 1.0, 0.0);

			/* x */
			glColor3f(1, 0, 0);
			glArrayElement(0);
			glArrayElement(1);

			/* z */
			glColor3f(0, 0, 1);
			glArrayElement(0);
			glArrayElement(3);

			/* Left */
			glNormal3f(1.0, 0.0, 0.0);

			/* z */
			glColor3f(0, 0, 1);
			glArrayElement(0);
			glArrayElement(3);

			/* y */
			glColor3f(0, 1, 0);
			glArrayElement(0);
			glArrayElement(2);

			/* Back */
			glNormal3f(0.0, 0.0, 1.0);
			/* x */
			glColor3f(1, 0, 0);
			glArrayElement(0);
			glArrayElement(1);

			/* y */
			glColor3f(0, 1, 0);
			glArrayElement(0);
			glArrayElement(2);

		glEnd();
	glEndList();

	return list;
}



GLuint cube_display_list(void)
{
	GLuint list;
	vec3 vertex[] = {
		{0.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{1.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},

		{0.0, 0.0, 1.0},
		{1.0, 0.0, 1.0},
		{1.0, 1.0, 1.0},
		{0.0, 1.0, 1.0},

		{0.5, 0.5, 0.5},
		{2.5, 0.5, 0.5},
		{0.5, 2.5, 0.5},
		{0.5, 0.5, 2.5},
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);

	list = glGenLists(1);

	glNewList(list, GL_COMPILE);
		glBegin(GL_QUADS);
			/* back */
			glColor3f(1, 0, 0);
			glNormal3f(0.0, 0.0, -1.0);
			glArrayElement(0);
			glArrayElement(1);
			glArrayElement(2);
			glArrayElement(3);

			/* front */
			glColor3f(0, 1, 0);
			glNormal3f(0.0, 0.0, 1.0);
			glArrayElement(4);
			glArrayElement(5);
			glArrayElement(6);
			glArrayElement(7);

			/* left */
			glColor3f(0, 0, 1);
			glNormal3f(-1.0, 0.0, 0.0);
			glArrayElement(0);
			glArrayElement(4);
			glArrayElement(7);
			glArrayElement(1);

			/* right */
			glNormal3f(1.0, 0.0, 0.0);
			glArrayElement(5);
			glArrayElement(6);
			glArrayElement(2);
			glArrayElement(3);

			/* top */
			glNormal3f(0.0, 1.0, 0.0);
			glArrayElement(7);
			glArrayElement(6);
			glArrayElement(2);
			glArrayElement(1);

			/* bottom */
			glNormal3f(0.0, -1.0, 0.0);
			glArrayElement(4);
			glArrayElement(0);
			glArrayElement(3);
			glArrayElement(5);
		glEnd();

#if 0
		glBegin(GL_LINES);
			glNormal3f(0.0, 0.0, 1.0);
			glColor3f(1, 0, 0);
			glArrayElement(8);
			glArrayElement(9);

			glColor3f(0, 1, 0);
			glArrayElement(8);
			glArrayElement(10);

			glColor3f(0, 0, 1);
			glArrayElement(8);
			glArrayElement(11);
		glEnd();
#endif
	glEndList();

	return list;
}


GLuint cylinder_display_list(float step)
{
	GLuint list;
	float angle;
	int i;
	int imax = (int)((360.0f/step) + 1.0f) + 1;
	vec3 vertex[imax];
	list = glGenLists(1);
	vec3 center = {0.5, 0.5, 0.5};
	float r = 0.5;

	for (i = 0, angle = 0.0; i < imax && angle < 360.0f; i++, angle = i * step) {
		vertex[i][0] = cosf(angle * M_PI / 180.0f);
		vertex[i][2] = sinf(angle * M_PI / 180.0f);
	}

	vertex[i][0] = cosf(0.0);
	vertex[i][2] = sinf(0.0);
	imax = i + 1;

	glNewList(list, GL_COMPILE);

		glBegin(GL_TRIANGLE_FAN);

			/* top face */
			glColor3f(1, 0, 0);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(center[0], 1.0, center[2]);

			for (i = 0; i < imax; i++)
				glVertex3f(center[0] + r * vertex[i][0], 1.0, center[2] + r * vertex[i][2]);

			glEnd();

		glBegin(GL_TRIANGLE_FAN);

			/* bottom face */
			glColor3f(0, 0, 1);
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(center[0], 0.0, center[2]);

			for (i = 0; i < imax; i++)
				glVertex3f(center[0] + r * vertex[i][0], 0.0, center[2] + r * vertex[i][2]);

		glEnd();

		glBegin(GL_QUADS);

			/* side faces */
			glColor3f(0, 1, 0);

			for (i = 0; i < imax - 1; i++) {
				glNormal3f(vertex[i][0], 0.0, vertex[i][2]);
				glVertex3f(center[0] + r * vertex[i][0], 0.0, center[2] + r * vertex[i][2]);
				glVertex3f(center[0] + r * vertex[i][0], 1.0, center[2] + r * vertex[i][2]);

				glNormal3f(vertex[i + 1][0], 0.0, vertex[i + 1][2]);
				glVertex3f(center[0] + r * vertex[i + 1][0], 1.0, center[2] + r * vertex[i + 1][2]);
				glVertex3f(center[0] + r * vertex[i + 1][0], 0.0, center[2] + r * vertex[i + 1][2]);
			}

		glEnd();

	glEndList();

	return list;
}
