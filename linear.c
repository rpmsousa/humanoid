#include <stdio.h>
#include <math.h>
#include "linear.h"

vec3 vec3_null = {0.0, 0.0, 0.0};
vec4 vec4_null = {0.0, 0.0, 0.0, 1.0};

vec3 vec3_unit_x = {1.0, 0.0, 0.0};
vec3 vec3_unit_y = {0.0, 1.0, 0.0};
vec3 vec3_unit_z = {0.0, 0.0, 1.0};

vec4 vec4_unit_x = {1.0, 0.0, 0.0, 1.0};
vec4 vec4_unit_y = {0.0, 1.0, 0.0, 1.0};
vec4 vec4_unit_z = {0.0, 0.0, 1.0, 1.0};

mat3 mat3_null = {
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0
};

mat3 mat3_unit = {
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0
};

mat4 mat4_null = {
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0
};

mat4 mat4_unit = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

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
	float sin = sinf((a * M_PI) / 180.0f);
	float cos = cosf((a * M_PI) / 180.0f);

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

/* 1/A = 1 / det(A) * transpose(Cofactor matrix A) */
void invert_mat3(mat3 *m, mat3 *n)
{
	scalar d = det(m);

	if (d == (scalar)0.0)
		return;

	/* Cofactor matrix */
	(*n)[0] =   _det_mat2((*m)[4], (*m)[5], (*m)[7], (*m)[8]);
	(*n)[1] = - _det_mat2((*m)[1], (*m)[2], (*m)[7], (*m)[8]);
	(*n)[2] =   _det_mat2((*m)[1], (*m)[2], (*m)[4], (*m)[5]);

	(*n)[3] = - _det_mat2((*m)[3], (*m)[5], (*m)[6], (*m)[8]);
	(*n)[4] =   _det_mat2((*m)[0], (*m)[2], (*m)[6], (*m)[8]);
	(*n)[5] = - _det_mat2((*m)[0], (*m)[2], (*m)[3], (*m)[5]);

	(*n)[6] =   _det_mat2((*m)[3], (*m)[4], (*m)[6], (*m)[7]);
	(*n)[7] = - _det_mat2((*m)[0], (*m)[1], (*m)[6], (*m)[7]);
	(*n)[8] =   _det_mat2((*m)[0], (*m)[1], (*m)[3], (*m)[4]);

	if (d != (scalar)1.0) {
		d = ((scalar)1.0)/d;
		product(&d, n, n);
	}
}

void invert_mat4(mat4 *m, mat4 *n)
{
	scalar d = det(m);

	if (d == (scalar)0.0)
		return;

	/* Cofactor matrix */
	(*n)[0] =   _det_mat3((*m)[5], (*m)[6], (*m)[7], (*m)[9], (*m)[10], (*m)[11], (*m)[13], (*m)[14], (*m)[15]);
	(*n)[1] = - _det_mat3((*m)[1], (*m)[2], (*m)[3], (*m)[9], (*m)[10], (*m)[11], (*m)[13], (*m)[14], (*m)[15]);
	(*n)[2] =   _det_mat3((*m)[1], (*m)[2], (*m)[3], (*m)[5], (*m)[6],  (*m)[7],  (*m)[13], (*m)[14], (*m)[15]);
	(*n)[3] = - _det_mat3((*m)[1], (*m)[2], (*m)[3], (*m)[5], (*m)[6],  (*m)[7],  (*m)[9],  (*m)[10], (*m)[11]);

	(*n)[4] = - _det_mat3((*m)[4], (*m)[6], (*m)[7], (*m)[8], (*m)[10], (*m)[11], (*m)[12], (*m)[14], (*m)[15]);
	(*n)[5] =   _det_mat3((*m)[0], (*m)[2], (*m)[3], (*m)[8], (*m)[10], (*m)[11], (*m)[12], (*m)[14], (*m)[15]);
	(*n)[6] = - _det_mat3((*m)[0], (*m)[2], (*m)[3], (*m)[4], (*m)[6],  (*m)[7],  (*m)[12], (*m)[14], (*m)[15]);
	(*n)[7] =   _det_mat3((*m)[0], (*m)[2], (*m)[3], (*m)[4], (*m)[6],  (*m)[7],  (*m)[8],  (*m)[10], (*m)[11]);

	(*n)[8]  =   _det_mat3((*m)[4], (*m)[5], (*m)[7], (*m)[8], (*m)[9], (*m)[11], (*m)[12], (*m)[13], (*m)[15]);
	(*n)[9]  = - _det_mat3((*m)[0], (*m)[1], (*m)[3], (*m)[8], (*m)[9], (*m)[11], (*m)[12], (*m)[13], (*m)[15]);
	(*n)[10] =   _det_mat3((*m)[0], (*m)[1], (*m)[3], (*m)[4], (*m)[5],  (*m)[7], (*m)[12], (*m)[13], (*m)[15]);
	(*n)[11] = - _det_mat3((*m)[0], (*m)[1], (*m)[3], (*m)[4], (*m)[5],  (*m)[7], (*m)[8],  (*m)[9], (*m)[11]);

	(*n)[12] = - _det_mat3((*m)[4], (*m)[5], (*m)[6], (*m)[8], (*m)[9], (*m)[10], (*m)[12], (*m)[13], (*m)[14]);
	(*n)[13] =   _det_mat3((*m)[0], (*m)[1], (*m)[2], (*m)[8], (*m)[9], (*m)[10], (*m)[12], (*m)[13], (*m)[14]);
	(*n)[14] = - _det_mat3((*m)[0], (*m)[1], (*m)[2], (*m)[4], (*m)[5],  (*m)[6], (*m)[12], (*m)[13], (*m)[14]);
	(*n)[15] =   _det_mat3((*m)[0], (*m)[1], (*m)[2], (*m)[4], (*m)[5],  (*m)[6], (*m)[8],  (*m)[9], (*m)[10]);

	if (d != (scalar)1.0) {
		d = ((scalar)1.0)/d;
		product(&d, n, n);
	}
}