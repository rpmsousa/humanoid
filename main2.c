#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include "linear.h"
#include "model2.h"
#include "draw.h"

GLuint frame_list;

double cz = -10.0;
double cy = 0.0;
double cx = 0.0;

double ty = 0.0;

struct model m = {
	.elements = 2,
	.element = {
		[0] = {
			.parent = NULL,
			.tr = {1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1},
		},
	
		[1] = {
			.parent = &m.element[0],
			.tr = {1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				5, 0, 0, 1},
		},

		[2] = {
			.parent = &m.element[0],
			.tr = {1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 5, 0, 1},
		},

		[3] = {
			.parent = &m.element[0],
			.tr = {1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 5, 1},
		},

		[4] = {
			.parent = &m.element[2],
			.tr = {1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				5, 0, 0, 1},
		},
		
		[5] = {
			.parent = &m.element[4],
			.tr = {1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				5, 0, 0, 1},
		},
	},
};

void element_print(struct element *b)
{
	printf("%p\n", b);

//	print("position", &b->r);
//	print("absolute position", &b->_r);

	print("transform", &b->tr);
	print("absolute transform", &b->_tr);
	
//	print("velocity", b->vt);
//	print("absolute velocity", b->_vt);
}

scalar t = 0.0;

void element_draw(struct element *e)
{
	vec4 v;
	mat4 _inv_tr;
	vec4 ri, r;

	transpose(&e->_tr, &_inv_tr);
	_inv_tr[3] = 0.0;
	_inv_tr[7] = 0.0;
	_inv_tr[11] = 0.0;
	_inv_tr[15] = 1.0;

	glPushMatrix();

	/* All drawing coordinates are local */
	glMultMatrixf(e->_tr);

	if (e->parent) {
		scalar a = -1;

		/* Element relative position */
		parent2local(e, &vec4_null, &ri); /* parent origin in local coordinates */

		product_c_vec4(&a, &ri, &r);
		sum(&vec4_null, &r, &r);

		draw_vec4(&ri, &r);

		/* Element relative velocity */
		local2parent(e, &vec4_null, &r);
		_velocity_local(e->parent, &r, &v);
		parent2local(e, &v, &v);
		draw_vec4(&vec4_null, &v);
	}

	/* Local velocity */
	_velocity_local(e, &vec4_unit_x, &v);

	draw_vec4(&vec4_unit_x, &v);

	_velocity_local(e, &vec4_unit_y, &v);

	draw_vec4(&vec4_unit_y, &v);

	_velocity_local(e, &vec4_unit_z, &v);

	draw_vec4(&vec4_unit_z, &v);

#if 0
	if (e->parent) {
		product(&e->_tr, &vec4_unit_x, &v); /* parent coordinates */
		_velocity_local(e->parent, &v, &v);
		parent2local(e, &v, &v);
		draw_vec4(&vec4_unit_x, &v);

		product(&e->_tr, &vec4_null, &v); /* parent coordinates */
		_velocity_local(e->parent, &v, &v);
		parent2local(e, &v, &v);
		draw_vec4(&vec4_null, &v);
	}
#if 0
	/* x axis relative velocity */
//	product(&e->tv, &vec4_unit_x, &v);

//	draw(&vec4_unit_x, &v);

	/* x axis absolute velocity */
	velocity(e, &vec4_unit_x, &v);

	product(&_inv_tr, &v, &v);

	draw_vec4(&vec4_unit_x, &v);
#endif

#if 0
	/* y axis relative velocity */
//	product(&e->tv, &vec4_unit_y, &v);

//	draw(&vec4_unit_y, &v);

	/* y axis absolute velocity */
	velocity(e, &vec4_unit_y, &v);

	product(&_inv_tr, &v, &v);

	draw_vec4(&vec4_unit_y, &v);
#endif

#if 0
	/* z axis relative velocity */
//	product(&e->tv, &vec4_unit_z, &v);

//	draw(&vec4_unit_z, &v);

	/* z axis absolute velocity */
	velocity(e, &vec4_unit_z, &v);

	product(&_inv_tr, &v, &v);

	draw_vec4(&vec4_unit_z, &v);
#endif
#endif
	glCallList(frame_list);

//	glScaled(b->dx, b->dy, b->dz);

//	glCallList(cube_list);

	glPopMatrix();
}

