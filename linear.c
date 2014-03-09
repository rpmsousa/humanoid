#include <stdio.h>
#include <math.h>
#include "linear.h"

void print_vec3(const char *name, vec3 *v)
{
	if (name)
		printf("%s:\n", name);

	printf("% 3f % 3f % 3f\n", (*v)[0], (*v)[1], (*v)[2]);
}

void print_vec4(const char *name, vec4 *v)
{
	if (name)
		printf("%s:\n", name);

	printf("% 3f % 3f % 3f % 3f\n", (*v)[0], (*v)[1], (*v)[2], (*v)[3]);
}

void print_mat3(const char *name, mat3 *m)
{
	if (name)
		printf("%s:\n", name);

	printf("% 3f % 3f % 3f\n", (*m)[0], (*m)[3], (*m)[6]);
	printf("% 3f % 3f % 3f\n", (*m)[1], (*m)[4], (*m)[7]);
	printf("% 3f % 3f % 3f\n", (*m)[2], (*m)[5], (*m)[8]);
}

void print_mat4(const char *name, mat4 *m)
{
	if (name)
		printf("%s:\n", name);

	printf("% 3f % 3f % 3f % 3f\n", (*m)[0], (*m)[4], (*m)[8], (*m)[12]);
	printf("% 3f % 3f % 3f % 3f\n", (*m)[1], (*m)[5], (*m)[9], (*m)[13]);
	printf("% 3f % 3f % 3f % 3f\n", (*m)[2], (*m)[6], (*m)[10], (*m)[14]);
	printf("% 3f % 3f % 3f % 3f\n", (*m)[3], (*m)[7], (*m)[11], (*m)[15]);
}

void identity_mat3(mat3 *m)
{
	(*m)[0] = 1.0; (*m)[4] = 0.0; (*m)[8] = 0.0;
	(*m)[1] = 0.0; (*m)[5] = 1.0; (*m)[9] = 0.0;
	(*m)[2] = 0.0; (*m)[6] = 0.0; (*m)[10] = 1.0;
}

void identity_mat4(mat4 *m)
{
	(*m)[0] = 1.0; (*m)[4] = 0.0; (*m)[8] = 0.0; (*m)[12] = 0.0;
	(*m)[1] = 0.0; (*m)[5] = 1.0; (*m)[9] = 0.0; (*m)[13] = 0.0;
	(*m)[2] = 0.0; (*m)[6] = 0.0; (*m)[10] = 1.0; (*m)[14] = 0.0;
	(*m)[3] = 0.0; (*m)[7] = 0.0; (*m)[11] = 0.0; (*m)[15] = 1.0;
}

void normalize_vec3(vec3 *v)
{
	scalar n;

	/* normalize v */
	product(v, v, &n);
	if ((n != (scalar) 0.0) && (n != (scalar)1.0)) {
		n = (scalar)1.0 / sqrtf(n);
		product(&n, v, v);
	}
}

void normalize_vec4(vec4 *v)
{
	scalar n;

	/* normalize v */
	product(v, v, &n);
	if ((n != (scalar) 0.0) && (n != (scalar)1.0)) {
		n = (scalar)1.0 / sqrtf(n);
		product(&n, v, v);
	}
}

void rotation_mat3(mat3 *m, scalar a, scalar u0, scalar u1, scalar u2)
{
	vec3 u = {u0, u1, u2};
	scalar sin = sinf(a * M_PI / 180.0f);
	scalar cos = cosf(a * M_PI / 180.0f);

	normalize_vec3(&u);

	(*m)[0] = cos + u[0] * u[0] * (1.0f - cos);
	(*m)[1] = u[1] * u[0] * (1.0f - cos) + u[2] * sin;
	(*m)[2] = u[2] * u[0] * (1.0f - cos) - u[1] * sin;

	(*m)[3] = u[0] * u[1] * (1.0f - cos) - u[2] * sin;
	(*m)[4] = cos + u[1] * u[1] * (1.0f - cos);
	(*m)[5] = u[2] * u[1] * (1.0f - cos) + u[0] * sin;

	(*m)[6] = u[0] * u[2] * (1.0f - cos) + u[1] * sin;
	(*m)[7] = u[1] * u[2] * (1.0f - cos) - u[0] * sin;
	(*m)[8] = cos + u[2] * u[2] * (1.0f - cos);
}

void rotation_mat4(mat4 *m, scalar a, scalar u0, scalar u1, scalar u2)
{
	vec3 u = {u0, u1, u2};
	float sin = sinf(a * M_PI / 180.0f);
	float cos = cosf(a * M_PI / 180.0f);

	normalize_vec3(&u);

	(*m)[0] = cos + u[0] * u[0] * (1.0f - cos);
	(*m)[1] = u[1] * u[0] * (1.0f - cos) + u[2] * sin;
	(*m)[2] = u[2] * u[0] * (1.0f - cos) - u[1] * sin;
	(*m)[3] = 0.0;

	(*m)[4] = u[0] * u[1] * (1.0f - cos) - u[2] * sin;
	(*m)[5] = cos + u[1] * u[1] * (1.0f - cos);
	(*m)[6] = u[2] * u[1] * (1.0f - cos) + u[0] * sin;
	(*m)[7] = 0.0;

	(*m)[8] = u[0] * u[2] * (1.0f - cos) + u[1] * sin;
	(*m)[9] = u[1] * u[2] * (1.0f - cos) - u[0] * sin;
	(*m)[10] = cos + u[2] * u[2] * (1.0f - cos);
	(*m)[11] = 0.0;

	(*m)[12] = 0.0;
	(*m)[13] = 0.0;
	(*m)[14] = 0.0;
	(*m)[15] = 1.0;
}

void rotate(mat4 *m, scalar a, scalar u0, scalar u1, scalar u2)
{
	mat3 o;

	rotation_mat3(&o, a, u0, u1, u2);

	product(&o, m, m);
}

void invert_mat3(mat3 *m, mat3 *n)
{
	scalar d = det(m);

	if (d == (scalar)0.0)
		return;

	(*n)[0] =   (*m)[4] * (*m)[8] - (*m)[5] * (*m)[7];
	(*n)[1] =  - (*m)[1] * (*m)[8] + (*m)[2] * (*m)[7];
	(*n)[2] =   (*m)[1] * (*m)[5] - (*m)[2] * (*m)[4];

	(*n)[3] = - (*m)[3] * (*m)[8] + (*m)[5] * (*m)[6];
	(*n)[4] =   (*m)[0] * (*m)[8] - (*m)[2] * (*m)[6];
	(*n)[5] = - (*m)[0] * (*m)[5] + (*m)[2] * (*m)[3];

	(*n)[6] =   (*m)[3] * (*m)[7] - (*m)[4] * (*m)[6];
	(*n)[7] = - (*m)[0] * (*m)[7] + (*m)[1] * (*m)[6];
	(*n)[8] =   (*m)[0] * (*m)[4] - (*m)[1] * (*m)[3];

	if (d != (scalar)1.0) {
		d = ((scalar)1.0)/d;
		product(&d, n, n);
	}
}

void test1(vec3 *v, vec4 *u, mat3 *m, mat4 *o)
{
	print("v3", v);
	print("v4", u);
	print("m3", m);
	print("m4", o);
}

#include <GL/glu.h>
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

#if 0
int main(int argc, char *argv[])
{
	linear_test();

	return 0;
}
#endif