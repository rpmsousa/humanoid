#ifndef _DRAW_H_
#define _DRAW_H_

#include <GL/glu.h>

void draw_vec3(vec3 *o, vec3 *v);
void draw_vec4(vec4 *o, vec4 *v);
void draw_vec4_color3f(vec4 *o, vec4 *v, float r, float g, float b);

static inline void draw_vec4_red(vec4 *o, vec4 *v)
{
	draw_vec4_color3f(o, v, 1, 0, 0);
}

static inline void draw_vec4_green(vec4 *o, vec4 *v)
{
	draw_vec4_color3f(o, v, 0, 1, 0);
}

static inline void draw_vec4_blue(vec4 *o, vec4 *v)
{
	draw_vec4_color3f(o, v, 0, 0, 1);
}

static inline void draw_vec4_white(vec4 *o, vec4 *v)
{
	draw_vec4_color3f(o, v, 1, 1, 1);
}

static inline void vector_draw(vec3 *v)
{
	draw_vec3(NULL, v);
}

GLuint frame_display_list(void);

GLuint cube_display_list(void);

GLuint cylinder_display_list(float r, float h, int faces);

GLuint cone_display_list(float r, float h, int faces);

GLuint vector_display_list(float l);

void draw_init(void);

void draw_segment_vec4(vec4 *p0, vec4 *p1);

#endif /* _DRAW_H_ */