#ifndef _LINEAR_H_
#define _LINEAR_H_

#include <stdio.h>
#include <math.h>

/*
 a_i,j, i=row index, j=comlumn index. a_mxn, m rows x n columns

 Orthogonal Matrice:

 A

 Trans(A) = Invert(A)

 Trans(A)A = I

 det(A) = +- 1
*/
extern void choice_error(void);

#define FLOAT float

typedef FLOAT scalar;
typedef FLOAT vec2[2];
typedef FLOAT vec3[3];
typedef FLOAT vec4[4];

typedef FLOAT mat2[4];
typedef FLOAT mat3[9];
typedef FLOAT mat4[16];

extern vec3 vec3_null;
extern vec4 vec4_null;
extern vec3 vec3_unit_x;
extern vec3 vec3_unit_y;
extern vec3 vec3_unit_z;
extern vec4 vec4_unit_x;
extern vec4 vec4_unit_y;
extern vec4 vec4_unit_z;
extern mat3 mat3_null;
extern mat3 mat3_unit;
extern mat4 mat4_null;
extern mat4 mat4_unit;

void print_vec3(const char *name, vec3 *v);
void print_vec4(const char *name, vec4 *v);
void print_mat3(const char *name, mat3 *m);
void print_mat4(const char *name, mat4 *m);

#define print(name, m)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), vec3 *), print_vec3(name, (void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), vec4 *), print_vec4(name, (void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *), print_mat3(name, (void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat4 *), print_mat4(name, (void *)m),	\
		choice_error()))))

void identity_mat3(mat3 *m);
void identity_mat4(mat4 *m);

#define identity(m)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *), identity_mat3((void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat4 *), identity_mat4((void *)m),	\
		choice_error()))

static inline void mat4_2_mat3(mat4 *m, mat3 *n)
{
	(*n)[0] = (*m)[0];
	(*n)[1] = (*m)[1];
	(*n)[2] = (*m)[2];

	(*n)[3] = (*m)[4];
	(*n)[4] = (*m)[5];
	(*n)[5] = (*m)[6];

	(*n)[6] = (*m)[8];
	(*n)[7] = (*m)[9];
	(*n)[8] = (*m)[10];
}

static inline void assign_vec3(vec3 *u, vec3 *v)
{
	(*v)[0] = (*u)[0];
	(*v)[1] = (*u)[1];
	(*v)[2] = (*u)[2];
}

static inline void assign_vec4(vec4 *u, vec4 *v)
{
	(*v)[0] = (*u)[0];
	(*v)[1] = (*u)[1];
	(*v)[2] = (*u)[2];
	(*v)[3] = (*u)[3];
}

static inline void assign_mat3(mat3 *m, mat3 *n)
{
	(*n)[0] = (*m)[0];
	(*n)[1] = (*m)[1];
	(*n)[2] = (*m)[2];

	(*n)[3] = (*m)[3];
	(*n)[4] = (*m)[4];
	(*n)[5] = (*m)[5];

	(*n)[6] = (*m)[6];
	(*n)[7] = (*m)[7];
	(*n)[8] = (*m)[8];
}

static inline void assign_mat4(mat4 *m, mat4 *n)
{
	(*n)[0] = (*m)[0];
	(*n)[1] = (*m)[1];
	(*n)[2] = (*m)[2];
	(*n)[3] = (*m)[3];

	(*n)[4] = (*m)[4];
	(*n)[5] = (*m)[5];
	(*n)[6] = (*m)[6];
	(*n)[7] = (*m)[7];

	(*n)[8] = (*m)[8];
	(*n)[9] = (*m)[9];
	(*n)[10] = (*m)[10];
	(*n)[11] = (*m)[11];

	(*n)[12] = (*m)[12];
	(*n)[13] = (*m)[13];
	(*n)[14] = (*m)[14];
	(*n)[15] = (*m)[15];
}

