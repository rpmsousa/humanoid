#include "model.h"

struct model model_simple = {
	.bones = 2,

	.bone = {
		[0] = {
			.dx = 0.5, .dy = 0.5, .dz = 0.5,
			.childs = 1,
			.child[0] = 1,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 1.0, 0.0},
			.angle = 0.0,

			.z_min = -5,
			.z_max = 5,

			.t_min = -80,
			.t_max = 80,

			.p_min = 0,
			.p_max = 0,
		},

		[1]  = {
			.dx = 0.1, .dy = 1.0, .dz = 10,

			.ppos = {2.0, 0.5, 0.5},
			.cpos = {-2.0, 0.5, 0.5},
			.axis = {0.0, 1.0, 0.0},
			.angle = 0.0,

			.z_min = -85,
			.z_max = 85,

			.t_min = -10,
			.t_max = 10,

			.p_min = -90,
			.p_max = 90,
		},
	}
};
