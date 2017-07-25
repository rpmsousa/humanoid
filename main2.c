#include <stdio.h>
#include <sys/time.h>
#ifdef USE_GLUT
#include <GL/glut.h>
#else
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
//#include <unistd.h>
#endif
#include <GL/glu.h>
#include <math.h>
#include "linear.h"
#include "model2.h"
#include "physics2.h"
#include "draw.h"

GLuint frame_list;
struct timeval t0;
int pause = 0;
struct timeval tprev;
scalar t = 0.0;

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

void element_velocity_draw(struct element *e, int index)
{
	vec4 *r, *v_local, *v_frame, *v_total;
	vec4 r0;

	local2parent(e, &vec4_null, &r0);

	r = (vec4 *)&e->tr[index];
	v_local = (vec4 *)&e->_tv[index];
	v_frame = (vec4 *)&e->__tv[index];
	v_total = (vec4 *)&e->___tv[index];

	sum(&r0, r, r);

	glColor3f(1.0, 1.0, 0.0);
	draw_vec4(r, v_frame);

	glColor3f(0.0, 1.0, 1.0);
	draw_vec4(r, v_total);

	sum(r, v_frame, r);

	glColor3f(0.0, 0.0, 1.0);
	draw_vec4(r, v_local);
}


void element_draw(struct element *e)
{
	vec4 r0, *r;
	vec4 r1, r2;

	glPushMatrix();

	/* All drawing coordinates are parent coordinates */
	if (e->parent)
		glMultMatrixf(e->parent->_tr);

	/* Basis origin */
	local2parent(e, &vec4_null, &r0);

#if DEBUG_TRANSFORM
	r1[0] = 0.5;
	r1[1] = 0.5;
	r1[2] = 0.5;
	r1[3] = 1.0;

	local2parent(e, &r1, &r1);

	draw_segment_vec4(&r0, &r1);
#endif

	/* Angular velocity vector */
	glColor3f(1.0, 1.0, 1.0); /* white */
	draw_vec4(&r0, &e->a);

	/* basis + basis local velocity */
	glColor3f(1.0, 0.0, 0.0);
	r = (vec4 *)&e->tr[0];
	draw_vec4(&r0, r);

	glColor3f(0.0, 1.0, 0.0);
	r = (vec4 *)&e->tr[4];
	draw_vec4(&r0, r);

	glColor3f(0.0, 0.0, 1.0);
	r = (vec4 *)&e->tr[8];
	draw_vec4(&r0, r);

	element_velocity_draw(e, 0);
	element_velocity_draw(e, 4);
	element_velocity_draw(e, 8);

//	glCallList(frame_list);

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

static void print_fps(void)
{
	struct timeval ti;
	static unsigned int count = 0;

	gettimeofday(&ti, NULL);
	count++;

	if (ti.tv_sec > t0.tv_sec) {
		float dt = (ti.tv_sec - t0.tv_sec) * 1000000.0 + ti.tv_usec - t0.tv_usec;
		printf("fps: %f\n", count * 1000000.0 / dt);
		t0 = ti;
		count = 0;
	}
}

static void display_func(void)
{
	struct timeval tnow;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(cx, cy, cz);

	glRotatef(ty, 0.0, 1.0, 0.0);

	model_draw(&m);

	gettimeofday(&tnow, NULL);

	if (!pause)
		t += (float)(tnow.tv_sec - tprev.tv_sec) + (tnow.tv_usec - tprev.tv_usec) / 1000000.0f;

	tprev = tnow;

	print_fps();

	model_update(&m, t);

#ifdef USE_GLUT
	glutSwapBuffers();
#else
#endif
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

	case ' ':
		if (pause)
			pause = 0;
		else
			pause = 1;

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
//	glEnable(GL_POLYGON_SMOOTH);

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
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static void culling_init(void)
{
	glCullFace(GL_BACK);

	glFrontFace(GL_CCW);

	glEnable(GL_CULL_FACE);
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

static void keypress_event_handler(XKeyEvent *xevent)
{
	printf("%u %c\n", xevent->keycode, (int)XLookupKeysym(xevent, 0));

	keyboard(XLookupKeysym(xevent, 0), xevent->x, xevent->y);
}

static void expose_event_handler(XExposeEvent *xevent)
{
	XWindowAttributes       gwa;

	if (!xevent->count) {
		//XGetWindowAttributes(xevent->display, xevent->window, &gwa);
		//reshape(gwa.width, gwa.height);
		display_func();
		glXSwapBuffers(xevent->display, xevent->window);
	}
}

static void event_handler(Display *display, Window window)
{
	XEvent xevent;

	if (XCheckWindowEvent(display, window, KeyPressMask, &xevent)) {

		switch (xevent.type) {
		case KeyPress:
			keypress_event_handler(&xevent.xkey);
			break;

		default:
			break;
		}
	}
}

#define XPOS	100
#define YPOS	100
#define XSIZE	1024
#define YSIZE	1024

int main(int argc, char *argv[])
{
#ifdef USE_GLUT
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);

	glutInitWindowPosition(XPOS, YPOS);

	glutInitWindowSize(XSIZE, YSIZE);

	glutCreateWindow("Skeleton");

	glutDisplayFunc(display_func);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
#else
	Display *display;
	Window root;
	Window window;
	XVisualInfo *visual;
	GLXContext glcontext;
	Colormap colormap;
	XSetWindowAttributes windowattributes;
	XWindowAttributes       gwa;
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, GLX_ALPHA_SIZE, 8, GLX_STENCIL_SIZE, 8, GLX_ACCUM_GREEN_SIZE, 4, None };

	display = XOpenDisplay(NULL);

	root = DefaultRootWindow(display);

	visual = glXChooseVisual(display, 0, att);

	colormap = XCreateColormap(display, root, visual->visual, AllocNone);

	windowattributes.colormap = colormap;
	windowattributes.event_mask = ExposureMask | KeyPressMask;

	window = XCreateWindow(display, root, XPOS, YPOS, XSIZE, YSIZE, 0, visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask, &windowattributes);

	XMapWindow(display, window);
	XStoreName(display, window, "Skeleton");

	glcontext = glXCreateContext(display, visual, NULL, GL_TRUE);
	glXMakeCurrent(display, window, glcontext);

#endif
	dump_info();

	glShadeModel(GL_SMOOTH);

	drawing_init();

	depth_init();

	lighting_init();

	antialiasing_init();

	culling_init();

	draw_init();

	frame_list = frame_display_list();

	gettimeofday(&t0, NULL);
	tprev = t0;

//	rotate(&m.element[0].tr, 30, 0, 1, 0);

//	rotate(&m.element[1].tr, 30, 0, 1, 0);

//	rotate(&m.element[2].tr, 30, 0, 1, 0);

//	rotate(&m.element[4].tr, 30, 0, 1, 0);

//	rotate(&m.element[5].tr, 30, 0, 1, 0);

	m.element[0].a[0] = 0.0;
	m.element[0].a[1] = 60.0; /* angular velocity about the y axis */
	m.element[0].a[2] = 0.0;

	m.element[1].a[0] = 0.0;
	m.element[1].a[1] = 0.0;
	m.element[1].a[2] = 10; /* angular velocity about the z axis */

	m.element[2].a[0] = 0.0;
	m.element[2].a[1] = 0.0;
	m.element[2].a[2] = 10;

	m.element[4].a[0] = 0.0;

	model_init(&m);

#ifdef USE_GLUT
	glutMainLoop();
#else
	XGetWindowAttributes(display, window, &gwa);
	reshape(gwa.width, gwa.height);

	display_func();
	glXSwapBuffers(display, window);

	while(1) {
		event_handler(display, window);
		display_func();
		glXSwapBuffers(display, window);

		usleep(10000);
	}
#endif

	return 0;
}