#define assign(m, n)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), vec3 *), assign_vec3((void *)m, (void *)n),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), vec4 *), assign_vec4((void *)m, (void *)n),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *), assign_mat3((void *)m, (void *)n),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat4 *), assign_mat4((void *)m, (void *)n),	\
		choice_error()))))


static inline void sum_vec3(vec3 *u, vec3 *v, vec3 *w)
{
	(*w)[0] = (*u)[0] + (*v)[0];
	(*w)[1] = (*u)[1] + (*v)[1];
	(*w)[2] = (*u)[2] + (*v)[2];
}

static inline void sum_vec4(vec4 *u, vec4 *v, vec4 *w)
{
	(*w)[0] = (*u)[0] + (*v)[0];
	(*w)[1] = (*u)[1] + (*v)[1];
	(*w)[2] = (*u)[2] + (*v)[2];
	(*w)[3] = (*u)[3] + (*v)[3];
}

static inline void sum_mat3(mat3 *m, mat3 *n, mat3 *l)
{
	(*l)[0] = (*m)[0] + (*n)[0];
	(*l)[1] = (*m)[1] + (*n)[1];
	(*l)[2] = (*m)[2] + (*n)[2];

	(*l)[3] = (*m)[3] + (*n)[3];
	(*l)[4] = (*m)[4] + (*n)[4];
	(*l)[5] = (*m)[5] + (*n)[5];

	(*l)[6] = (*m)[6] + (*n)[6];
	(*l)[7] = (*m)[7] + (*n)[7];
	(*l)[8] = (*m)[8] + (*n)[8];
}

static inline void sum_mat4(mat4 *m, mat4 *n, mat4 *l)
{
	(*l)[0] = (*m)[0] + (*n)[0];
	(*l)[1] = (*m)[1] + (*n)[1];
	(*l)[2] = (*m)[2] + (*n)[2];
	(*l)[3] = (*m)[3] + (*n)[3];

	(*l)[4] = (*m)[4] + (*n)[4];
	(*l)[5] = (*m)[5] + (*n)[5];
	(*l)[6] = (*m)[6] + (*n)[6];
	(*l)[7] = (*m)[7] + (*n)[7];

	(*l)[8] = (*m)[8] + (*n)[8];
	(*l)[9] = (*m)[9] + (*n)[9];
	(*l)[10] = (*m)[10] + (*n)[10];
	(*l)[11] = (*m)[11] + (*n)[11];

	(*l)[12] = (*m)[12] + (*n)[12];
	(*l)[13] = (*m)[13] + (*n)[13];
	(*l)[14] = (*m)[14] + (*n)[14];
	(*l)[15] = (*m)[15] + (*n)[15];
}

#define sum(m, n, o)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), vec3 *), sum_vec3((void *)m, (void *)n, (void *)o),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), vec4 *), sum_vec4((void *)m, (void *)n, (void *)o),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *), sum_mat3((void *)m, (void *)n, (void *)o),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat4 *), sum_mat4((void *)m, (void *)n, (void *)o),	\
	choice_error()))))

static inline void product_mat3(mat3 *m, mat3 *n, mat3 *l)
{
	mat3 k;

	k[0] = (*m)[0] * (*n)[0] + (*m)[3] * (*n)[1] + (*m)[6] * (*n)[2];
	k[1] = (*m)[1] * (*n)[0] + (*m)[4] * (*n)[1] + (*m)[7] * (*n)[2];
	k[2] = (*m)[2] * (*n)[0] + (*m)[5] * (*n)[1] + (*m)[8] * (*n)[2];

	k[3] = (*m)[0] * (*n)[3] + (*m)[3] * (*n)[4] + (*m)[6] * (*n)[5];
	k[4] = (*m)[1] * (*n)[3] + (*m)[4] * (*n)[4] + (*m)[7] * (*n)[5];
	k[5] = (*m)[2] * (*n)[3] + (*m)[5] * (*n)[4] + (*m)[8] * (*n)[5];

	k[6] = (*m)[0] * (*n)[6] + (*m)[3] * (*n)[7] + (*m)[6] * (*n)[8];
	k[7] = (*m)[1] * (*n)[6] + (*m)[4] * (*n)[7] + (*m)[7] * (*n)[8];
	k[8] = (*m)[2] * (*n)[6] + (*m)[5] * (*n)[7] + (*m)[8] * (*n)[8];

	assign(&k, l);
}

