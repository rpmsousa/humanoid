#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "model.h"

void joint_adjust(struct bone *b, float dz, float dt, float dp);

void joint_adjust_random(struct model *model);

#endif /* _MOVEMENT_H_ */
