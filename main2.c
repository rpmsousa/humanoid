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
	.elements = 3,
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
			.parent = &m.element[1],
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

	glPushMatrix();

	/* All drawing coordinates are local */
	glMultMatrixf(e->_tr);

	/* local velocity */
	v[0] = e->_tv[0];
	v[1] = e->_tv[1];
	v[2] = e->_tv[2];
	v[3] = e->_tv[3];

	draw_vec4(&vec4_unit_x, &v);

	v[0] = e->_tv[4];
	v[1] = e->_tv[5];
	v[2] = e->_tv[6];
	v[3] = e->_tv[7];

	draw_vec4(&vec4_unit_y, &v);

	v[0] = e->_tv[8];
	v[1] = e->_tv[9];
	v[2] = e->_tv[10];
	v[3] = e->_tv[11];

	draw_vec4(&vec4_unit_z, &v);
#if 0
	v[0] = e->__tv[0];
	v[1] = e->__tv[1];
	v[2] = e->__tv[2];
	v[3] = e->__tv[3];

	draw_vec4(&vec4_unit_x, &v);


	v[0] = e->__tv[4];
	v[1] = e->__tv[5];
	v[2] = e->__tv[6];
	v[3] = e->__tv[7];

	draw_vec4(&vec4_unit_y, &v);

	v[0] = e->__tv[8];
	v[1] = e->__tv[9];
	v[2] = e->__tv[10];
	v[3] = e->__tv[11];

	draw_vec4(&vec4_unit_z, &v);
#endif

	glCallList(frame_list);

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
	m.element[0].a[1] = 0.5; /* angular velocity about the y axis */
	m.element[0].a[2] = 0.0;

	m.element[1].a[0] = 0.0;
	m.element[1].a[1] = 1.0;
	m.element[1].a[2] = 0.0; /* angular velocity about the z axis */

	m.element[2].a[0] = 1.0;
	m.element[2].a[1] = 0.0;
	m.element[2].a[2] = 0.0;

	m.element[4].a[0] = 0.0;

	model_init(&m);

	glutMainLoop();

	return 0;
}