static inline void product_mat3_mat4(mat3 *m, mat4 *n, mat4 *l)
{
	mat4 k;

	k[0] = (*m)[0] * (*n)[0] + (*m)[3] * (*n)[1] + (*m)[6] * (*n)[2];
	k[1] = (*m)[1] * (*n)[0] + (*m)[4] * (*n)[1] + (*m)[7] * (*n)[2];
	k[2] = (*m)[2] * (*n)[0] + (*m)[5] * (*n)[1] + (*m)[8] * (*n)[2];
	k[3] = (*n)[3];

	k[4] = (*m)[0] * (*n)[4] + (*m)[3] * (*n)[5] + (*m)[6] * (*n)[6];
	k[5] = (*m)[1] * (*n)[4] + (*m)[4] * (*n)[5] + (*m)[7] * (*n)[6];
	k[6] = (*m)[2] * (*n)[4] + (*m)[5] * (*n)[5] + (*m)[8] * (*n)[6];
	k[7] = (*n)[7];

	k[8] = (*m)[0] * (*n)[8] + (*m)[3] * (*n)[9] + (*m)[6] * (*n)[10];
	k[9] = (*m)[1] * (*n)[8] + (*m)[4] * (*n)[9] + (*m)[7] * (*n)[10];
	k[10] = (*m)[2] * (*n)[8] + (*m)[5] * (*n)[9] + (*m)[8] * (*n)[10];
	k[11] = (*n)[11];

	k[12] = (*m)[0] * (*n)[12] + (*m)[3] * (*n)[13] + (*m)[6] * (*n)[14];
	k[13] = (*m)[1] * (*n)[12] + (*m)[4] * (*n)[13] + (*m)[7] * (*n)[14];
	k[14] = (*m)[2] * (*n)[12] + (*m)[5] * (*n)[13] + (*m)[8] * (*n)[14];
	k[15] = (*n)[15];

	assign(&k, l);
}

static inline void product_mat4(mat4 *m, mat4 *n, mat4 *l)
{
	mat4 k;

	k[0] = (*m)[0] * (*n)[0] + (*m)[4] * (*n)[1] + (*m)[8] * (*n)[2] + (*m)[12] * (*n)[3];
	k[1] = (*m)[1] * (*n)[0] + (*m)[5] * (*n)[1] + (*m)[9] * (*n)[2] + (*m)[13] * (*n)[3];
	k[2] = (*m)[2] * (*n)[0] + (*m)[6] * (*n)[1] + (*m)[10] * (*n)[2] + (*m)[14] * (*n)[3];
	k[3] = (*m)[3] * (*n)[0] + (*m)[7] * (*n)[1] + (*m)[11] * (*n)[2] + (*m)[15] * (*n)[3];

	k[4] = (*m)[0] * (*n)[4] + (*m)[4] * (*n)[5] + (*m)[8] * (*n)[6] + (*m)[12] * (*n)[7];
	k[5] = (*m)[1] * (*n)[4] + (*m)[5] * (*n)[5] + (*m)[9] * (*n)[6] + (*m)[13] * (*n)[7];
	k[6] = (*m)[2] * (*n)[4] + (*m)[6] * (*n)[5] + (*m)[10] * (*n)[6] + (*m)[14] * (*n)[7];
	k[7] = (*m)[3] * (*n)[4] + (*m)[7] * (*n)[5] + (*m)[11] * (*n)[6] + (*m)[15] * (*n)[7];

	k[8] = (*m)[0] * (*n)[8] + (*m)[4] * (*n)[9] + (*m)[8] * (*n)[10] + (*m)[12] * (*n)[11];
	k[9] = (*m)[1] * (*n)[8] + (*m)[5] * (*n)[9] + (*m)[9] * (*n)[10] + (*m)[13] * (*n)[11];
	k[10] = (*m)[2] * (*n)[8] + (*m)[6] * (*n)[9] + (*m)[10] * (*n)[10] + (*m)[14] * (*n)[11];
	k[11] = (*m)[3] * (*n)[8] + (*m)[7] * (*n)[9] + (*m)[11] * (*n)[10] + (*m)[15] * (*n)[11];

	k[12] = (*m)[0] * (*n)[12] + (*m)[4] * (*n)[13] + (*m)[8] * (*n)[14] + (*m)[12] * (*n)[15];
	k[13] = (*m)[1] * (*n)[12] + (*m)[5] * (*n)[13] + (*m)[9] * (*n)[14] + (*m)[13] * (*n)[15];
	k[14] = (*m)[2] * (*n)[12] + (*m)[6] * (*n)[13] + (*m)[10] * (*n)[14] + (*m)[14] * (*n)[15];
	k[15] = (*m)[3] * (*n)[12] + (*m)[7] * (*n)[13] + (*m)[11] * (*n)[14] + (*m)[15] * (*n)[15];

	assign(&k, l);
}

