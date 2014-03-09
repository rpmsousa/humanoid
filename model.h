#ifndef _MODEL_H_
#define _MODEL_H_

#include <sys/time.h>
#include <GL/glu.h>

#include "linear.h"
#include "physics.h"

enum {
	TRUNK,
	WAIST,
	HIPS,
	HEAD,
	NECK,
	LEFT_ARM,
	LEFT_FOREARM,
	LEFT_HAND,
	RIGHT_ARM,
	RIGHT_FOREARM,
	RIGHT_HAND,
	LEFT_LEG,
	LEFT_FORELEG,
	LEFT_FOOT,
	RIGHT_LEG,
	RIGHT_FORELEG,
	RIGHT_FOOT,
	BONE_MAX
};

#define MAX_JOINTS	5

struct bone {
	/* dimensions */
	float dx;
	float dy;
	float dz;

	/* child bones */
	int childs;
	int child[MAX_JOINTS];

	int flag;

	vec3 cpos; /* child position where parent is attached, in unit coordinates */

	union {
		struct {
			vec3 ppos; /* parent position where child is attached, in unit coordinates */
			vec3 axis;
			float angle;
		};

		mat4 m; /* matrix transformation relative to parent */
	};

	float t;
	float t_min;
	float t_max;
	float p;
	float p_min;
	float p_max;
	float z;
	float z_min;
	float z_max;

	struct solid s;
};

struct model {
	vec3 cm; /* center of mass (in model coordinates) */

	float mass; /* total mass */
	vec3 pos; /* absolute position of center of mass */
	vec3 velocity; /* linear velocity (of the center of mass) */
	vec3 momentum;

	mat3 inertia; /* inertia matrix relative to center of mass */
	mat4 angular_pos; /* angular position relative to center of mass */
	vec3 angular_velocity; /* angular velocity (relative to the center of mass) */
	vec3 angular_momentum;

	struct timeval t;

	int bones;
	struct bone bone[];
};

extern struct model model_bulky;
extern struct model model_sticky;
extern struct model model_simple;

void model_init(struct model *m, float x);
void model_center_of_mass(struct model *m);
void model_inertia_matrix(struct model *m);
void model_momentum(struct model *m);
void model_angular_momentum(struct model *m);
void model_update_shape(struct model *m);
void model_movement(struct model *m);

#endif /* _MODEL_H_ */