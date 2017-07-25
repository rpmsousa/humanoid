#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "linear.h"


void test1(vec3 *v, vec4 *u, mat3 *m, mat4 *o)
{
	print("v3", v);
	print("v4", u);
	print("m3", m);
	print("m4", o);
}

void transform_test(void)
{
	mat4 l;
	vec3 tv = {1, 2, 3};
	vec3 rv = {3, 4, 5};
	float angle = 20;

	printf("%s\n", __func__);

	identity(&l);

	translate(&l, tv[0], tv[1], tv[2]);

	print("translation", &l);

	identity(&l);

	rotate(&l, angle, rv[0], rv[1], rv[2]);

	print("rotation", &l);

	identity(&l);
	translate(&l, tv[0], tv[1], tv[2]);
	rotate(&l, angle, rv[0], rv[1], rv[2]);
	rotate(&l, angle, rv[0], rv[1], rv[2]);

	print("translate + 2 rotation", &l);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(tv[0], tv[1], tv[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX, l);
	print("fltranslation", &l);

	glLoadIdentity();

	glRotatef(angle, rv[0], rv[1], rv[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX, l);
	print("flrotation", &l);

	glLoadIdentity();
	glRotatef(angle, rv[0], rv[1], rv[2]);
	glRotatef(angle, rv[0], rv[1], rv[2]);
	glTranslatef(tv[0], tv[1], tv[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX, l);
	print("fl translate + 2 fl rotation", &l);

	glPopMatrix();
}

void invert_test(void)
{
	float b = M_PI/6.5;

	mat3 l = {
		cos(b), 0, -sin(b),
		0, 1, 0,
		sin(b), 0, cos(b)
	};

	mat3 m;

	printf("%s\n", __func__);

	print("rotation", &l);

	transpose(&l, &m);

	print("transpose", &m);

	invert_mat3(&l, &m);

	print("invert", &m);

	product(&l, &m, &m);

	print("identity", &m);
}

void linear_test(void)
{
	float a = 3.14/4;
	float b = 3.14/4;

	mat4 l = {
		cos(b), 0, -sin(b), 0,
		0, 1, 0, 0,
		sin(b), 0, cos(b), 0,
		0, 0, 0, 1
	};

	mat4 m = {
		1, 0, 0, 0,
		0, cos(a), sin(a), 0,
		0, -sin(a), cos(a), 0,
		0, 0, 0, 1
	};

	mat4 n = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3, 3, 3, 1
	};

	mat4 o;
	mat3 p = {1, 2, 3, 4};

	vec3 v = {1, 2, 3};
	vec4 u = {1, 2, 3};
	mat3 t;
	int i;

	invert_test();
	transform_test();

	print("vector3", &v);
	print("vector4", &u);
	print("mat3", &p);
	print("mat4", &n);

	test1(&v, &u, &p, &n);

	mat4_2_mat3(&l, &t);
	print("l", &t);
	printf("l: %f\n", det(&t));

	mat4_2_mat3(&m, &t);
	print("m", &t);
	printf("m: %f\n", det(&t));

	mat4_2_mat3(&n, &t);
	print("n", &t);
	printf("n: %f\n", det(&t));

	product(&n, &m, &o);
	print("n", &n);

	mat4_2_mat3(&n, &t);
	printf("o: %f\n", det(&t));

	for (i = 0; i < 100; i++) {
		product(&m, &o, &o);

		print("product", &o);

		mat4_2_mat3(&o, &t);
		printf("%f\n", det(&t));

		product(&n, &o, &o);

		print("product", &o);

		mat4_2_mat3(&o, &t);
		printf("%f\n", det(&t));

		product(&l, &o, &o);

		print("product", &o);

		mat4_2_mat3(&o, &t);
		printf("%f\n", det(&t));
	}
}

int main(int argc, char *argv[])
{
	vec4 v = {1, 2, 3, 9 };
	vec3 u = {6, 7, 8 };
	vec4 w;
	float c = 2.34;

	glutInit(&argc, argv);

	linear_test();

	return 0;
}