static inline void product_mat4_3(mat4 *m, mat4 *n, mat4 *l)
{
	mat4 k;

	k[0] = (*m)[0] * (*n)[0] + (*m)[4] * (*n)[1] + (*m)[8] * (*n)[2] + (*m)[12] * (*n)[3];
	k[1] = (*m)[1] * (*n)[0] + (*m)[5] * (*n)[1] + (*m)[9] * (*n)[2] + (*m)[13] * (*n)[3];
	k[2] = (*m)[2] * (*n)[0] + (*m)[6] * (*n)[1] + (*m)[10] * (*n)[2] + (*m)[14] * (*n)[3];
	k[3] = (*m)[3] * (*n)[0] + (*m)[7] * (*n)[1] + (*m)[11] * (*n)[2] + (*m)[15] * (*n)[3];

	k[4] = (*m)[0] * (*n)[4] + (*m)[4] * (*n)[5] + (*m)[8] * (*n)[6] + (*m)[12] * (*n)[7];
	k[5] = (*m)[1] * (*n)[4] + (*m)[5] * (*n)[5] + (*m)[9] * (*n)[6] + (*m)[13] * (*n)[7];
	k[6] = (*m)[2] * (*n)[4] + (*m)[6] * (*n)[5] + (*m)[10] * (*n)[6] + (*m)[14] * (*n)[7];
	k[7] = (*m)[3] * (*n)[4] + (*m)[7] * (*n)[5] + (*m)[11] * (*n)[6] + (*m)[15] * (*n)[7];

	k[8] = (*m)[0] * (*n)[8] + (*m)[4] * (*n)[9] + (*m)[8] * (*n)[10] + (*m)[12] * (*n)[11];
	k[9] = (*m)[1] * (*n)[8] + (*m)[5] * (*n)[9] + (*m)[9] * (*n)[10] + (*m)[13] * (*n)[11];
	k[10] = (*m)[2] * (*n)[8] + (*m)[6] * (*n)[9] + (*m)[10] * (*n)[10] + (*m)[14] * (*n)[11];
	k[11] = (*m)[3] * (*n)[8] + (*m)[7] * (*n)[9] + (*m)[11] * (*n)[10] + (*m)[15] * (*n)[11];

	k[12] = (*n)[12];
	k[13] = (*n)[13];
	k[14] = (*n)[14];
	k[15] = (*n)[15];

	assign(&k, l);
}

