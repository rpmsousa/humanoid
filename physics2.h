#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "linear.h"
#include "model2.h"

void position_absolute_update(struct element *e);
void position_relative_update(struct element *e, float *t);
void position_init(struct element *e);

#endif /* _PHYSICS_H_ */
