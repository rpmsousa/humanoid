//#include <GL/glut.h>
#include <GL/glu.h>

#include "linear.h"

static GLuint _cylinder_list, _cone_list, _vector_list;
#define MIN	0.00001f


/* Returns a display list for a cylinder of radius r and height h along the z direction.
 * The cylinder will have N faces
 * The base of the cylinder will be parallel to the xy plane, centered around "o"
 * All polygons front face is drawn counter clock wise */
void cylinder_vertex_list(vec3 *o, float r, float h, int faces)
{
	float angle;
	int i;
	int imax = faces + 1;
	float step = 360.0f / (float) faces;
	vec3 vertex[imax];
	vec3 center = {r, r, h/2.0f};
	vec3 v0, v1, v2, v3, p0, p1, norm;

	if (o) {
		center[0] += (*o)[0];
		center[1] += (*o)[1];
		center[2] += (*o)[2];
	}

	for (i = 0, angle = 0.0; i < imax && angle < 360.0f; i++, angle = i * step) {
		vertex[i][0] = cosf(angle * M_PI / 180.0f);
		vertex[i][1] = sinf(angle * M_PI / 180.0f);
	}

	vertex[i][0] = cosf(0.0);
	vertex[i][1] = sinf(0.0);
	imax = i + 1;

	glBegin(GL_TRIANGLE_FAN);

		/* top face */
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(center[0], center[1], center[2] + h / 2.0f);

		for (i = 0; i < imax; i++)
			glVertex3f(center[0] + r * vertex[i][0], center[1] + r * vertex[i][1], center[2] +  h / 2.0f);

	glEnd();

	glBegin(GL_TRIANGLE_FAN);

		/* bottom face */
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(center[0], center[1], center[2] - h / 2.0f);

		for (i = imax - 1; i >= 0; i--)
			glVertex3f(center[0] + r * vertex[i][0], center[1] + r * vertex[i][1], center[2] - h / 2.0f);

	glEnd();

	glBegin(GL_QUADS);

		/* side faces */

		for (i = 0; i < imax - 1; i++) {
			/* lower left */
			v0[0] = center[0] + r * vertex[i][0];
			v0[1] = center[1] + r * vertex[i][1];
			v0[2] = center[2] - h / 2.0f;

			/* lower right */
			v1[0] = center[0] + r * vertex[i + 1][0];
			v1[1] = center[1] + r * vertex[i + 1][1];
			v1[2] = center[2] - h / 2.0f;

			/* upper right */
			v2[0] = center[0] + r * vertex[i + 1][0];
			v2[1] = center[1] + r * vertex[i + 1][1];
			v2[2] = center[2] + h / 2.0f;

			/* upper left */
			v3[0] = center[0] + r * vertex[i][0];
			v3[1] = center[1] + r * vertex[i][1];
			v3[2] = center[2] + h / 2.0f;

			p0[0] = v1[0] - v0[0];
			p0[1] = v1[1] - v0[1];
			p0[2] = v1[2] - v0[2];

			p1[0] = v3[0] - v0[0];
			p1[1] = v3[1] - v0[1];
			p1[2] = v3[2] - v0[2];

			unit_normal_vec3(&p0, &p1, &norm);
			glNormal3f(norm[0], norm[1], norm[2]);

			glVertex3f(v0[0], v0[1], v0[2]);
			glVertex3f(v1[0], v1[1], v1[2]);
			glVertex3f(v2[0], v2[1], v2[2]);
			glVertex3f(v3[0], v3[1], v3[2]);
		}

	glEnd();

#if NORMAL_DEBUG
	glBegin(GL_LINES);
		for (i = 0; i < imax - 1; i++) {
			/* lower left */
			v0[0] = center[0] + r * vertex[i][0];
			v0[1] = center[1] + r * vertex[i][1];
			v0[2] = center[2] - h / 2.0f;

			/* lower right */
			v1[0] = center[0] + r * vertex[i + 1][0];
			v1[1] = center[1] + r * vertex[i + 1][1];
			v1[2] = center[2] - h / 2.0f;

			/* upper right */
			v2[0] = center[0] + r * vertex[i + 1][0];
			v2[1] = center[1] + r * vertex[i + 1][1];
			v2[2] = center[2] + h / 2.0f;

			/* upper left */
			v3[0] = center[0] + r * vertex[i][0];
			v3[1] = center[1] + r * vertex[i][1];
			v3[2] = center[2] + h / 2.0f;

			p0[0] = v1[0] - v0[0];
			p0[1] = v1[1] - v0[1];
			p0[2] = v1[2] - v0[2];

			p1[0] = v3[0] - v0[0];
			p1[1] = v3[1] - v0[1];
			p1[2] = v3[2] - v0[2];

			unit_normal_vec3(&p0, &p1, &norm);
			glNormal3f(norm[0], norm[1], norm[2]);

			glVertex3f((v0[0] + v1[0])/2, (v0[1] + v1[1])/2, (v0[2] + v3[2])/2);
			glVertex3f((v0[0] + v1[0])/2 + norm[0], (v0[1] + v1[1])/2 + norm[1], (v0[2] + v3[2])/2 + norm[2]);
		}

	glEnd();
#endif
}