static inline void product_mat3_vec3(mat3 *m, vec3 *u, vec3 *v)
{
	vec3 w;

	w[0] = (*m)[0] * (*u)[0] + (*m)[3] * (*u)[1] + (*m)[6] * (*u)[2];
	w[1] = (*m)[1] * (*u)[0] + (*m)[4] * (*u)[1] + (*m)[7] * (*u)[2];
	w[2] = (*m)[2] * (*u)[0] + (*m)[5] * (*u)[1] + (*m)[8] * (*u)[2];

	assign(&w, v);
}

static inline void product_mat4_vec4(mat4 *m, vec4 *u, vec4 *v)
{
	vec4 w;

	w[0] = (*m)[0] * (*u)[0] + (*m)[4] * (*u)[1] + (*m)[8] * (*u)[2] + (*m)[12] * (*u)[3];
	w[1] = (*m)[1] * (*u)[0] + (*m)[5] * (*u)[1] + (*m)[9] * (*u)[2] + (*m)[13] * (*u)[3];
	w[2] = (*m)[2] * (*u)[0] + (*m)[6] * (*u)[1] + (*m)[10] * (*u)[2] + (*m)[14] * (*u)[3];
	w[3] = (*m)[3] * (*u)[0] + (*m)[7] * (*u)[1] + (*m)[11] * (*u)[2] + (*m)[15] * (*u)[3];

	assign(&w, v);
}

static inline void product_mat4_vec3(mat4 *m, vec3 *u, vec3 *v)
{
	vec3 w;


	w[0] = (*m)[0] * (*u)[0] + (*m)[4] * (*u)[1] + (*m)[8] * (*u)[2] + (*m)[12];
	w[1] = (*m)[1] * (*u)[0] + (*m)[5] * (*u)[1] + (*m)[9] * (*u)[2] + (*m)[13];
	w[2] = (*m)[2] * (*u)[0] + (*m)[6] * (*u)[1] + (*m)[10] * (*u)[2] + (*m)[14];

	assign(&w, v);
}

static inline void product_c_vec3(scalar *c, vec3 *u, vec3 *v)
{
	(*v)[0] = *c * (*u)[0];
	(*v)[1] = *c * (*u)[1];
	(*v)[2] = *c * (*u)[2];
}

static inline void product_c_vec4(scalar *c, vec4 *u, vec4 *v)
{
	(*v)[0] = *c * (*u)[0];
	(*v)[1] = *c * (*u)[1];
	(*v)[2] = *c * (*u)[2];
	(*v)[3] = *c * (*u)[3];
}

static inline void product_c_mat3(scalar *c, mat3 *m, mat3 *n)
{
	(*n)[0] = *c * (*m)[0];
	(*n)[1] = *c * (*m)[1];
	(*n)[2] = *c * (*m)[2];

	(*n)[3] = *c * (*m)[3];
	(*n)[4] = *c * (*m)[4];
	(*n)[5] = *c * (*m)[5];

	(*n)[6] = *c * (*m)[6];
	(*n)[7] = *c * (*m)[7];
	(*n)[8] = *c * (*m)[8];
}

static inline void product_c_mat4(scalar *c, mat4 *m, mat4 *n)
{
	(*n)[0] = *c * (*m)[0];
	(*n)[1] = *c * (*m)[1];
	(*n)[2] = *c * (*m)[2];
	(*n)[3] = *c * (*m)[3];

	(*n)[4] = *c * (*m)[4];
	(*n)[5] = *c * (*m)[5];
	(*n)[6] = *c * (*m)[6];
	(*n)[7] = *c * (*m)[7];

	(*n)[8] = *c * (*m)[8];
	(*n)[9] = *c * (*m)[9];
	(*n)[10] = *c * (*m)[10];
	(*n)[11] = *c * (*m)[11];

	(*n)[12] = *c * (*m)[12];
	(*n)[13] = *c * (*m)[13];
	(*n)[14] = *c * (*m)[14];
	(*n)[15] = *c * (*m)[15];
}

static inline void product_vec3(vec3 *u, vec3 *v, scalar *c)
{
	*c = (*u)[0] * (*v)[0] + (*u)[1] * (*v)[1] + (*u)[2] * (*v)[2];
}

