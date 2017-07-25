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

	mat4 tr; /* basis transformation (relative to parent basis): rotation + translation */
	mat4 _tr; /* absolute position transformation */
	mat4 __tr; /* inital/constant absolute position transformation */

	mat4 tv; /* basis velocity (in matrix from: angular + linear, v = tv r) */
	mat4 _tv; /* velocity of basis unit vectors, one column per vector */
	mat4 __tv; /* velocity of basis unit vectors, one column per vector */
	mat4 ___tv; /* velocity of basis unit vectors, one column per vector */
	mat4 r_v; /* rotation matrix */
	vec4 a; /* angular velocity */
};

struct model {
	unsigned int elements;
	struct element element[40];
};

void model_update(struct model *m, float dt);
void model_init(struct model *m);

#endif /* _MODEL_H_ */
