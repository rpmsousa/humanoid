#include "physics2.h"
#include "model2.h"
#include "list.h"


/* update position based on angular velocity */
void position_relative_update(struct element *e, float dt)
{
#if 1
	mat4 rotation;
	scalar a;

	product(&e->a, &e->a, &a);
	a = sqrtf(a);

	rotation_mat4(&rotation, dt * a, e->a[0], e->a[1], e->a[2]);

	product(&e->__tr, &rotation, &e->tr);
#else
	mat4 dr;
	scalar c;

	/* This calculation diverges, since it needs to continuously integrate dr over time */
	printf("element: %p\n", e);
	print("tr", &e->tr);
	print("tv", &e->tv);
	

	product(&e->tv, &e->tr, &dr);
	c = 0.01;
	product_c_mat4(&c, &dr, &dr);

	/* The velocity is relative to the local basis origin, reset the translation */
	dr[12] = 0.0;
	dr[13] = 0.0;
	dr[14] = 0.0;
	dr[15] = 0.0;

	sum(&e->tr, &dr, &e->tr);

	print("dr", &dr);
	print("tr new", &e->tr);
#endif
}

void position_update(struct element *e, float dt)
{
	struct element *p = e->parent;

	position_relative_update(e, dt);

	if (p) {
		/* Absolute transform */
		product(&p->_tr, &e->tr, &e->_tr);
	} else {
		assign(&e->tr, &e->_tr);
	}
}


/* Returns global coordinates for a local vector */
void local2global(struct element *e, vec4 *r1, vec4 *r2)
{
	product(&e->_tr, r1, r2);
}

/* Returns parent coordinates for a local vector */
void local2parent(struct element *e, vec4 *r1, vec4 *r2)
{
	product(&e->tr, r1, r2);
}

/* Returns local coordinates for a parent vector */
void parent2local(struct element *e, vec4 *r1, vec4 *r2)
{
	mat4 inv_tr;

	invert_mat4(&e->tr, &inv_tr);

	product(&inv_tr, r1, r2);
}

/* Returns relative velocity for a point, in local coordinates */
void _velocity_local(struct element *e, vec4 *r, vec4 *v)
{
	/* Own relative velocity, in local coordinates */
	product(&e->tv, r, v);
}

/* Returns relative velocity for a point, in global coordinates */
void velocity_local(struct element *e, vec4 *r, vec4 *v)
{
	_velocity_local(e, r, v);
	local2global(e, v, v);
}

/* Returns the absolute velocity vector for a point in element e basis.
 * r is the relative position of the point.
 * v is the calculated absolute velocity (in element e coordinates) */
void velocity(struct element *e, vec4 *r, vec4 *v)
{
	struct element *p;
//	mat4 inv_tr, _inv_tr;
	vec4 _v, p_r;

//	printf("\n\n%p\n", e);

//	assign(&mat4_unit, &inv_tr);

//	print("pos", r);

	/* Own relative velocity, in local coordinates */
	product(&e->tv, r, v);

	/* Own relative velocity, in global coordinates */
	product(&e->_tr, v, v);

//	print("v", v);

	while ((p = e->parent)) {
		/* position in parent coordinates */
		//product(&e->tr, &vec4_null, &p_r);
		product(&e->tr, r, &p_r);

		print("p_pos", &p_r);

		/* velocity in parent coordinates */
		product(&p->tv, &p_r, &_v);

		/*velocity in global coordinates */
		product(&p->_tr, &_v, &_v);
	
//		print("p_v", &_v);

		/* total velocity in own coordinates */
	//	transpose(&e->tr, &_inv_tr);
	//	_inv_tr[3] = 0.0;
	//	_inv_tr[7] = 0.0;
	//	_inv_tr[11] = 0.0;
	//	_inv_tr[15] = 1.0;

//		print("tr", &e->tr);
//		print("_inv tr", &_inv_tr);

//		print("inv tr", &inv_tr);

		//product(&inv_tr, &_inv_tr, &inv_tr);

//		print("total inv tr", &inv_tr);

		//product(&inv_tr, &_v, &_v);

//		print("_v", &_v);

		sum(v, &_v, v);

//		print("total _v", &v);

		e = p;
		r = &p_r;
	}
}


void position_init(struct element *e)
{
	struct element *p = e->parent;

	/* __tr = tr */
	assign(&e->tr, &e->__tr);

	if (p) {
		/* Absolute transform */
		/* _tr = parent _tr x tr */
		product(&p->_tr, &e->tr, &e->_tr);
	} else {
		/* _tr = tr */
		assign(&e->tr, &e->_tr);
	}
}

void velocity_update(struct element *e)
{
	
}

void velocity_init(struct element *e)
{
	struct element *p = e->parent;

	/* Relative velocity */
	cross_product_mat4(&e->a, &e->tv);

	print("angular velocity", &e->a);
	print("velocity", &e->tv);

	/* Basis unit vectors velocity */
	if (p) {

	} else {


	}
}