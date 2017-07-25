#ifndef _DRAW_H_
#define _DRAW_H_

#include <GL/glu.h>

void draw_vec3(vec3 *o, vec3 *v);
void draw_vec4(vec4 *o, vec4 *v);

static inline void vector_draw(vec3 *v)
{
	draw_vec3(NULL, v);
}

GLuint frame_display_list(void);

GLuint cube_display_list(void);

GLuint cylinder_display_list(float step);

#endif /* _DRAW_H_ */