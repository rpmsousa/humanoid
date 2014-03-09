#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "linear.h"

enum solid_type {
	SOLID_TYPE_CUBE,
	SOLID_TYPE_CYLINDER,
	SOLID_TYPE_MAX
};

#define SOLID_MAX_PARAMS 5

#define SOLID_DENSITY_DEFAULT	(1.0 / (0.1 * 0.1 * 0.1)) /* 1Kg/dm3 */

struct solid {
	enum solid_type type;
	mat4 t_abs; /* matrix transformation to absolute/model coordinates */
	vec3 cm_abs; /* center of mass, absolute/model coordinates */

	float density;
	float volume;
	float mass;
	mat3 inertia; /* inertia matrix, relative coordinates */
	vec3 cm; /* center of mass, relative coordinates */

	union {
		struct {
			float dx;
			float dy;
			float dz;
		} cube;
		
		float params[SOLID_MAX_PARAMS];
	};
};

struct solid_type_properties {
	void (*mass)(struct solid *s);
	void (*volume)(struct solid *s);
	void (*center_of_mass)(struct solid *s);
	void (*inertia_matrix)(struct solid *s);
};

void solid_volume(struct solid *s);
void solid_mass(struct solid *s);
void solid_center_of_mass_abs(struct solid *s, vec3 *cm);
void solid_inertia_matrix_abs(struct solid *s, vec3 *p, mat3 *inertia);
void solid_init(struct solid *s, enum solid_type type, float density, float params[]);

#endif /* _PHYSICS_H_ */