static void model_draw(struct model *m)
{
	int i;

	glPushMatrix();

	for (i = 0; i < m->elements; i++)
		element_draw(&m->element[i]);

	glPopMatrix();
}

static void reshape(int w, int h)
{
	float ratio =  w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.01, 100.0);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(cx, cy, cz);

	glRotatef(ty, 0.0, 1.0, 0.0);

	model_draw(&m);

	t += 0.1;

	model_update(&m, t);

	glutSwapBuffers();
}

static void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		cy += 0.1;
		break;

	case 's':
		cy -= 0.1;
		break;

	case 'a':
		cx -= 0.1;
		break;

	case 'd':
		cx += 0.1;
		break;

	case 'q':
		cz -= 0.1;
		break;

	case 'e':
		cz += 0.1;
		break;

	case 'z':
		ty += 5;
		break;

	case 'c':
		ty -= 5;
		break;

	default:
		break;
	}
}

static void lighting_init(void)
{
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[] = {0.0, 0.0, 5.0, 1.0};
//	GLfloat light_position[] = {0.0, 0.0, 1.0, 0.0};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	/* Always keep normal vectors normalized */
	glEnable(GL_NORMALIZE);
}

static void antialiasing_init(void)
{
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
}

static void depth_init(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

static void drawing_init(void)
{
	glLineWidth(3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void dump_info(void)
{
	int val[4];
	float fval;

	printf("%s\n", glGetString(GL_VERSION));
#if 0
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("%d.%d\n", major, minor);
#endif
	glGetIntegerv(GL_RED_BITS, &val[0]);
	glGetIntegerv(GL_GREEN_BITS, &val[1]);
	glGetIntegerv(GL_BLUE_BITS, &val[2]);
	glGetIntegerv(GL_ALPHA_BITS, &val[3]);
	printf("Color buffer bits: %d %d %d %d\n", val[0], val[1], val[2], val[3]);

	glGetIntegerv(GL_DEPTH_BITS, &val[0]);
	printf("Depth buffer bits: %d\n", val[0]);

	glGetIntegerv(GL_STENCIL_BITS, &val[0]);
	printf("Stencil buffer bits: %d\n", val[0]);

	glGetIntegerv(GL_ACCUM_RED_BITS, &val[0]);
	glGetIntegerv(GL_ACCUM_GREEN_BITS, &val[1]);
	glGetIntegerv(GL_ACCUM_BLUE_BITS, &val[2]);
	glGetIntegerv(GL_ACCUM_ALPHA_BITS, &val[3]);
	printf("Accumulator buffer bits: %d %d %d %d\n", val[0], val[1], val[2], val[3]);

	glGetIntegerv(GL_SAMPLE_BUFFERS, &val[0]);
	printf("Sample buffers: %d\n", val[0]);

	glGetIntegerv(GL_SAMPLES, &val[0]);
	printf("Samples: %d\n", val[0]);

	glGetFloatv(GL_SAMPLE_COVERAGE_VALUE, &fval);
	printf("Sample cover value: %f\n", fval);

	printf("%d %d %d %d\n", glIsEnabled(GL_MULTISAMPLE),
			glIsEnabled(GL_SAMPLE_ALPHA_TO_ONE),
			glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE),
			glIsEnabled(GL_SAMPLE_COVERAGE));
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);

	glutInitWindowPosition(100,100);

	glutInitWindowSize(1024, 1024);

	glutCreateWindow("Skeleton");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);

	dump_info();

	glShadeModel(GL_SMOOTH);

	drawing_init();

	depth_init();

	lighting_init();

	antialiasing_init();

	frame_list = frame_display_list();

//	rotate(&m.element[0].tr, 30, 0, 1, 0);

//	rotate(&m.element[1].tr, 30, 0, 1, 0);

//	rotate(&m.element[2].tr, 30, 0, 1, 0);

//	rotate(&m.element[4].tr, 30, 0, 1, 0);

//	rotate(&m.element[5].tr, 30, 0, 1, 0);

	m.element[0].a[0] = 0.0;
	m.element[0].a[1] = 1.1; /* angular velocity about the y axis */

	m.element[1].a[2] = 1.2; /* angular velocity about the x axis */
	m.element[2].a[0] = 0.0;
	m.element[4].a[0] = 0.0;

	model_init(&m);

	glutMainLoop();

	return 0;
}
