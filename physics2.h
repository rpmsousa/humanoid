#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "linear.h"
#include "model2.h"

void velocity_update(struct element *e);
void position_update(struct element *e, float dt);
void velocity_init(struct element *e);
void position_init(struct element *e);

void local2global(struct element *e, vec4 *r1, vec4 *r2);
void local2parent(struct element *e, vec4 *r1, vec4 *r2);
void parent2local(struct element *e, vec4 *r1, vec4 *r2);
void local_velocity(struct element *e, vec4 *r, vec4 *v);
void frame_velocity(struct element *e, vec4 *r, vec4 *v);
void total_velocity(struct element *e, vec4 *r, vec4 *v);

#endif /* _PHYSICS_H_ */