static inline void product_vec4(vec4 *u, vec4 *v, scalar *c)
{
	*c = (*u)[0] * (*v)[0] + (*u)[1] * (*v)[1] + (*u)[2] * (*v)[2] + (*u)[3] * (*v)[3];
}


#define product(m, n, o)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), scalar *),		\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), vec3 *), product_c_vec3((void *)m, (void *)n, (void *)o),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), vec4 *), product_c_vec4((void *)m, (void *)n, (void *)o),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), mat3 *), product_c_mat3((void *)m, (void *)n, (void *)o),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), mat4 *), product_c_mat4((void *)m, (void *)n, (void *)o),	\
		choice_error())))),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), vec3 *), product_mat3_vec3((void *)m, (void *)n, (void *)o),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), mat3 *), product_mat3((void *)m, (void *)n, (void *)o),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), mat4 *), product_mat3_mat4((void *)m, (void *)n, (void *)o),	\
		choice_error()))),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat4 *),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), vec3 *), product_mat4_vec3((void *)m, (void *)n, (void *)o),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), vec4 *), product_mat4_vec4((void *)m, (void *)n, (void *)o),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), mat4 *), product_mat4((void *)m, (void *)n, (void *)o),	\
		choice_error()))),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), vec3 *),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), vec3 *), product_vec3((void *)m, (void *)n, (void *)o),	\
		choice_error()),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), vec4 *),	\
		__builtin_choose_expr(__builtin_types_compatible_p(typeof(n), vec4 *), product_vec4((void *)m, (void *)n, (void *)o),	\
		choice_error()),	\
		choice_error())))))


/* constructs cross produt matrix M from a vector m so that
   Mv = m x v */
static inline void cross_product_mat3(vec3 *v, mat3 *m)
{
	(*m)[0] =      0.0; (*m)[3] = -(*v)[2]; (*m)[6] =  (*v)[1];
	(*m)[1] =  (*v)[2]; (*m)[4] =      0.0; (*m)[7] = -(*v)[0];
	(*m)[2] = -(*v)[1]; (*m)[5] =  (*v)[0]; (*m)[8] =      0.0;
}

static inline void cross_product_mat4(vec4 *v, mat4 *m)
{
	(*m)[0] =      0.0; (*m)[4] = -(*v)[2]; (*m)[8]  =  (*v)[1]; (*m)[12] = 0.0;
	(*m)[1] =  (*v)[2]; (*m)[5] =      0.0; (*m)[9]  = -(*v)[0]; (*m)[13] = 0.0;
	(*m)[2] = -(*v)[1]; (*m)[6] =  (*v)[0]; (*m)[10] =      0.0; (*m)[14] = 0.0;
	(*m)[3] =      0.0; (*m)[7] =      0.0; (*m)[11] =      0.0; (*m)[15] = 0.0;
}

static inline void cross_product(vec3 *u, vec3 *v, vec3 *w)
{
	/* FIXME */ /* vk[0] = vi[0] * vj[0] + vi[1] * vj[1] + vi[2] + vj[2]; */
}

static inline scalar det_mat2(mat2 *m)
{
	return (*m)[0] * (*m)[3] - (*m)[1] * (*m)[2];
}

static inline scalar _det_mat2(scalar m0, scalar m1, scalar m2, scalar m3)
{
	return  m0 * m3 - m1 * m2;
}

static inline scalar det_mat3(mat3 *m)
{
	return  (*m)[0] * _det_mat2((*m)[4], (*m)[5], (*m)[7], (*m)[8]) -
		(*m)[3] * _det_mat2((*m)[1], (*m)[2], (*m)[7], (*m)[8]) +
		(*m)[6] * _det_mat2((*m)[1], (*m)[2], (*m)[4], (*m)[5]);
}

