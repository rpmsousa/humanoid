#include "model.h"

struct model model_bulky = {
	.bones = BONE_MAX,

	.bone = {
		[HEAD] = {
			.dx = 0.15, .dy = 0.20, .dz = 0.20,

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

		[NECK]  = {
			.dx = 0.12, .dy = 0.10, .dz = 0.12,
			.childs = 1,
			.child[0] = HEAD,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 1.0, 0.0},
			.angle = 0.0,

			.z_min = -85,
			.z_max = 85,

			.t_min = -10,
			.t_max = 10,

			.p_min = -90,
			.p_max = 90,
		},
	
		[LEFT_HAND] = {
			.dx = 0.10, .dy = 0.15, .dz = 0.03,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 1.0, 0.0},
			.angle = 0.0,

			.z_min = -45,
			.z_max = 45,

			.t_min = -90,
			.t_max = 90,

			.p_min = -5,
			.p_max = 5,
		},


		[RIGHT_HAND] = {
			.dx = 0.10, .dy = 0.15, .dz = 0.03,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 1.0, 0.0},
			.angle = 0.0,

			.z_min = -45,
			.z_max = 45,

			.t_min = -90,
			.t_max = 90,

			.p_min = -5,
			.p_max = 5,
		},

		[LEFT_FOREARM] = {
			.dx = 0.09, .dy = 0.25, .dz = 0.09,

			.childs = 1,
			.child[0] = LEFT_HAND,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 1.0, 0.0},
			.angle = 0.0,

			.z_min = -45,
			.z_max = 0,

			.t_min = 0,
			.t_max = 90,

			.p_min = 0,
			.p_max = 0,
		},

		[RIGHT_FOREARM] = {
			.dx = 0.09, .dy = 0.25, .dz = 0.09,

			.childs = 1,
			.child[0] = RIGHT_HAND,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 1.0, 0.0},
			.angle = 0.0,

			.z_min = -45,
			.z_max = 0,

			.t_min = 0,
			.t_max = 90,

			.p_min = 0,
			.p_max = 0,
		},

		[LEFT_ARM] = {
			.dx = 0.10, .dy = 0.30, .dz = 0.10,
			.childs = 1,
			.child[0] = LEFT_FOREARM,

			.ppos = {1.1, 0.9, 0.5},
			.cpos = {0.5, 0.1, 0.5},
			.axis = {0.0, 0.0, 1.0},
			.angle = -180.0,

			.z_min = -45,
			.z_max = 20,

			.t_min = -90,
			.t_max = 180,

			.p_min = -80,
			.p_max = 95,
		},

		[RIGHT_ARM] = {
			.dx = 0.10, .dy = 0.30, .dz = 0.10,
			.childs = 1,
			.child[0] = RIGHT_FOREARM,

			.ppos = {-0.1, 0.9, 0.5},
			.cpos = {0.5, 0.1, 0.5},
			.axis = {0.0, 0.0, 1.0},
			.angle = 180.0,

			.z_min = -45,
			.z_max = 20,

			.t_min = -90,
			.t_max = 180,

			.p_min = -95,
			.p_max = 80,
		},

		[TRUNK] = {
			.dx = 0.45, .dy = 0.45, .dz = 0.15,
			.childs = 4,
			.child[0] = RIGHT_ARM,
			.child[1] = LEFT_ARM,
			.child[2] = NECK,
			.child[3] = WAIST,

			.ppos = {0.0, 0.0, 0.0},
			.cpos = {0.5, 0.5, 0.5},
			.angle = 0.0,

			.z_min = -45,
			.z_max = 45,

			.t_min = -180,
			.t_max = 20,

			.p_min = -90,
			.p_max = 90,
		},

		[WAIST] = {
			.dx = 0.35, .dy = 0.15, .dz = 0.14,
			.childs = 1,
			.child[0] = HIPS,

			.ppos = {0.5, 0.0, .5},
			.cpos = {0.5, 1.0, 0.5},
			.angle = 0.0,
		},

		[HIPS] = {
			.dx = 0.40, .dy = 0.10, .dz = 0.14,
			.childs = 2,
			.child[0] = LEFT_LEG,
			.child[1] = RIGHT_LEG,

			.ppos = {0.5, 0.0, .5},
			.cpos = {0.5, 1.0, 0.5},
			.angle = 0.0,
		},
	
		[LEFT_LEG] = {
			.dx = 0.20, .dy = 0.45, .dz = 0.15,
			.childs = 1,
			.child[0] = LEFT_FORELEG,

			.ppos = {0.25, 0.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 0.0, 1.0},
			.angle = -180.0,

			.z_min = -45,
			.z_max = 10,

			.t_min = -45,
			.t_max = 170,

			.p_min = -90,
			.p_max = 90,
		},

		[RIGHT_LEG] = {
			.dx = 0.20, .dy = 0.45, .dz = 0.15,
			.childs = 1,
			.child[0] = RIGHT_FORELEG,

			.ppos = {0.75, 0.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 0.0, 1.0},
			.angle = 180.0,

			.z_min = -45,
			.z_max = 10,

			.t_min = -45,
			.t_max = 170,

			.p_min = -90,
			.p_max = 90,
		},

		[LEFT_FORELEG] = {
			.dx = 0.12, .dy = 0.45, .dz = 0.12,
			.childs = 1,
			.child[0] = LEFT_FOOT,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 0.0, 1.0},
			.angle = 0.0,

			.z_min = -0,
			.z_max = 0,

			.t_min = -170,
			.t_max = 5,

			.p_min = -5,
			.p_max = 5,
		},

		[RIGHT_FORELEG] = {
			.dx = 0.12, .dy = 0.45, .dz = 0.12,
			.childs = 1,
			.child[0] = RIGHT_FOOT,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.0, 0.5},
			.axis = {0.0, 0.0, 1.0},
			.angle = 0.0,

			.z_min = -0,
			.z_max = 0,

			.t_min = -170,
			.t_max = 5,

			.p_min = -5,
			.p_max = 5,
		},
	
		[LEFT_FOOT] = {
			.dx = 0.10, .dy = 0.25, .dz = 0.07,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.3, 1.0},
			.axis = {1.0, 0.0, 0.0},
			.angle = 90.0,
		},

		[RIGHT_FOOT] = {
			.dx = 0.10, .dy = 0.25, .dz = 0.07,

			.ppos = {0.5, 1.0, 0.5},
			.cpos = {0.5, 0.3, 1.0},
			.axis = {1.0, 0.0, 0.0},
			.angle = 90.0,
		},
	}
};
