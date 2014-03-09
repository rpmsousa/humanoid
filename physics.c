#include "physics.h"

static void cube_volume(struct solid *s)
{
	s->volume = s->cube.dx * s->cube.dy * s->cube.dz;
}

static void cube_mass(struct solid *s)
{
	/* Assumes uniform density */
	s->mass = s->density * s->volume;
}

static void cube_center_of_mass(struct solid *s)
{
	s->cm[0] = 0.5 * s->cube.dx;
	s->cm[1] = 0.5 * s->cube.dy;
	s->cm[2] = 0.5 * s->cube.dz;
}

static void cube_inertia_matrix(struct solid *s)
{
	/* For rotations about the center of mass and in the principal axis base */
	s->inertia[0] = s->mass / 12.0 * (s->cube.dy * s->cube.dy + s->cube.dz * s->cube.dz);
	s->inertia[1] = 0.0;
	s->inertia[2] = 0.0;
	s->inertia[3] = 0.0;
	s->inertia[4] = s->mass / 12.0 * (s->cube.dx * s->cube.dx + s->cube.dz * s->cube.dz);
	s->inertia[5] = 0.0;
	s->inertia[6] = 0.0;
	s->inertia[7] = 0.0;
	s->inertia[8] = s->mass / 12.0 * (s->cube.dx * s->cube.dx + s->cube.dy * s->cube.dy);
}

const struct solid_type_properties solid_properties[SOLID_TYPE_MAX] = {
	[SOLID_TYPE_CUBE] = {
		.mass = cube_mass,
		.volume = cube_volume,
		.center_of_mass = cube_center_of_mass,
		.inertia_matrix = cube_inertia_matrix,
	},
};


void solid_volume(struct solid *s)
{
	solid_properties[s->type].volume(s);
}

void solid_mass(struct solid *s)
{
	solid_properties[s->type].mass(s);
}

void solid_center_of_mass(struct solid *s)
{
	solid_properties[s->type].center_of_mass(s);
}

void solid_inertia_matrix(struct solid *s)
{
	solid_properties[s->type].inertia_matrix(s);
}


void solid_center_of_mass_abs(struct solid *s, vec3 *cm)
{
	product(&s->t_abs, &s->cm, &s->cm_abs);

	product(&s->mass, &s->cm_abs, cm);
}

void solid_inertia_matrix_abs(struct solid *s, vec3 *p, mat3 *inertia)
{
	vec3 d;
	mat3 t, t_t;
	scalar c;

	/* transform inertia matrix from principle axis to current orientation */
	/* I' = t I transpose(t) */
	/* inertia' = t s->inertia transpose(t) */

	/* extract orthonormal transformation matrix */
	mat4_2_mat3(&s->t_abs, &t);

	transpose(&t, &t_t);
	product(&t, &s->inertia, &t);
	product(&t, &t_t, inertia);

	/* transform inertia matrix from solid center of mass to new reference point */ 
	/* I'' = I' - m [d]^2 */
	/* inertia'' = inertia' - m * [d] * [d] */
	d[0] = (*p)[0] - s->cm_abs[0];
	d[1] = (*p)[1] - s->cm_abs[1];
	d[2] = (*p)[2] - s->cm_abs[2];

	cross_product_mat3(&d, &t);

	product(&t, &t, &t);

	c = -s->mass;
	product(&c, &t, &t);

	sum(inertia, &t, inertia);
}


void solid_init(struct solid *s, enum solid_type type, float density, float params[])
{
	s->type = type;
	s->density = density;
	s->params[0] = params[0];
	s->params[1] = params[1];
	s->params[2] = params[2];

	solid_volume(s);
	solid_mass(s);
	solid_center_of_mass(s);
	solid_inertia_matrix(s);
}


void linear_velocity()
{
	
	
}