static inline scalar _det_mat3(scalar m0, scalar m1, scalar m2, scalar m3, scalar m4, scalar m5, scalar m6, scalar m7, scalar m8)
{
	return  m0 * _det_mat2(m4, m5, m7, m8) -
		m3 * _det_mat2(m1, m2, m7, m8) +
		m6 * _det_mat2(m1, m2, m4, m5);
}

static inline scalar det_mat4(mat4 *m)
{
	return  (*m)[0]  * _det_mat3((*m)[5], (*m)[6], (*m)[7], (*m)[9], (*m)[10], (*m)[11], (*m)[13], (*m)[14], (*m)[15]) -
		(*m)[4]  * _det_mat3((*m)[1], (*m)[2], (*m)[3], (*m)[9], (*m)[10], (*m)[11], (*m)[13], (*m)[14], (*m)[15]) +
		(*m)[8]  * _det_mat3((*m)[1], (*m)[2], (*m)[3], (*m)[5], (*m)[6], (*m)[7], (*m)[13], (*m)[14], (*m)[15]) -
		(*m)[12] * _det_mat3((*m)[1], (*m)[2], (*m)[3], (*m)[5], (*m)[6], (*m)[7], (*m)[9], (*m)[10], (*m)[11]);
}

#define det(m)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat2 *), det_mat2((void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), const mat2 *), det_mat2((void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *), det_mat3((void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), const mat3 *), det_mat3((void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat4 *), det_mat4((void *)m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), const mat4 *), det_mat4((void *)m),	\
	choice_error()))))))

static inline scalar tr_mat3(mat3 *m)
{
	return  (*m)[0] + (*m)[4] + (*m)[8];
}

static inline scalar tr_mat4(mat4 *m)
{
	return  (*m)[0] + (*m)[5] + (*m)[10] + (*m)[15];
}

#define tr(m)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *), tr_mat3(m),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat4 *), tr_mat4(m),	\
		choice_error()))

static inline void transpose_mat3(mat3 *m, mat3 *n)
{
	mat3 l;

	l[0] = (*m)[0];
	l[1] = (*m)[3];
	l[2] = (*m)[6];

	l[3] = (*m)[1];
	l[4] = (*m)[4];
	l[5] = (*m)[7];

	l[6] = (*m)[2];
	l[7] = (*m)[5];
	l[8] = (*m)[8];

	assign(&l, n);
};

static inline void transpose_mat4(mat4 *m, mat4 *n)
{
	mat4 l;

	l[0] = (*m)[0];
	l[1] = (*m)[4];
	l[2] = (*m)[8];
	l[3] = (*m)[12];

	l[4] = (*m)[1];
	l[5] = (*m)[5];
	l[6] = (*m)[9];
	l[7] = (*m)[13];

	l[8] = (*m)[2];
	l[9] = (*m)[6];
	l[10] = (*m)[10];
	l[11] = (*m)[14];

	l[12] = (*m)[3];
	l[13] = (*m)[7];
	l[14] = (*m)[11];
	l[15] = (*m)[15];

	assign(&l, n);
};

#define transpose(m, n)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *), transpose_mat3((void *)m, (void *)n),	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat4 *), transpose_mat4((void *)m, (void *)n),	\
	choice_error()))

static inline void translate(mat4 *m, scalar t0, scalar t1, scalar t2)
{
	(*m)[12] += t0;
	(*m)[13] += t1;
	(*m)[14] += t2;
}

static inline void scale(mat4 *m, scalar s0, scalar s1, scalar s2)
{
	(*m)[0] *= s0;
	(*m)[5] *= s1;
	(*m)[10] *= s2;
}

void rotation_mat4(mat4 *m, scalar a, scalar u0, scalar u1, scalar u2);
void rotate(mat4 *m, scalar a, scalar u0, scalar u1, scalar u2);

void invert_mat3(mat3 *m, mat3 *n);

#define invert(m, n)	\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(m), mat3 *), invert_mat3((void *)m, (void *)n),	\
	choice_error())
#endif /* _LINEAR_H_ */