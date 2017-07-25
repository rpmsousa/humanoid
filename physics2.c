#include "physics2.h"
#include "model2.h"
#include "list.h"

void velocity_relative_init(struct element *e)
{
	cross_product_mat4(&e->a, &e->tv);

	print("angular velocity", &e->a);
	print("velocity", &e->tv);
}

void position_absolute_update(struct element *e)
{
	struct element *p = e->parent;
	struct list_head *entry;
	struct element *child;

	if (p) {
		/* Absolute transform */
		product(&p->_tr, &e->tr, &e->_tr);
	} else {
		assign(&e->tr, &e->_tr);
	}

	for (entry = list_first(&e->childs); entry != &e->childs; entry = list_next(entry)) {
		child = container_of(entry, struct element, list);

		position_absolute_update(child);
	}
}

/* update position based on angular velocity */
void position_relative_update(struct element *e, float *temp)
{
	mat4 rotation;
	scalar a;

	product_vec4(&e->a, &e->a, &a);
	a = sqrtf(a);

	rotation_mat4(&rotation, *temp * a, e->a[0], e->a[1], e->a[2]);

	product(&e->__tr, &rotation, &e->tr);
}

void velocity(struct element *e, vec4 *r, vec4 *v)
{
	struct element *p;
	mat4 inv_tr, _inv_tr;
	vec4 _v, p_r;

//	printf("\n\n%p\n", e);

	assign_mat4(&mat4_unit, &inv_tr);

//	print("pos", r);

	/* Own relative velocity */
	product_mat4_vec4(&e->tv, r, v);

//	print("v", v);

	while ((p = e->parent)) {
		/* position in parent coordinates */
		product(&e->tr, r, &p_r);

//		print("p_pos", &p_r);

		/* velocity in parent coordinates */
		product_mat4_vec4(&p->tv, &p_r, &_v);

//		print("p_v", &_v);

		/* total velocity in own coordinates */
		transpose_mat4(&e->tr, &_inv_tr);
		_inv_tr[3] = 0.0;
		_inv_tr[7] = 0.0;
		_inv_tr[11] = 0.0;
		_inv_tr[15] = 1.0;

//		print("tr", &e->tr);
//		print("_inv tr", &_inv_tr);

//		print("inv tr", &inv_tr);

		product_mat4(&inv_tr, &_inv_tr, &inv_tr);

//		print("total inv tr", &inv_tr);

		product_mat4_vec4(&inv_tr, &_v, &_v);

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
	struct list_head *entry;
	struct element *child;

	assign(&e->tr, &e->__tr);

	if (p) {
		/* Absolute transform */
		product(&p->_tr, &e->tr, &e->_tr);
	} else {
		assign(&e->tr, &e->_tr);
	}

	velocity_relative_init(e);

	for (entry = list_first(&e->childs); entry != &e->childs; entry = list_next(entry)) {
		child = container_of(entry, struct element, list);

		position_init(child);
	}
}