GLuint cylinder_display_list(float r, float h, int faces)
{
	GLuint list = glGenLists(1);

	glNewList(list, GL_COMPILE);

	cylinder_vertex_list(NULL, r, h, faces);

	glEndList();

	return list;
}

/* Returns a display list for a cone of radius r and height h along the z direction.
 * The cone will have N faces
 * The base of the cone will be parallel to the xy plane, centered around "o"
 * All polygons front face is drawn counter clock wise */
void cone_vertex_list(vec3 *o, float r, float h, int faces)
{
	float angle;
	int i;
	int imax = faces + 1;
	float step = 360.0f / (float) faces;
	vec3 vertex[imax];
	vec3 center = {0, 0, h/2.0f};
	vec3 v0, v1, v2, p0, p1, norm;

	if (o) {
		center[0] += (*o)[0];
		center[1] += (*o)[1];
		center[2] += (*o)[2];
	}

	for (i = 0, angle = 0.0; i < imax && angle < 360.0f; i++, angle = i * step) {
		vertex[i][0] = cosf(angle * M_PI / 180.0f);
		vertex[i][1] = sinf(angle * M_PI / 180.0f);
	}

	vertex[i][0] = cosf(0.0);
	vertex[i][1] = sinf(0.0);
	imax = i + 1;

	glBegin(GL_TRIANGLE_FAN);

		/* bottom face */
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(center[0], center[1], center[2] - h / 2.0f);

		for (i = imax - 1; i >= 0; i--)
			glVertex3f(center[0] + r * vertex[i][0], center[1] + r * vertex[i][1], center[2] - h / 2.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

		/* side faces */
		for (i = 0; i < imax - 1; i++) {
			/* lower left */
			v0[0] = center[0] + r * vertex[i][0];
			v0[1] = center[1] + r * vertex[i][1];
			v0[2] = center[2] - h / 2.0f;

			/* lower right */
			v1[0] = center[0] + r * vertex[i + 1][0];
			v1[1] = center[1] + r * vertex[i + 1][1];
			v1[2] = center[2] - h / 2.0f;

			/* upper */
			v2[0] = center[0];
			v2[1] = center[1];
			v2[2] = center[2] + h / 2.0f;

			p0[0] = v1[0] - v0[0];
			p0[1] = v1[1] - v0[1];
			p0[2] = v1[2] - v0[2];

			p1[0] = v2[0] - v0[0];
			p1[1] = v2[1] - v0[1];
			p1[2] = v2[2] - v0[2];

			unit_normal_vec3(&p0, &p1, &norm);
			glNormal3f(norm[0], norm[1], norm[2]);

			glVertex3f(v0[0], v0[1], v0[2]);
			glVertex3f(v1[0], v1[1], v1[2]);
			glVertex3f(v2[0], v2[1], v2[2]);
		}

	glEnd();

#if NORMAL_DEBUG
	glBegin(GL_LINES);
		for (i = 0; i < imax - 1; i++) {
			/* lower left */
			v0[0] = center[0] + r * vertex[i][0];
			v0[1] = center[1] + r * vertex[i][1];
			v0[2] = center[2] - h / 2.0f;

			/* lower right */
			v1[0] = center[0] + r * vertex[i + 1][0];
			v1[1] = center[1] + r * vertex[i + 1][1];
			v1[2] = center[2] - h / 2.0f;

			/* upper */
			v2[0] = center[0];
			v2[1] = center[1];
			v2[2] = center[2] + h / 2.0f;

			p0[0] = v1[0] - v0[0];
			p0[1] = v1[1] - v0[1];
			p0[2] = v1[2] - v0[2];

			p1[0] = v2[0] - v0[0];
			p1[1] = v2[1] - v0[1];
			p1[2] = v2[2] - v0[2];

			unit_normal_vec3(&p0, &p1, &norm);

			glVertex3f(v0[0], v0[1], v0[2]);
			glVertex3f(v0[0] + norm[0], v0[1] + norm[1], v0[2] + norm[2]);
		}

	glEnd();
#endif

}

GLuint cone_display_list(float r, float h, int faces)
{
	GLuint list = glGenLists(1);

	glNewList(list, GL_COMPILE);

	cone_vertex_list(NULL, r, h, faces);

	glEndList();

	return list;
}

/* Returns a display list for a vector of length l.
 * The vector will be along the z axis
 * The vector will start at 0,0,0 and end at 0,l,0 */
void vector_vertex_list(float r, float l, int faces)
{
	float length = 0.90 * l;
	vec3 o = {0.0, 0.0, length};

	/* draw cylinder, x% of the length */
	cylinder_vertex_list(NULL, r, length, faces);

	/* draw cone, (1-x)% of the length */
	cone_vertex_list(&o, r * 1.5, 1.0 - length, faces);
}

GLuint vector_display_list(float r, float l, int faces)
{
	GLuint list = glGenLists(1);

	glNewList(list, GL_COMPILE);

	vector_vertex_list(r, l, faces);

	glEndList();

	return list;
}

void draw_vec3(vec3 *o, vec3 *v)
{
	scalar l, angle;
	vec3 n;

	l = norm_vec3(v);

	if (l < MIN)
		return;

	glPushMatrix();

	if (o)
		glTranslatef((*o)[0], (*o)[1], (*o)[2]);

	cross_product_vec3(&vec3_unit_z, v, &n);
	angle = angle_vec3(v, &vec3_unit_z);

	if (angle > 180.0f - MIN)
		glRotatef(180.0, 1.0f, 0.0, 0.0);
	else if (angle > MIN)
		glRotatef(angle, n[0], n[1], n[2]);

	glScalef(1.0f, 1.0f, l);
	glCallList(_vector_list);

	glPopMatrix();
}

void draw_segment_vec4(vec4 *p0, vec4 *p1)
{
	scalar l, angle;
	vec3 n, v;

	v[0] = (*p1)[0] - (*p0)[0];
	v[1] = (*p1)[1] - (*p0)[1];
	v[2] = (*p1)[2] - (*p0)[2];

	l = norm_vec3(&v);

	if (l < MIN)
		return;

	glPushMatrix();

	glTranslatef((*p0)[0], (*p0)[1], (*p0)[2]);

	cross_product_vec3(&vec3_unit_z, &v, &n);
	angle = angle_vec3(&v, &vec3_unit_z);

	if (angle > 180.0f - MIN)
		glRotatef(180, 1.0f, 0.0, 0.0);
	else if (angle > MIN)
		glRotatef(angle, n[0], n[1], n[2]);

	glScalef(1.0f, 1.0f, l);

	glCallList(_vector_list);

	glPopMatrix();
}

void _draw_vec4(vec4 *o, vec4 *v)
{
	scalar l, angle;
	vec3 n;

	l = norm_vec3((vec3 *)v);

	if (l < MIN)
		return;

	glPushMatrix();

	if (o)
		glTranslatef((*o)[0], (*o)[1], (*o)[2]);

	cross_product_vec3(&vec3_unit_z, (vec3 *)v, &n);
	angle = angle_vec3((vec3 *)v, &vec3_unit_z);

	if (angle > 180.0f - MIN)
		glRotatef(180, 1.0f, 0.0, 0.0);
	else if (angle > MIN)
		glRotatef(angle, n[0], n[1], n[2]);

	glScalef(1.0f, 1.0f, l);

	glCallList(_vector_list);

	glPopMatrix();
}


void draw_vec4_color3f(vec4 *o, vec4 *v, float r, float g, float b)
{
	glColor3f(r, g, b);

	_draw_vec4(o, v);
}

void draw_vec4(vec4 *o, vec4 *v)
{
	_draw_vec4(o, v);
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

void draw_init(void)
{
	glPushMatrix();
	glLoadIdentity();

	_cone_list = cone_display_list(0.5f, 1.0f, 20);
	_cylinder_list = cylinder_display_list(0.5f, 1.0f, 20);
	_vector_list = vector_display_list(0.02f, 1.0f, 20);

	glPopMatrix();
}
