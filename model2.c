#include "model2.h"
#include "physics2.h"

static void model_velocity_init(struct element *e)
{
	struct list_head *entry;
	struct element *child;

	velocity_init(e);

	for (entry = list_first(&e->childs); entry != &e->childs; entry = list_next(entry)) {
		child = container_of(entry, struct element, list);

		velocity_init(child);
	}
}

static void model_position_init(struct element *e)
{
	struct list_head *entry;
	struct element *child;

	position_init(e);

	for (entry = list_first(&e->childs); entry != &e->childs; entry = list_next(entry)) {
		child = container_of(entry, struct element, list);

		position_init(child);
	}
}

static void element_init(struct element *e)
{
	struct element *p = e->parent;

	if (p)
		list_add(&p->childs, &e->list);
}

void model_init(struct model *m)
{
	int i;

	for (i = 0; i < m->elements; i++) {
		struct element *e = &m->element[i];

		list_head_init(&e->childs);
	}

	for (i = 0; i < m->elements; i++)
		element_init(&m->element[i]);

	model_position_init(&m->element[0]);

	model_velocity_init(&m->element[0]);
}


static void model_velocity_update(struct element *e)
{
	struct list_head *entry;
	struct element *child;

	velocity_update(e);

	for (entry = list_first(&e->childs); entry != &e->childs; entry = list_next(entry)) {
		child = container_of(entry, struct element, list);

		velocity_update(child);
	}
}

static void model_position_update(struct element *e, float dt)
{
	struct list_head *entry;
	struct element *child;

	position_update(e, dt);

	for (entry = list_first(&e->childs); entry != &e->childs; entry = list_next(entry)) {
		child = container_of(entry, struct element, list);

		position_update(child, dt);
	}
}

void model_update(struct model *m, float dt)
{
	model_position_update(&m->element[0], dt);

	model_velocity_update(&m->element[0]);
}
