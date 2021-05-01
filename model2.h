#ifndef _MODEL_H_
#define _MODEL_H_

#include "linear.h"
#include "list.h"

/* Each element has a single parent */
/* Each element can have multiple childs */
/* First element has no parent (parent basis is the cartesian basis, parent velocity is 0) */

struct element {
	struct element *parent;
	struct list_head childs;
	struct list_head list;
	unsigned int index;

	mat4 tr; /* basis transformation (relative to parent basis): rotation + translation (parent coordinates) */
		/* The first three columns vectors are the new basis unit _vectors_ in parent coordinates:
		 * ux = (1, 0, 0) - (0, 0, 0)
		 * uy = (0, 1, 0) - (0, 0, 0)
		 * uz = (0, 0, 1) - (0, 0, 0)
		 * The fourth is the new basis origin (0, 0, 0) in parent coordinates */
	mat4 _tr; /* absolute position transformation (cartesian coordinates) */
	mat4 __tr; /* initial/constant absolute position transformation */

	mat4 tr_inverse;

	vec4 v0; /* basis origin speed vector, in ... */
	vec4 _v0;

	mat4 _tv; /* velocity of basis unit vectors, one column per vector (parent coordinates) */
	mat4 __tv; /* velocity of basis unit vectors, one column per vector (own coordinates) */
	mat4 ___tv; /* velocity of basis unit vectors, one column per vector */
	mat4 ____tv; /* total velocity, parent coordinates */

	vec4 a; /* angular velocity vector (parent coordinates, degrees / s), v_p = a x r. The advantage of using parent coordinates
		is that the vector coordinates remains fixed as the basis rotates */

	mat4 tv; /* velocity matrix containing angular + linear, v_t = v_p + v_l = a x r + v_l = tv r) (parent coordinates) */
};

struct model {
	unsigned int elements;
	struct element element[40];
};

void model_update(struct model *m, float dt);
void model_init(struct model *m);

#endif /* _MODEL_H_ */
