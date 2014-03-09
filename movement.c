#include <stdlib.h>
#include "movement.h"

void joint_adjust(struct bone *b, float dz, float dt, float dp)
{
	b->z += dz;
	if (b->z < b->z_min)
		b->z = b->z_min;
	else if (b->z > b->z_max)
		b->z = b->z_max;

	b->t += dt;
	if (b->t < b->t_min)
		b->t = b->t_min;
	else if (b->t > b->t_max)
		b->t = b->t_max;

	b->p += dp;
	if (b->p < b->p_min)
		b->p = b->p_min;
	else if (b->p > b->p_max)
		b->p = b->p_max;
}

void joint_adjust_random(struct model *model)
{
	int i;

	for (i = 0; i < 1; i++) {
		int b = ((float)random() / RAND_MAX) * (model->bones - 1) + 1;
		int incr = ((float)random() / RAND_MAX) * 20.0 - 10.0;
		int a = ((float)random() / RAND_MAX) * 3;

		switch (a) {
		case 0:
			joint_adjust(&model->bone[b], 0.0, 0.0, incr);
			break;

		case 1:
			joint_adjust(&model->bone[b], 0.0, incr, 0.0);
			break;

		case 2:
		default:
			joint_adjust(&model->bone[b], incr, 0.0, 0.0);
			break;
		}
	}
}
