#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include "linear.h"
#include "model.h"
#include "movement.h"
#include "draw.h"

/*
(x, y, z)
(r, t, p)
r = sqrt(x*x + y*y + z*z);
cos t = z/r;
cos p = x/sqrt(x*x + y*y);
tan p = y/x;

z = r * cos(t);
y = r * sin(t) * sin(p);
x = r * sin(t) * cos(p);
*/

double cz = -10.0;
double cy = 0.0;
double cx = 0.0;

double tz = 0.0;
double ty = 0.0;
double tx = 0.0;

GLuint cube_list;
GLuint frame_list;

void bone_draw(struct bone *b)
{
	glPushMatrix();

	glMultMatrixf(b->s.t_abs);

	glScaled(b->dx, b->dy, b->dz);

	glCallList(cube_list);

	glPopMatrix();
}

static void model_draw(struct model *m)
{
	int i;

	glPushMatrix();

	model_update_shape(m);

	/* translate to center of mass coordinates */
	glTranslatef(m->pos[0], m->pos[1], m->pos[2]);

	glRotatef(ty, 0.0, 1.0, 0.0);

	glPushMatrix();

	/* rotate about the center of mass */
	glMultMatrixf(m->angular_pos);

	glTranslatef(-m->cm[0], -m->cm[1], -m->cm[2]);

	for (i = 0; i < m->bones; i++)
		bone_draw(&m->bone[i]);

	glPopMatrix();

	glColor3f(1, 0, 0);
	vector_draw(&m->momentum);

	glColor3f(0, 1, 0);
	vector_draw(&m->angular_velocity);

	glColor3f(0, 0, 1);
	vector_draw(&m->angular_momentum);

	model_movement(m);

	glPopMatrix();
}


void cube_draw(vec3 p, float w, float h)
{
	glPushMatrix();

	glTranslatef(p[0], p[1], p[2]);

	glTranslatef(-w/2.0, -h/2.0, -w/2.0);

	glScaled(w, h, w);

	glCallList(cube_list);

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
//	joint_adjust_random(&model_bulky);
	joint_adjust_random(&model_sticky);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(cx, cy, cz);

	model_draw(&model_bulky);

	model_draw(&model_sticky);

//	model_draw(&model_simple);

	glutSwapBuffers();
}

static void keyboard (unsigned char key, int x, int y)
{
	struct model *m = &model_bulky;
	float d_arm[3] = {0, 0, 0};
	float d_leg[3] = {0, 0, 0};

	switch (key) {
	case 'y':
		d_arm[0] = 1.0;
		break;

	case 'u':
		d_arm[0] = -1.0;
		break;

	case 'h':
		d_arm[1] = 5.0;
		break;

	case 'j':
		d_arm[1] = -5.0;
		break;

	case 'n':
		d_arm[2] = 5.0;
		break;

	case 'm':
		d_arm[2] = -5.0;
		break;

	case 'i':
		d_leg[0] = 1.0;
		break;

	case 'o':
		d_leg[0] = -1.0;
		break;

	case 'k':
		d_leg[1] = 5.0;
		break;

	case 'l':
		d_leg[1] = -5.0;
		break;

	case ',':
		d_leg[2] = 5.0;
		break;

	case '.':
		d_leg[2] = -5.0;
		break;

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

	case 'r':
		ty += 5;
		break;

	case 't':
		ty -= 5;
		break;

	default:
		break;
	}

	joint_adjust(&m->bone[LEFT_ARM], d_arm[0], d_arm[1], d_arm[2]);
	joint_adjust(&m->bone[RIGHT_ARM], d_arm[0], d_arm[1], d_arm[2]);

	joint_adjust(&m->bone[LEFT_LEG], d_leg[0], d_leg[1], d_leg[2]);
	joint_adjust(&m->bone[RIGHT_LEG], d_leg[0], d_leg[1], d_leg[2]);
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
	glLineWidth(1);

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

//	linear_test();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);

	dump_info();

	model_init(&model_bulky, -1.0);
	model_init(&model_sticky, 1.0);
	model_init(&model_simple, 3.0);

	glShadeModel(GL_SMOOTH);

//	cube_list = cube_display_list();
	cube_list = cylinder_display_list(0.5, 1.0, 60);
	frame_list = frame_display_list();

	drawing_init();

	depth_init();

	lighting_init();

	antialiasing_init();

	glutMainLoop();

	return 0;
}
