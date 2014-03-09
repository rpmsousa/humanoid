#include "model.h"

static void bone_init(struct bone *b)
{
	float params[] = {b->dx, b->dy, b->dz};

	solid_init(&b->s, SOLID_TYPE_CUBE, SOLID_DENSITY_DEFAULT, params);
}

void model_mass(struct model *m)
{
	struct bone *b;
	int i;

	m->mass = 0.0;

	for (i = 0; i < m->bones; i++) {
		b = &m->bone[i];

		m->mass += b->s.mass;
	}
}

void model_center_of_mass(struct model *m)
{
	struct bone *b;
	vec3 cm;
	int i;

	/* Reset center of mass */
	m->cm[0] = 0.0;
	m->cm[1] = 0.0;
	m->cm[2] = 0.0;

	for (i = 0; i < m->bones; i++) {
		b = &m->bone[i];

		solid_center_of_mass_abs(&b->s, &cm);
		sum(&cm, &m->cm, &m->cm);
	}

	m->cm[0] /= m->mass;
	m->cm[1] /= m->mass;
	m->cm[2] /= m->mass;
}

void model_inertia_matrix(struct model *m)
{
	struct bone *b;
	mat3 inertia;
	int i;

	m->inertia[0] = 0;
	m->inertia[1] = 0;
	m->inertia[2] = 0;
	m->inertia[3] = 0;
	m->inertia[4] = 0;
	m->inertia[5] = 0;
	m->inertia[6] = 0;
	m->inertia[7] = 0;
	m->inertia[8] = 0;

	for (i = 0; i < m->bones; i++) {

		b = &m->bone[i];

		solid_inertia_matrix_abs(&b->s, &m->cm, &inertia);
		sum(&m->inertia, &inertia, &m->inertia);
	}
}

void model_momentum(struct model *m)
{
	product(&m->mass, &m->velocity, &m->momentum);
}

void model_angular_momentum(struct model *m)
{
	product(&m->inertia, &m->angular_velocity, &m->angular_momentum);
}


/* Update bone transform, relative to parent */
void bone_update_position(struct model *m, struct bone *b, struct bone *p, mat4 *t)
{
	int i;

	identity(&b->s.t_abs);
//	scale(&t, b->dx, b->dy, b->dz);

	translate(&b->s.t_abs, -b->dx * b->cpos[0], -b->dy * b->cpos[1], -b->dz * b->cpos[2]);

	rotate(&b->s.t_abs, b->z, 0.0, 1.0, 0.0);
	rotate(&b->s.t_abs, b->t, 1.0, 0.0, 0.0);
	rotate(&b->s.t_abs, b->p, 0.0, 1.0, 0.0);

	if (!b->flag) {
		mat4 t1;

		identity(&t1);

		rotate(&t1, b->angle, b->axis[0], b->axis[1], b->axis[2]);

		if (p)
			translate(&t1, p->dx * b->ppos[0], p->dy * b->ppos[1], p->dz * b->ppos[2]);

		assign(&t1, &b->m);

		b->flag = 1;
	}

	product(&b->m, &b->s.t_abs, &b->s.t_abs);
	product(t, &b->s.t_abs, &b->s.t_abs);

	for (i = 0; i < b->childs; i++)
		bone_update_position(m, &m->bone[b->child[i]], b, &b->s.t_abs);
}

void model_update_shape(struct model *m)
{
	mat4 t;

	identity(&t);

	bone_update_position(m, &m->bone[0], NULL, &t);

	/* Only need to be updated when relative positions of bones change */
	model_center_of_mass(m);
	model_inertia_matrix(m);
}

void model_movement(struct model *m)
{
	struct timeval now;
	vec3 v;
	float dt;
	float angle;
	mat3 t;
	float e;

	gettimeofday(&now, NULL);
	dt = (now.tv_usec - m->t.tv_usec) / 1000000.0 + (now.tv_sec - m->t.tv_sec);
	m->t = now;

	invert(&m->inertia, &t);

//	print("inertia", &m->inertia);
//	print("inertia invert", &t);

	product(&t, &m->angular_momentum, &m->angular_velocity);

	product(&m->angular_velocity, &m->angular_velocity, &angle);
	angle = sqrt(angle) * dt;

	rotate(&m->angular_pos, angle, m->angular_velocity[0], m->angular_velocity[1], m->angular_velocity[2]);

	model_momentum(m);

//	print("ang m", &m->angular_momentum);
//	print("ang v", &m->angular_velocity);
//	print("cm", &m->cm);

	product(&m->inertia, &m->angular_velocity, &v);
	product(&v, &m->angular_velocity, &e);
//	printf("energy %f\n", e/2.0);
}

void model_init(struct model *m, float x)
{
	struct bone *b;
	int i;

	m->velocity[0] = 1.0;

	m->angular_velocity[0] = 90.0;
	m->angular_velocity[1] = 0.0;
	m->angular_velocity[2] = 0.0;

	m->pos[0] = x;
	m->pos[1] = 0.0;
	m->pos[2] = 0.0;

	identity(&m->angular_pos);

	for (i = 0; i < m->bones; i++) {
		b = &m->bone[i];

		bone_init(b);
	}

	model_mass(m);

	model_update_shape(m);

	model_angular_momentum(m);
}
