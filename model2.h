#ifndef _MODEL_H_
#define _MODEL_H_

#include "linear.h"
#include "list.h"

struct element {
	struct element *parent;
	struct list_head childs;
	struct list_head list;

	mat4 tr; /* position transformation: rotation + translation */
	mat4 _tr; /* absolute position transformation */
	mat4 __tr; /* absolute position transformation */

	mat4 tv; /* velocity matrix: angular + linear */
	mat4 r_v; /* rotation matrix */
	vec4 a; /* angular velocity */
};

struct model {
	unsigned int elements;
	struct element element[40];
};

void model_init(struct model *m);

#endif /* _MODEL_H_ */
