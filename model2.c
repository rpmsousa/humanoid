#include "model2.h"
#include "physics2.h"

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

	position_init(&m->element[0]);
}
