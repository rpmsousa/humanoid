#include "physics2.h"
#include "model2.h"
#include "list.h"


/* update position based on angular velocity */
void position_relative_update(struct element *e, float dt)
{
#if 1
	mat4 rotation;
	scalar a;

	a = norm_vec3((vec3 *)&e->a);

	rotation_mat4(&rotation, dt * a, e->a[0], e->a[1], e->a[2]);

	product(&e->__tr, &rotation, &e->tr);
#else
	mat4 dr;
	scalar c;

	/* This calculation diverges, since it needs to continuously integrate dr over time */
	//printf("element: %p\n", e);
//	print("tr", &e->tr);
//	print("tv", &e->tv);
	

	product(&e->tv, &e->tr, &dr);
	c = 0.01;
	product_c_mat4(&c, &dr, &dr);

	/* The velocity is relative to the local basis origin, reset the translation */
	dr[12] = 0.0;
	dr[13] = 0.0;
	dr[14] = 0.0;
	dr[15] = 0.0;

	sum(&e->tr, &dr, &e->tr);

//	print("dr", &dr);
//	print("tr new", &e->tr);
#endif

	invert_mat4(&e->tr, &e->tr_inverse);
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


/* Returns global coordinates for local coordinates */
void local2global(struct element *e, vec4 *r1, vec4 *r2)
{
	product(&e->_tr, r1, r2);
}

/* Returns parent coordinates for local coordinates */
void local2parent(struct element *e, vec4 *r1, vec4 *r2)
{
	product(&e->tr, r1, r2);
}

/* Returns local coordinates for parent coordinates */
void parent2local(struct element *e, vec4 *r1, vec4 *r2)
{
	product(&e->tr_inverse, r1, r2);
}

/* Returns global coordinates for parent coordinates */
void parent2global(struct element *e, vec4 *r1, vec4 *r2)
{
	if (e->parent)
		local2global(e->parent, r1, r2);
	else
		local2global(e, r1, r2);
}

/* Returns local velocity (parent coordinates) for a point (parent coordinates) */
void local_velocity(struct element *e, vec4 *r, vec4 *v)
{
	vec4 _r; /* vector from element basis origin to r, in parent coordinates */

	sub(r, (vec4 *)&e->tr[12], &_r);

	/* Velocity in element frame, in parent coordinates */
	product(&e->tv, &_r, v);
}

/* Returns velocity (parent coordinates) for a point (parent coordinates) due to parent frame velocity */
void frame_velocity(struct element *e, vec4 *r, vec4 *v)
{
	if (e->parent) {
		/* velocity due to basis rotation around origin */
		product(&e->parent->____tv, r, v);

		/* add origin velocity */
		sum(&e->parent->_v0, v, v);
	} else
		product(&mat4_null, r, v);
}

/* Returns total velocity (parent coordinates) for a point (parent coordinates) */
void total_velocity(struct element *e, vec4 *r, vec4 *v)
{
	vec4 local_v, frame_v;

	local_velocity(e, r, &local_v);
	frame_velocity(e, r, &frame_v);

	sum(&local_v, &frame_v, v);
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

	invert_mat4(&e->tr, &e->tr_inverse);
}

void velocity_update(struct element *e)
{
	vec4 r;

	/* total velocity of basis origin */
	/* same as frame velocity, since local velocity for basis origin is always 0 */
	local2parent(e, &vec4_null, &r);
	total_velocity(e, &r, &e->v0);
	parent2local(e, &e->v0, &e->_v0);

	local2parent(e, &vec4_unit_x, &r);
	local_velocity(e, &r, (vec4 *)&e->_tv[0]);
	frame_velocity(e, &r, (vec4 *)&e->__tv[0]);
	total_velocity(e, &r, (vec4 *)&e->___tv[0]);

	local2parent(e, &vec4_unit_y, &r);
	local_velocity(e, &r, (vec4 *)&e->_tv[4]);
	frame_velocity(e, &r, (vec4 *)&e->__tv[4]);
	total_velocity(e, &r, (vec4 *)&e->___tv[4]);

	local2parent(e, &vec4_unit_z, &r);
	local_velocity(e, &r, (vec4 *)&e->_tv[8]);
	frame_velocity(e, &r, (vec4 *)&e->__tv[8]);
	total_velocity(e, &r, (vec4 *)&e->___tv[8]);

	/* basis unit vector speed, relative to basis origin */
	sub((vec4 *)&e->___tv[0], &e->v0, (vec4 *)&e->____tv[0]);
	sub((vec4 *)&e->___tv[4], &e->v0, (vec4 *)&e->____tv[4]);
	sub((vec4 *)&e->___tv[8], &e->v0, (vec4 *)&e->____tv[8]);

	parent2local(e, (vec4 *)&e->____tv[0], (vec4 *)&e->____tv[0]);
	parent2local(e, (vec4 *)&e->____tv[4], (vec4 *)&e->____tv[4]);
	parent2local(e, (vec4 *)&e->____tv[8], (vec4 *)&e->____tv[8]);
}

void velocity_init(struct element *e)
{
	vec4 w; /*angular velocity vector in radians/s */

	w[0] = degrees_2_radians(e->a[0]);
	w[1] = degrees_2_radians(e->a[1]);
	w[2] = degrees_2_radians(e->a[2]);
	w[3] = degrees_2_radians(e->a[3]);

	/* Basis velocity, in parent coordinates */
	cross_product_mat4(&w, &e->tv);

	printf("Element: %u\n", e->index);
	print("local angular velocity", &e->a);
	print("local velocity", &e->tv);

	velocity_update(e);
}
