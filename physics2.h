#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "linear.h"
#include "model2.h"

void velocity_update(struct element *e);
void position_update(struct element *e, float dt);
void velocity_init(struct element *e);
void position_init(struct element *e);

#endif /* _PHYSICS_H_ */
